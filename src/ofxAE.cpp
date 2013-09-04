#include "ofxAE.h"

#include "ofxJSONElement.h"
#include "ofFileUtils.h"
#include "ofxAEComposition.h"
#include "ofxAEAVLayer.h"
#include "ofxAELayerHelper.h"
#include "ofxAEMarker.h"
#include "ofxAEMask.h"
#include "ofxAEProperty.h"
#include "ofxAECameraLayer.h"

namespace ofxAE {
Composition* Loader::loadComposition(const string& filepath)
{
	string ext = ofFilePath::getFileExt(filepath);
	if(ext == "json") {
		ofxJSONElement json;
		if(json.open(filepath)) {
			return loadCompositionJson(json);
		}
		return NULL;
	}
}
Composition* Loader::loadCompositionJson(const Json::Value& json)
{
	Composition *ret = new Composition();
	// Basics
	ret->name_ = json.get("name", "noname").asString();
	ret->allocate(json.get("width", 1).asFloat(), json.get("height", 1).asFloat());
	// Layers
	const Json::Value& layers = json.get("layer", Json::Value::null);
	if(layers.isArray()) {
		map<string, Layer*> work;
		int layer_count = layers.size();
		for(int i = layer_count; i--;) {	// reverse iterate for draw priority
			const Json::Value& layer = layers.get(i, Json::Value::null);
			loadLayerJson(layer, ret);
			if(layer.isMember("parent")) {
				work.insert(pair<string,Layer*>(layer.get("parent", Json::Value::null).asString(), ret->layer_.back()));
			}
		}
		// search parent
		for(map<string,Layer*>::iterator it_w = work.begin(); it_w != work.end(); ++it_w) {
			for(vector<Layer*>::iterator it_l = ret->layer_.begin(); it_l != ret->layer_.end(); ++it_l) {
				if((*it_w).first == (*it_l)->name_) {
					(*it_w).second->setParent(*it_l);
					break;
				}
			}
		}
	}
	
	return ret;
}

void Loader::loadLayerJson(const Json::Value &json, Composition *comp)
{
	Layer *layer = NULL;
	const string& type_name = json.get("layerType", "unknown").asString();
	if(type_name == "composition"
	   || type_name == "solid"
	   || type_name == "still") {
		layer = loadAVLayerJson(json);
	}
	if(type_name == "camera") {
		CameraLayer *c_layer = loadCameraLayerJson(json, comp);
		comp->camera_.push_back(c_layer);
		layer = c_layer;
	}
	if(!layer) {
		layer = new Layer();
	}
	layer->name_ = json.get("name", "noname").asString();
	const Json::Value& properties = json.get("property", Json::Value::null);
	if(properties.isMember("active")) {
		LayerActiveProp *prop = new LayerActiveProp();
		prop->setTarget(layer);
		const Json::Value& keys = properties.get("active", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			bool value = keys[name].asBool();
			prop->addKey(key_frame, value);
		}
		layer->property_.push_back(prop);
	}
	if(properties.isMember("Position")) {
		LayerPositionProp *prop = new LayerPositionProp();
		prop->setTarget(layer);
		const Json::Value& keys = properties.get("Position", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			ofVec3f value = ofVec3f(keys[name][0].asFloat(),keys[name][1].asFloat(),-keys[name][2].asFloat());
			prop->addKey(key_frame, value);
		}
		layer->property_.push_back(prop);
	}
	if(properties.isMember("Scale")) {
		LayerScaleProp *prop = new LayerScaleProp();
		prop->setTarget(layer);
		const Json::Value& keys = properties.get("Scale", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			ofVec3f value = ofVec3f(keys[name][0].asFloat(),keys[name][1].asFloat(),keys[name][2].asFloat())/100.f;
			prop->addKey(key_frame, value);
		}
		layer->property_.push_back(prop);
	}
	if(properties.isMember("RotationX")) {
		LayerRotationXProp *prop = new LayerRotationXProp();
		prop->setTarget(layer);
		const Json::Value& keys = properties.get("RotationX", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			float value = -keys[name].asFloat();
			prop->addKey(key_frame, value);
		}
		layer->property_.push_back(prop);
	}
	if(properties.isMember("RotationY")) {
		LayerRotationYProp *prop = new LayerRotationYProp();
		prop->setTarget(layer);
		const Json::Value& keys = properties.get("RotationY", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			float value = -keys[name].asFloat();
			prop->addKey(key_frame, value);
		}
		layer->property_.push_back(prop);
	}
	if(properties.isMember("RotationZ")) {
		LayerRotationZProp *prop = new LayerRotationZProp();
		prop->setTarget(layer);
		const Json::Value& keys = properties.get("RotationZ", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			float value = keys[name].asFloat();
			prop->addKey(key_frame, value);
		}
		layer->property_.push_back(prop);
	}
	if(properties.isMember("AnchorPoint")) {
		LayerAnchorPointProp *prop = new LayerAnchorPointProp();
		prop->setTarget(layer);
		const Json::Value& keys = properties.get("AnchorPoint", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			ofVec3f value = ofVec3f(keys[name][0].asFloat(),keys[name][1].asFloat(),-keys[name][2].asFloat());
			prop->addKey(key_frame, value);
		}
		layer->property_.push_back(prop);
	}
	if(properties.isMember("Opacity")) {
		LayerOpacityProp *prop = new LayerOpacityProp();
		prop->setTarget(layer);
		const Json::Value& keys = properties.get("Opacity", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			float value = keys[name].asFloat()/100.f;
			prop->addKey(key_frame, value);
		}
		layer->property_.push_back(prop);
	}
	if(properties.isMember("Orientation")) {
		LayerOrientationProp *prop = new LayerOrientationProp();
		prop->setTarget(layer);
		const Json::Value& keys = properties.get("Orientation", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			ofVec3f value = ofVec3f(-keys[name][0].asFloat(),-keys[name][1].asFloat(),keys[name][2].asFloat());
			prop->addKey(key_frame, value);
		}
		layer->property_.push_back(prop);
	}
	
	// Markers
	const Json::Value& markers = json.get("marker", Json::Value::null);
	if(markers.isArray()) {
		int marker_count = markers.size();
		for(int i = 0; i < marker_count; ++i) {
			const Json::Value& marker = markers.get(i, Json::Value::null);
			layer->marker_.push_back(loadMarkerJson(marker));
		}
	}
	layer->resetPropertyFrame();
	comp->layer_.push_back(layer);
}
AVLayer* Loader::loadAVLayerJson(const Json::Value &json)
{
	AVLayer *ret = new AVLayer();
	const Json::Value& properties = json.get("property", Json::Value::null);
	bool use_mask = properties.isMember("mask");
	// Propaties
	if(use_mask) {
		const Json::Value& masks = properties.get("mask", Json::Value::null);
		int mask_count = masks.size();
		for(int i = 0; i < mask_count; ++i) {
			const Json::Value& mask = masks.get(i, Json::Value::null);
			Mask *target = loadMaskJson(mask);
			{
				const Json::Value& array = mask.get("vertices", Json::Value::null);
				if(array.isArray()) {
					target->setVertexSize(array.size());
					int array_count = array.size();
					for(int j = 0; j < array_count; ++j) {
						MaskVertexProp *prop = new MaskVertexProp();
						prop->setTarget(target);
						const Json::Value& keys = array[j];
						for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
							const string& name = key.key().asString();
							int key_frame = ofToInt(name);
							ofVec2f value = ofVec2f(keys[name][0].asFloat(),keys[name][1].asFloat());
							prop->addKey(key_frame, MaskShapeVertexArg(j, value));
						}
						ret->property_.push_back(prop);
					}
				}
			}
			{
				const Json::Value& array = mask.get("inTangents", Json::Value::null);
				if(array.isArray()) {
					target->setInTangentSize(array.size());
					int array_count = array.size();
					for(int j = 0; j < array_count; ++j) {
						MaskInTangentProp *prop = new MaskInTangentProp();
						prop->setTarget(target);
						const Json::Value& keys = array[j];
						for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
							const string& name = key.key().asString();
							int key_frame = ofToInt(name);
							ofVec2f value = ofVec2f(keys[name][0].asFloat(),keys[name][1].asFloat());
							prop->addKey(key_frame, MaskShapeInTangentArg(j, value));
						}
						ret->property_.push_back(prop);
					}
				}
			}
			{
				const Json::Value& array = mask.get("outTangents", Json::Value::null);
				if(array.isArray()) {
					target->setOutTangentSize(array.size());
					int array_count = array.size();
					for(int j = 0; j < array_count; ++j) {
						MaskOutTangentProp *prop = new MaskOutTangentProp();
						prop->setTarget(target);
						const Json::Value& keys = array[j];
						for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
							const string& name = key.key().asString();
							int key_frame = ofToInt(name);
							ofVec2f value = ofVec2f(keys[name][0].asFloat(),keys[name][1].asFloat());
							prop->addKey(key_frame, MaskShapeOutTangentArg(j, value));
						}
						ret->property_.push_back(prop);
					}
				}
			}
			ret->mask_.push_back(target);
		}
	}
	ret->allocate(json.get("width", 1).asFloat(), json.get("height", 1).asFloat(), use_mask);
	
	// Type specified
	// Solid
	const Json::Value& solid = json.get("solid", Json::Value::null);
	if(!solid.isNull()) {
		SolidLayerHelper *helper = new SolidLayerHelper();
		helper->setTarget(ret);
		helper->color_.set(solid[0].asFloat(), solid[1].asFloat(), solid[2].asFloat());
		ret->helper_.push_back(helper);
	}
	// Still(Image)
	const Json::Value& still = json.get("still", Json::Value::null);
	if(!still.isNull()) {
		StillLayerHelper *helper = new StillLayerHelper();
		helper->setTarget(ret);
		helper->loadImage(still.asString());
		ret->helper_.push_back(helper);
	}
	// Composition
	const Json::Value& composition = json.get("composition", Json::Value::null);
	if(!composition.isNull()) {
		CompositionLayerHelper *helper = new CompositionLayerHelper();
		helper->setTarget(ret);
		helper->composition_ = loadCompositionJson(composition);
		ret->helper_.push_back(helper);
	}
	return ret;
}
	
CameraLayer* Loader::loadCameraLayerJson(const Json::Value &json, Composition *comp)
{
	CameraLayer *ret = new CameraLayer();
	const Json::Value& properties = json.get("property", Json::Value::null);
	// Propaties
	if(properties.isMember("Zoom")) {
		CameraLayerFovProp *prop = new CameraLayerFovProp();
		prop->setTarget(ret);
		ret->property_.push_back(prop);
		const Json::Value& keys = properties.get("Zoom", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			float value = 2 * atan(comp->getHeight() / (2 * keys[name].asFloat())) * (180 / PI);
			prop->addKey(key_frame, value);
		}
	}
	return ret;
}
	
Marker* Loader::loadMarkerJson(const Json::Value &json)
{
	Marker *ret = new Marker();
	ret->name_ = json.get("name", "noname").asString();
	ret->from_ = json.get("from", 0).asInt();
	ret->to_ = json.get("from", 0).asInt();
	return ret;
}

Mask* Loader::loadMaskJson(const Json::Value &json)
{
	Mask *ret = new Mask();
	ret->name_ = json.get("name", "noname").asString();
	ret->is_inverted_ = json.get("inverted", false).asBool();
	const string& blend_mode = json.get("mode", "add").asString();
	if(blend_mode == "add")			{ ret->blend_mode_ = OF_BLENDMODE_ADD; }
	if(blend_mode == "subtract")	{ ret->blend_mode_ = OF_BLENDMODE_SUBTRACT; }
	return ret;
}
	
}

/* EOF */