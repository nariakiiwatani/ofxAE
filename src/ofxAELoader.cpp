#include "ofxAELoader.h"

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
#include "ofxAEImageLayer.h"
#include "ofxAESequenceLayer.h"
#include "ofxAEShapeLayer.h"

OFX_AE_NAMESPACE_BEGIN

Loader::Loader(const string& base_path)
{
	setBasePath(base_path);
}
void Loader::setBasePath(const string &base_path)
{
	base_path_ = ofFilePath::addTrailingSlash(base_path);
}
void Loader::loadComposition(Composition& comp, const string& filepath)
{
	string ext = ofFilePath::getFileExt(filepath);
	if(ext == "json") {
		ofxJSONElement json;
		if(file_cache_.find(base_path_+filepath) == file_cache_.end()) {
			file_cache_.insert(pair<string,string>(base_path_+filepath,ofBufferFromFile(base_path_+filepath).getText()));
		}
		if(json.parse(file_cache_[base_path_+filepath])) {
			setupCompositionJson(comp, json);
		}
		else {
			ofLog(OF_LOG_WARNING, "can't open json file");
		}
	}
}
void Loader::setupCompositionJson(Composition& comp, const Json::Value& json)
{
	// Basics
	comp.name_ = json.get("name", "noname").asString();
	comp.allocate(json.get("width", 1).asFloat(), json.get("height", 1).asFloat());
	comp.setLength(json.get("length", 0).asInt());
	// Layers
	const Json::Value& layers = json.get("layer", Json::Value::null);
	if(layers.isArray()) {
		map<Layer*, int> all;
		map<Layer*, int> children;
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
			else if(type_name == "sequence") {
				SequenceLayer *ll = new SequenceLayer();
				setupSequenceLayerJson(*ll, layer);
				comp.av_.push_back(ll);
				l = ll;
			}
			else if(type_name == "image") {
				ImageLayer *ll = new ImageLayer();
				setupImageLayerJson(*ll, layer);
				comp.av_.push_back(ll);
				l = ll;
			}
			else if(type_name == "camera") {
				CameraLayer *ll = new CameraLayer();
				setupCameraLayerJson(*ll, layer, comp);
				comp.camera_.push_back(ll);
				l = ll;
			}
			else if(type_name == "shape") {
				ShapeLayer *ll = new ShapeLayer();
				setupShapeLayerJson(*ll, layer);
				comp.av_.push_back(ll);
				l = ll;
			}
			if(!l) {
				continue;
			}
			all.insert(pair<Layer*,int>(l, layer.get("index", 0).asInt()));
			if(layer.isMember("parent")) {
				children.insert(pair<Layer*,int>(l, layer.get("parent", 0).asInt()));
			}
		}
		// search parent
		for(map<Layer*, int>::iterator child = children.begin(); child != children.end(); ++child) {
			for(map<Layer*, int>::iterator one = all.begin(); one != all.end(); ++one) {
				if((*child).second == (*one).second) {
					(*child).first->setParent((*one).first);
				}
			}
		}
	}
	// Markers
	const Json::Value& markers = json.get("marker", Json::Value::null);
	if(markers.isArray()) {
		int marker_count = markers.size();
		for(int i = 0; i < marker_count; ++i) {
			const Json::Value& marker = markers.get(i, Json::Value::null);
			Marker *m = new Marker();
			setupMarkerJson(*m, marker);
			comp.marker_.push_back(m);
		}
	}
	comp.setFrame(0);
}

void Loader::setupLayerJson(Layer& layer, const Json::Value& json)
{
	layer.name_ = json.get("name", "noname").asString();
	layer.start_frame_ = json.get("startFrame", 0).asFloat();
	const Json::Value& properties = json.get("property", Json::Value::null);
	setupPropertyKeysJson(layer.active_, properties.get("active", Json::Value::null));
	setupPropertyKeysJson(layer.opacity_, properties.get("Opacity", Json::Value::null), 0.01f);
	setupPropertyKeysJson(layer.transform_, properties);
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
}
void Loader::setupAVLayerJson(AVLayer& layer, const Json::Value& json)
{
	const Json::Value& properties = json.get("property", Json::Value::null);
	bool use_mask = properties.isMember("mask");
	layer.allocate(json.get("width", 1).asFloat(), json.get("height", 1).asFloat(), use_mask);
	layer.is_3d_ = json.get("is3d", false).asBool();
	layer.is_collapse_ = json.get("isCollapse", false).asBool();
	setupLayerJson(layer, json);
	if(use_mask) {
		const Json::Value& masks = properties.get("mask", Json::Value::null);
		int mask_count = masks.size();
		for(int i = 0; i < mask_count; ++i) {
			const Json::Value& mask = masks.get(i, Json::Value::null);
			Mask *target = new Mask();
			setupMaskJson(*target, mask);
			layer.addMask(target);
		}
	}
}

void Loader::setupCameraLayerJson(CameraLayer& layer, const Json::Value& json, Composition& comp)
{
	const Json::Value& properties = json.get("property", Json::Value::null);
	setupLayerJson(layer, json);
	setupPropertyKeysJson(layer.look_at_, properties.get("LookAt", Json::Value::null), ofVec3f(1,1,-1));
	if(properties.isMember("Zoom")) {
		const Json::Value& keys = properties.get("Zoom", Json::Value::null);
		for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
			const string& name = key.key().asString();
			int key_frame = ofToInt(name);
			float value = 2 * atan(comp.getHeight() / (2 * keys[name].asFloat())) * (180 / PI);
			layer.fov_.addKey(key_frame, value);
		}
	}
}

void Loader::setupCompositionLayerJson(CompositionLayer& layer, const Json::Value& json)
{
	setupAVLayerJson(layer, json);
	const Json::Value& source_dir = json.get("sourceDirectory", Json::Value::null);
	const Json::Value& source = json.get("source", Json::Value::null);
	if(!source.isNull()) {
		loadComposition(layer.composition_, (source_dir.isNull()?"":source_dir.asString())+source.asString());
//		setupCompositionJson(layer.composition_, source);
	}
}
void Loader::setupSolidLayerJson(SolidLayer& layer, const Json::Value& json)
{
	setupAVLayerJson(layer, json);
	const Json::Value& color = json.get("color", Json::Value::null);
	if(!color.isNull()) {
		layer.setColor(ofFloatColor(color[0].asFloat(), color[1].asFloat(), color[2].asFloat()));
	}
}
void Loader::setupImageLayerJson(ImageLayer& layer, const Json::Value& json)
{
	setupAVLayerJson(layer, json);
	const Json::Value& source_dir = json.get("sourceDirectory", Json::Value::null);
	const Json::Value& source = json.get("source", Json::Value::null);
	if(!source.isNull()) {
		layer.loadImage(base_path_+(source_dir.isNull()?"":source_dir.asString())+source.asString());
	}
}
void Loader::setupSequenceLayerJson(SequenceLayer& layer, const Json::Value& json)
{
	setupAVLayerJson(layer, json);
	const Json::Value& source_dir = json.get("sourceDirectory", Json::Value::null);
	const Json::Value& source = json.get("source", Json::Value::null);
	if(!source.isNull()) {
		layer.setSequenceString(base_path_+(source_dir.isNull()?"":source_dir.asString())+source.asString());
	}
}
void Loader::setupShapeLayerJson(ShapeLayer& layer, const Json::Value& json)
{
	setupAVLayerJson(layer, json);
	const Json::Value& properties = json.get("property", Json::Value::null);
	if(properties.isMember("contents")) {
		const Json::Value& contents = properties.get("contents", Json::Value::null);
		setupShapeContentsJson(layer, contents);
	}
}
void Loader::setupShapeContentsJson(ShapeLayer& layer, const Json::Value& contents, ShapeContentGroup *parent)
{
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
				parent->addContent(target);
			}
			else {
				layer.addContent(target);
			}
		}
		// ellipse
		else if(type == "ellipse") {
			ShapeContentEllipse *target = new ShapeContentEllipse();
			setupPropertyKeysJson(target->pos_, content.get("Position", Json::Value::null));
			setupPropertyKeysJson(target->size_, content.get("Size", Json::Value::null));
			if(parent) {
				parent->addContent(target);
			}
			else {
				layer.addContent(target);
			}
		}
		// rect
		else if(type == "rect") {
			ShapeContentRect *target = new ShapeContentRect();
			setupPropertyKeysJson(target->pos_, content.get("Position", Json::Value::null));
			setupPropertyKeysJson(target->size_, content.get("Size", Json::Value::null));
			setupPropertyKeysJson(target->roundness_, content.get("Roundness", Json::Value::null));
			if(parent) {
				parent->addContent(target);
			}
			else {
				layer.addContent(target);
			}
		}
		// path
		else if(type == "path") {
			ShapeContentPath *target = new ShapeContentPath();
			target->setSize(layer.getSize());
			setupPropertyKeysJson(target->getPath(), content.get("path", Json::Value::null));
			if(parent) {
				parent->addContent(target);
			}
			else {
				layer.addContent(target);
			}
		}
		// stroke
		else if(type == "stroke") {
			ShapeContentStroke *target = new ShapeContentStroke();
			setupPropertyKeysJson(target->color_, content.get("Color", Json::Value::null));
			setupPropertyKeysJson(target->opacity_, content.get("Opacity", Json::Value::null), 0.01f);
			setupPropertyKeysJson(target->width_, content.get("StrokeWidth", Json::Value::null));
			if(parent) {
				parent->addContent(target);
			}
			else {
				layer.addContent(target);
			}
		}
		// fill
		else if(type == "fill") {
			ShapeContentFill *target = new ShapeContentFill();
			setupPropertyKeysJson(target->color_, content.get("Color", Json::Value::null));
			setupPropertyKeysJson(target->opacity_, content.get("Opacity", Json::Value::null), 0.01f);
			if(parent) {
				parent->addContent(target);
			}
			else {
				layer.addContent(target);
			}
		}
		// transform
		else if(type == "transform") {
			setupPropertyKeysJson(parent->transform_.translation_, content.get("Position", Json::Value::null));
			setupPropertyKeysJson(parent->transform_.anchor_point_, content.get("AnchorPoint", Json::Value::null));
			setupPropertyKeysJson(parent->transform_.scale_, content.get("Scale", Json::Value::null), ofVec3f(0.01f,0.01f,0), ofVec3f(0,0,1));
			setupPropertyKeysJson(parent->rotation_z_, content.get("Rotation", Json::Value::null));
			setupPropertyKeysJson(parent->opacity_, content.get("Opacity", Json::Value::null), 0.01f);
			setupPropertyKeysJson(parent->skew_, content.get("Skew", Json::Value::null));
			setupPropertyKeysJson(parent->skew_axis_, content.get("SkewAxis", Json::Value::null));
		}
	}
}
void Loader::setupMarkerJson(Marker& marker, const Json::Value& json)
{
	marker.setupByComment(json.get("comment", "").asString());
	marker.from_ = json.get("from", 0).asInt();
	marker.length_ = json.get("length", 0).asInt();
}

void Loader::setupMaskJson(Mask& mask, const Json::Value& json)
{
	mask.name_ = json.get("name", "noname").asString();
	mask.setInverted(json.get("inverted", false).asBool());
	const string& blend_mode = json.get("mode", "none").asString();
	if(blend_mode == "none")		{ mask.blend_mode_ = OF_BLENDMODE_DISABLED; }
	if(blend_mode == "add")			{ mask.blend_mode_ = OF_BLENDMODE_ADD; }
	if(blend_mode == "subtract")	{ mask.blend_mode_ = OF_BLENDMODE_SUBTRACT; }
	setupPropertyKeysJson(mask.path_, json);
	setupPropertyKeysJson(mask.opacity_, json.get("opacity", Json::Value::null), 0.01f);
}

void Loader::setupPropertyKeysJson(TransformProperty& prop, const Json::Value& json)
{
	setupPropertyKeysJson(prop.translation_, json.get("Position", Json::Value::null), ofVec3f(1,1,-1));
	setupPropertyKeysJson(prop.scale_, json.get("Scale", Json::Value::null), ofVec3f(0.01f,0.01f,0.01f));
	setupPropertyKeysJson(prop.rotation_, json.get("Rotation", Json::Value::null), ofVec3f(-1,-1,1));
	setupPropertyKeysJson(prop.anchor_point_, json.get("AnchorPoint", Json::Value::null), ofVec3f(1,1,-1));
	setupPropertyKeysJson(prop.orientation_, json.get("Orientation", Json::Value::null), ofVec3f(-1,-1,1));
}

void Loader::setupPropertyKeysJson(Property<bool>& prop, const Json::Value& json)
{
	for(Json::Value::iterator key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key().asString();
		int key_frame = ofToInt(name);
		bool value = json[name].asBool();
		prop.addKey(key_frame, value);
	}
}
void Loader::setupPropertyKeysJson(Property<float>& prop, const Json::Value& json, float scale, float offset)
{
	for(Json::Value::iterator key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key().asString();
		int key_frame = ofToInt(name);
		float value = json[name].asFloat()*scale+offset;
		prop.addKey(key_frame, value);
	}
}
void Loader::setupPropertyKeysJson(Property<ofVec2f>& prop, const Json::Value& json, const ofVec2f& scale, const ofVec2f& offset)
{
	for(Json::Value::iterator key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key().asString();
		int key_frame = ofToInt(name);
		ofVec2f value = ofVec2f(json[name][0].asFloat(), json[name][1].asFloat())*scale+offset;
		prop.addKey(key_frame, value);
	}
}
void Loader::setupPropertyKeysJson(Property<ofVec3f>& prop, const Json::Value& json, const ofVec3f& scale, const ofVec3f& offset)
{
	for(Json::Value::iterator key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key().asString();
		int key_frame = ofToInt(name);
		ofVec3f value = ofVec3f(json[name][0].asFloat(), json[name][1].asFloat(), json[name][2].asFloat())*scale+offset;
		prop.addKey(key_frame, value);
	}
}
void Loader::setupPropertyKeysJson(Property<ofFloatColor>& prop, const Json::Value& json, const ofFloatColor& scale, const ofFloatColor& offset)
{
	for(Json::Value::iterator key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key().asString();
		int key_frame = ofToInt(name);
		ofFloatColor value = ofFloatColor(json[name][0].asFloat(), json[name][1].asFloat(), json[name][2].asFloat())*scale+offset;
		prop.addKey(key_frame, value);
	}
}
void Loader::setupPropertyKeysJson(PathProperty& prop, const Json::Value& json)
{
	{
		const Json::Value& array = json.get("vertices", Json::Value::null);
		if(array.isArray()) {
			prop.setVertexSize(array.size());
			int array_count = array.size();
			for(int j = 0; j < array_count; ++j) {
				setupPropertyKeysJson(prop.vertices_[j], array[j]);
			}
		}
	}
	{
		const Json::Value& array = json.get("inTangents", Json::Value::null);
		if(array.isArray()) {
			prop.setInTangentSize(array.size());
			int array_count = array.size();
			for(int j = 0; j < array_count; ++j) {
				setupPropertyKeysJson(prop.in_tangents_[j], array[j]);
			}
		}
	}
	{
		const Json::Value& array = json.get("outTangents", Json::Value::null);
		if(array.isArray()) {
			prop.setOutTangentSize(array.size());
			int array_count = array.size();
			for(int j = 0; j < array_count; ++j) {
				setupPropertyKeysJson(prop.out_tangents_[j], array[j]);
			}
		}
	}
}

OFX_AE_NAMESPACE_END

/* EOF */