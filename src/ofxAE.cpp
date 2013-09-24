#include "ofxAE.h"

#include "ofxJSONElement.h"
#include "ofFileUtils.h"
#include "ofxAEComposition.h"
#include "ofxAEAVLayer.h"
#include "ofxAEMarker.h"
#include "ofxAEMask.h"
#include "ofxAEProperty.h"
#include "ofxAECameraLayer.h"
#include "ofxAECompositionLayer.h"
#include "ofxAESolidLayer.h"
#include "ofxAEStillLayer.h"
#include "ofxAEShapeLayer.h"

namespace ofxAE {
void Loader::loadComposition(Composition& comp, const string& filepath)
{
	string ext = ofFilePath::getFileExt(filepath);
	if(ext == "json") {
		ofxJSONElement json;
		if(json.open(filepath)) {
			setupCompositionJson(comp, json);
		}
	}
}
void Loader::setupCompositionJson(Composition& comp, const Json::Value& json)
{
	// Basics
	comp.name_ = json.get("name", "noname").asString();
	comp.allocate(json.get("width", 1).asFloat(), json.get("height", 1).asFloat());
	// Layers
	const Json::Value& layers = json.get("layer", Json::Value::null);
	if(layers.isArray()) {
		vector<Layer*> all;
		map<string, Layer*> children;
		int layer_count = layers.size();
		for(int i = layer_count; i--;) {	// reverse iterate for draw priority
			const Json::Value& layer = layers.get(i, Json::Value::null);
			const string& type_name = layer.get("layerType", "unknown").asString();
			Layer *l = NULL;
			if(type_name == "composition") {
				CompositionLayer *ll = new CompositionLayer();
				setupCompositionLayerJson(*ll, layer);
				comp.av_.push_back(ll);
				l = ll;
			}
			else if(type_name == "solid") {
				SolidLayer *ll = new SolidLayer();
				setupSolidLayerJson(*ll, layer);
				comp.av_.push_back(ll);
				l = ll;
			}
			else if(type_name == "still") {
				StillLayer *ll = new StillLayer();
				setupStillLayerJson(*ll, layer);
				comp.av_.push_back(ll);
				l = ll;
			}
			else if(type_name == "camera") {
				CameraLayer *camera = new CameraLayer();
				setupCameraLayerJson(*camera, layer, comp);
				comp.camera_.push_back(camera);
				l = camera;
			}
			else if(type_name == "shape") {
				ShapeLayer *shape = new ShapeLayer();
				setupShapeLayerJson(*shape, layer);
				comp.av_.push_back(shape);
				l = shape;
			}
			if(!l) {
				continue;
			}
			all.push_back(l);
			if(layer.isMember("parent")) {
				children.insert(pair<string,Layer*>(layer.get("parent", Json::Value::null).asString(), l));
			}
		}
		// search parent
		for(map<string,Layer*>::iterator child = children.begin(); child != children.end(); ++child) {
			for(vector<Layer*>::iterator one = all.begin(); one != all.end(); ++one) {
				if((*child).first == (*one)->name_) {
					(*child).second->setParent(*one);
					break;
				}
			}
		}
	}
}

void Loader::setupLayerJson(Layer& layer, const Json::Value& json)
{
	layer.name_ = json.get("name", "noname").asString();
	layer.start_frame_ = json.get("startFrame", 0).asFloat();
	const Json::Value& properties = json.get("property", Json::Value::null);
	if(properties.isMember("active")) {
		LayerActiveProp *prop = new LayerActiveProp();
		const Json::Value& keys = properties.get("active", Json::Value::null);
		setupPropertyKeysJson(*prop, keys);
		prop->setTarget(&layer);
		layer.property_.push_back(prop);
	}
	if(properties.isMember("Position")) {
		LayerPositionProp *prop = new LayerPositionProp();
		const Json::Value& keys = properties.get("Position", Json::Value::null);
		setupPropertyKeysJson(*prop, keys, ofVec3f(1,1,-1));
		prop->setTarget(&layer);
		layer.property_.push_back(prop);
	}
	if(properties.isMember("Scale")) {
		LayerScaleProp *prop = new LayerScaleProp();
		const Json::Value& keys = properties.get("Scale", Json::Value::null);
		setupPropertyKeysJson(*prop, keys, ofVec3f(0.01f,0.01f,0.01f));
		prop->setTarget(&layer);
		layer.property_.push_back(prop);
	}
	if(properties.isMember("RotationX")) {
		LayerRotationXProp *prop = new LayerRotationXProp();
		const Json::Value& keys = properties.get("RotationX", Json::Value::null);
		setupPropertyKeysJson(*prop, keys, -1);
		prop->setTarget(&layer);
		layer.property_.push_back(prop);
	}
	if(properties.isMember("RotationY")) {
		LayerRotationYProp *prop = new LayerRotationYProp();
		const Json::Value& keys = properties.get("RotationY", Json::Value::null);
		setupPropertyKeysJson(*prop, keys, -1);
		prop->setTarget(&layer);
		layer.property_.push_back(prop);
	}
	if(properties.isMember("RotationZ")) {
		LayerRotationZProp *prop = new LayerRotationZProp();
		const Json::Value& keys = properties.get("RotationZ", Json::Value::null);
		setupPropertyKeysJson(*prop, keys);
		prop->setTarget(&layer);
		layer.property_.push_back(prop);
	}
	if(properties.isMember("AnchorPoint")) {
		LayerAnchorPointProp *prop = new LayerAnchorPointProp();
		const Json::Value& keys = properties.get("AnchorPoint", Json::Value::null);
		setupPropertyKeysJson(*prop, keys, ofVec3f(1,1,-1));
		prop->setTarget(&layer);
		layer.property_.push_back(prop);
	}
	if(properties.isMember("Opacity")) {
		LayerOpacityProp *prop = new LayerOpacityProp();
		const Json::Value& keys = properties.get("Opacity", Json::Value::null);
		setupPropertyKeysJson(*prop, keys, 0.01f);
		prop->setTarget(&layer);
		layer.property_.push_back(prop);
	}
	if(properties.isMember("Orientation")) {
		LayerOrientationProp *prop = new LayerOrientationProp();
		const Json::Value& keys = properties.get("Orientation", Json::Value::null);
		setupPropertyKeysJson(*prop, keys, ofVec3f(-1,-1,1));
		prop->setTarget(&layer);
		layer.property_.push_back(prop);
	}
	
	// Markers
	const Json::Value& markers = json.get("marker", Json::Value::null);
	if(markers.isArray()) {
		int marker_count = markers.size();
		for(int i = 0; i < marker_count; ++i) {
			const Json::Value& marker = markers.get(i, Json::Value::null);
			Marker *m = new Marker();
			setupMarkerJson(*m, marker);
			layer.marker_.push_back(m);
		}
	}
	layer.resetPropertyFrame();
}
void Loader::setupAVLayerJson(AVLayer& layer, const Json::Value& json)
{
	const Json::Value& properties = json.get("property", Json::Value::null);
	bool use_mask = properties.isMember("mask");
	// Propaties
	if(use_mask) {
		const Json::Value& masks = properties.get("mask", Json::Value::null);
		int mask_count = masks.size();
		for(int i = 0; i < mask_count; ++i) {
			const Json::Value& mask = masks.get(i, Json::Value::null);
			Mask *target = new Mask();
			setupMaskJson(*target, mask);
			setupPathJson(target->getPath(), mask, layer);
			layer.mask_.push_back(target);
		}
	}
	layer.allocate(json.get("width", 1).asFloat(), json.get("height", 1).asFloat(), use_mask);
	layer.is_3d_ = json.get("is3d", false).asBool(); 
	setupLayerJson(layer, json);
}

void Loader::setupPathJson(Path& path, const Json::Value& json, Layer& layer)
{
	{
		const Json::Value& array = json.get("vertices", Json::Value::null);
		if(array.isArray()) {
			path.setVertexSize(array.size());
			int array_count = array.size();
			for(int j = 0; j < array_count; ++j) {
				PathVertexProp *prop = new PathVertexProp();
				const Json::Value& keys = array[j];
				for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
					const string& name = key.key().asString();
					int key_frame = ofToInt(name);
					ofVec2f value = ofVec2f(keys[name][0].asFloat(),keys[name][1].asFloat());
					prop->addKey(key_frame, PathShapeVertexArg(j, value));
				}
				prop->setTarget(&path);
				layer.property_.push_back(prop);
			}
		}
	}
	{
		const Json::Value& array = json.get("inTangents", Json::Value::null);
		if(array.isArray()) {
			path.setInTangentSize(array.size());
			int array_count = array.size();
			for(int j = 0; j < array_count; ++j) {
				PathInTangentProp *prop = new PathInTangentProp();
				const Json::Value& keys = array[j];
				for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
					const string& name = key.key().asString();
					int key_frame = ofToInt(name);
					ofVec2f value = ofVec2f(keys[name][0].asFloat(),keys[name][1].asFloat());
					prop->addKey(key_frame, PathShapeInTangentArg(j, value));
				}
				prop->setTarget(&path);
				layer.property_.push_back(prop);
			}
		}
	}
	{
		const Json::Value& array = json.get("outTangents", Json::Value::null);
		if(array.isArray()) {
			path.setOutTangentSize(array.size());
			int array_count = array.size();
			for(int j = 0; j < array_count; ++j) {
				PathOutTangentProp *prop = new PathOutTangentProp();
				const Json::Value& keys = array[j];
				for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
					const string& name = key.key().asString();
					int key_frame = ofToInt(name);
					ofVec2f value = ofVec2f(keys[name][0].asFloat(),keys[name][1].asFloat());
					prop->addKey(key_frame, PathShapeOutTangentArg(j, value));
				}
				prop->setTarget(&path);
				layer.property_.push_back(prop);
			}
		}
	}
}

void Loader::setupCameraLayerJson(CameraLayer& layer, const Json::Value& json, Composition& comp)
{
	const Json::Value& properties = json.get("property", Json::Value::null);
	// Propaties
	if(properties.isMember("Zoom")) {
		CameraLayerFovProp *prop = new CameraLayerFovProp();
		prop->setTarget(&layer);
		const Json::Value& keys = properties.get("Zoom", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			float value = 2 * atan(comp.getHeight() / (2 * keys[name].asFloat())) * (180 / PI);
			prop->addKey(key_frame, value);
		}
		layer.property_.push_back(prop);
	}
	setupLayerJson(layer, json);
}

void Loader::setupCompositionLayerJson(CompositionLayer& layer, const Json::Value& json)
{
	const Json::Value& composition = json.get("composition", Json::Value::null);
	if(!composition.isNull()) {
		setupCompositionJson(layer.composition_, composition);
	}
	setupAVLayerJson(layer, json);
}
void Loader::setupSolidLayerJson(SolidLayer& layer, const Json::Value& json)
{
	const Json::Value& solid = json.get("solid", Json::Value::null);
	if(!solid.isNull()) {
		layer.setColor(ofFloatColor(solid[0].asFloat(), solid[1].asFloat(), solid[2].asFloat()));
	}
	setupAVLayerJson(layer, json);
}
void Loader::setupStillLayerJson(StillLayer& layer, const Json::Value& json)
{
	const Json::Value& still = json.get("still", Json::Value::null);
	if(!still.isNull()) {
		layer.loadImage(still.asString());
	}
	setupAVLayerJson(layer, json);
}
void Loader::setupShapeLayerJson(ShapeLayer& layer, const Json::Value& json)
{
	const Json::Value& properties = json.get("property", Json::Value::null);
	if(properties.isMember("contents")) {
		const Json::Value& contents = properties.get("contents", Json::Value::null);
		setupShapeContentsJson(layer, contents);
	}
	setupAVLayerJson(layer, json);
}
void Loader::setupShapeContentsJson(ShapeLayer& layer, const Json::Value& contents, ShapeContentGroup *parent)
{
	if(contents.isArray()) {
		int content_count = contents.size();
		for(int i = 0; i < content_count; ++i) {
			const Json::Value& content = contents.get(i, Json::Value::null);
			const string& type = content.get("type", "").asString();
			// group
			if(type == "group") {
				ShapeContentGroup *target = new ShapeContentGroup();
				if(content.isMember("contents")) {
					setupShapeContentsJson(layer, content.get("contents", Json::Value::null), target);
				}
				if(parent) {
					parent->content_.push_back(target);
				}
				else {
					layer.content_.push_back(target);
				}
			}
			// ellipse
			else if(type == "ellipse") {
				ShapeContentEllipse *target = new ShapeContentEllipse();
				{
					ShapeEllipsePositionProp *prop = new ShapeEllipsePositionProp();
					const Json::Value& keys = content.get("Position", Json::Value::null);
					setupPropertyKeysJson(*prop, keys);
					prop->setTarget(target);
					layer.property_.push_back(prop);
				}
				{
					ShapeEllipseSizeProp *prop = new ShapeEllipseSizeProp();
					const Json::Value& keys = content.get("Size", Json::Value::null);
					setupPropertyKeysJson(*prop, keys);
					prop->setTarget(target);
					layer.property_.push_back(prop);
				}
				if(parent) {
					parent->content_.push_back(target);
				}
				else {
					layer.content_.push_back(target);
				}
			}
			// rect
			else if(type == "rect") {
				ShapeContentRect *target = new ShapeContentRect();
				{
					ShapeRectPositionProp *prop = new ShapeRectPositionProp();
					const Json::Value& keys = content.get("Position", Json::Value::null);
					setupPropertyKeysJson(*prop, keys);
					prop->setTarget(target);
					layer.property_.push_back(prop);
				}
				{
					ShapeRectSizeProp *prop = new ShapeRectSizeProp();
					const Json::Value& keys = content.get("Size", Json::Value::null);
					setupPropertyKeysJson(*prop, keys);
					prop->setTarget(target);
					layer.property_.push_back(prop);
				}
				{
					ShapeRectRoundnessProp *prop = new ShapeRectRoundnessProp();
					const Json::Value& keys = content.get("Roundness", Json::Value::null);
					setupPropertyKeysJson(*prop, keys);
					prop->setTarget(target);
					layer.property_.push_back(prop);
				}
				if(parent) {
					parent->content_.push_back(target);
				}
				else {
					layer.content_.push_back(target);
				}
			}
			// path
			else if(type == "path") {
				ShapeContentPath *target = new ShapeContentPath();
				const Json::Value& path = content.get("path", Json::Value::null);
				setupPathJson(target->getPath(), path, layer);
				if(parent) {
					parent->content_.push_back(target);
				}
				else {
					layer.content_.push_back(target);
				}
			}
			// stroke
			else if(type == "stroke") {
				ShapeContentStroke *target = new ShapeContentStroke();
				{
					ShapeStrokeColorProp *prop = new ShapeStrokeColorProp();
					const Json::Value& keys = content.get("Color", Json::Value::null);
					setupPropertyKeysJson(*prop, keys);
					prop->setTarget(target);
					layer.property_.push_back(prop);
				}
				{
					ShapeStrokeOpacityProp *prop = new ShapeStrokeOpacityProp();
					const Json::Value& keys = content.get("Opacity", Json::Value::null);
					setupPropertyKeysJson(*prop, keys, 0.01f);
					prop->setTarget(target);
					layer.property_.push_back(prop);
				}
				{
					ShapeStrokeWidthProp *prop = new ShapeStrokeWidthProp();
					setupPropertyKeysJson(*prop, content.get("StrokeWidth", Json::Value::null));
					prop->setTarget(target);
					layer.property_.push_back(prop);
				}
				if(parent) {
					parent->content_.push_back(target);
				}
				else {
					layer.content_.push_back(target);
				}
			}
			// fill
			else if(type == "fill") {
				ShapeContentFill *target = new ShapeContentFill();
				{
					ShapeFillColorProp *prop = new ShapeFillColorProp();
					const Json::Value& keys = content.get("Color", Json::Value::null);
					setupPropertyKeysJson(*prop, keys);
					prop->setTarget(target);
					layer.property_.push_back(prop);
				}
				{
					ShapeFillOpacityProp *prop = new ShapeFillOpacityProp();
					const Json::Value& keys = content.get("Opacity", Json::Value::null);
					setupPropertyKeysJson(*prop, keys);
					prop->setTarget(target);
					layer.property_.push_back(prop);
				}
				if(parent) {
					parent->content_.push_back(target);
				}
				else {
					layer.content_.push_back(target);
				}
			}
			// transform
			else if(type == "transform") {
				{
					ShapeGroupPositionProp *prop = new ShapeGroupPositionProp();
					setupPropertyKeysJson(*prop, content.get("Position", Json::Value::null));
					prop->setTarget(parent);
					layer.property_.push_back(prop);
				}
				{
					ShapeGroupAnchorPointProp *prop = new ShapeGroupAnchorPointProp();
					setupPropertyKeysJson(*prop, content.get("AnchorPoint", Json::Value::null));
					prop->setTarget(parent);
					layer.property_.push_back(prop);
				}
				{
					ShapeGroupScaleProp *prop = new ShapeGroupScaleProp();
					setupPropertyKeysJson(*prop, content.get("Scale", Json::Value::null), ofVec2f(0.01f,0.01f));
					prop->setTarget(parent);
					layer.property_.push_back(prop);
				}
				{
					ShapeGroupRotationProp *prop = new ShapeGroupRotationProp();
					setupPropertyKeysJson(*prop, content.get("Rotation", Json::Value::null));
					prop->setTarget(parent);
					layer.property_.push_back(prop);
				}
				{
					ShapeGroupOpacityProp *prop = new ShapeGroupOpacityProp();
					setupPropertyKeysJson(*prop, content.get("Opacity", Json::Value::null), 0.01f);
					prop->setTarget(parent);
					layer.property_.push_back(prop);
				}
				{
					ShapeGroupSkewProp *prop = new ShapeGroupSkewProp();
					setupPropertyKeysJson(*prop, content.get("Skew", Json::Value::null));
					prop->setTarget(parent);
					layer.property_.push_back(prop);
				}
				{
					ShapeGroupSkewAxisProp *prop = new ShapeGroupSkewAxisProp();
					setupPropertyKeysJson(*prop, content.get("SkewAxis", Json::Value::null));
					prop->setTarget(parent);
					layer.property_.push_back(prop);
				}
			}
		}
	}
}
void Loader::setupMarkerJson(Marker& marker, const Json::Value& json)
{
	marker.name_ = json.get("name", "noname").asString();
	marker.from_ = json.get("from", 0).asInt();
	marker.to_ = json.get("to", 0).asInt();
}

void Loader::setupMaskJson(Mask& mask, const Json::Value& json)
{
	mask.name_ = json.get("name", "noname").asString();
	mask.is_inverted_ = json.get("inverted", false).asBool();
	const string& blend_mode = json.get("mode", "add").asString();
	if(blend_mode == "add")			{ mask.blend_mode_ = OF_BLENDMODE_ADD; }
	if(blend_mode == "subtract")	{ mask.blend_mode_ = OF_BLENDMODE_SUBTRACT; }
}

template<class Target>
void Loader::setupPropertyKeysJson(Property_<Target, bool>& prop, const Json::Value& json)
{
	for(Json::Value::iterator key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key().asString();
		int key_frame = ofToInt(name);
		bool value = json[name].asBool();
		prop.addKey(key_frame, value);
	}
}
template<class Target>
void Loader::setupPropertyKeysJson(Property_<Target, float>& prop, const Json::Value& json, float scale, float offset)
{
	for(Json::Value::iterator key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key().asString();
		int key_frame = ofToInt(name);
		float value = json[name].asFloat()*scale+offset;
		prop.addKey(key_frame, value);
	}
}
template<class Target>
void Loader::setupPropertyKeysJson(Property_<Target, ofVec2f>& prop, const Json::Value& json, const ofVec2f& scale, const ofVec2f& offset)
{
	for(Json::Value::iterator key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key().asString();
		int key_frame = ofToInt(name);
		ofVec2f value = ofVec2f(json[name][0].asFloat(), json[name][1].asFloat())*scale+offset;
		prop.addKey(key_frame, value);
	}
}
template<class Target>
void Loader::setupPropertyKeysJson(Property_<Target, ofVec3f>& prop, const Json::Value& json, const ofVec3f& scale, const ofVec3f& offset)
{
	for(Json::Value::iterator key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key().asString();
		int key_frame = ofToInt(name);
		ofVec3f value = ofVec3f(json[name][0].asFloat(), json[name][1].asFloat(), json[name][2].asFloat())*scale+offset;
		prop.addKey(key_frame, value);
	}
}

}

/* EOF */