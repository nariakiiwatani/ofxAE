#include "ofxAELoader.h"

#include "ofxJSONElement.h"
#include "ofFileUtils.h"
#include "ofxAEComposition.h"
#include "ofxAEAVLayer.h"
#include "ofxAEMarker.h"
#include "ofxAEMask.h"
#include "ofxAEProperty.h"
#include "ofxAECameraLayer.h"
#include "ofxAECompositionCap.h"
#include "ofxAEPlaneCap.h"
#include "ofxAEImageCap.h"
#include "ofxAESequenceCap.h"
#include "ofxAEMovieCap.h"
#include "ofxAEShapeCap.h"

OFX_AE_NAMESPACE_BEGIN

map<string,string> Loader::file_cache_;

Loader::Loader(const string& base_path)
{
	setBasePath(base_path);
}
Loader::~Loader()
{
	while(!allocated_.property.empty()) {
		delete allocated_.property.front();
		allocated_.property.erase(allocated_.property.begin());
	}
	while(!allocated_.cap.empty()) {
		delete allocated_.cap.front();
		allocated_.cap.erase(allocated_.cap.begin());
	}
	while(!allocated_.marker.empty()) {
		delete allocated_.marker.front();
		allocated_.marker.erase(allocated_.marker.begin());
	}
	while(!allocated_.layer.empty()) {
		delete allocated_.layer.front();
		allocated_.layer.erase(allocated_.layer.begin());
	}
	while(!allocated_.comp.empty()) {
		delete allocated_.comp.front();
		allocated_.comp.erase(allocated_.comp.begin());
	}
}
void Loader::setBasePath(const string &base_path)
{
	base_path_ = ofFilePath::addTrailingSlash(base_path);
}
Composition* Loader::loadComposition(const string& filepath)
{
	Composition *comp = new Composition();
	allocated_.comp.push_back(comp);
	string ext = ofFilePath::getFileExt(filepath);
	if(ext == "json") {
		ofxJSONElement json;
		if(file_cache_.find(base_path_+filepath) == file_cache_.end()) {
			file_cache_.insert(pair<string,string>(base_path_+filepath,ofBufferFromFile(base_path_+filepath).getText()));
		}
		if(json.parse(file_cache_[base_path_+filepath])) {
			setupCompositionJson(*comp, json);
		}
		else {
			ofLog(OF_LOG_WARNING, "couldn't open json file: "+base_path_+filepath);
		}
	}
	return comp;
}
Composition* Loader::getComposition(int index)
{
	return allocated_.comp[index];
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
			ofLog(OF_LOG_WARNING, "couldn't open json file: "+base_path_+filepath);
		}
	}
}
void Loader::setupCompositionJson(Composition& comp, const Json::Value& json)
{
	// Basics
	comp.name_ = json.get("name", "noname").asString();
	comp.allocate(json.get("width", 1).asFloat(), json.get("height", 1).asFloat());
	comp.setLength(json.get("length", 0).asInt());
	comp.setFrameRate(json.get("frameRate", 0).asInt());
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
				AVLayer *ll = new AVLayer();
				allocated_.layer.push_back(ll);
				setupAVLayerJson(*ll, layer);
				CompositionCap *cap = new CompositionCap(ll);
				allocated_.cap.push_back(cap);
				setupCompositionJson(cap, layer);
				comp.av_.push_back(ll);
				l = ll;
			}
			else if(type_name == "solid") {
				AVLayer *ll = new AVLayer();
				allocated_.layer.push_back(ll);
				setupAVLayerJson(*ll, layer);
				PlaneCap *cap = new PlaneCap(ll);
				allocated_.cap.push_back(cap);
				setupPlaneJson(cap, layer);
				comp.av_.push_back(ll);
				l = ll;
			}
			else if(type_name == "sequence") {
				AVLayer *ll = new AVLayer();
				allocated_.layer.push_back(ll);
				setupAVLayerJson(*ll, layer);
				SequenceCap *cap = new SequenceCap(ll);
				allocated_.cap.push_back(cap);
				setupSequenceJson(cap, layer);
				comp.av_.push_back(ll);
				l = ll;
			}
			else if(type_name == "image") {
				AVLayer *ll = new AVLayer();
				allocated_.layer.push_back(ll);
				setupAVLayerJson(*ll, layer);
				ImageCap *cap = new ImageCap(ll);
				allocated_.cap.push_back(cap);
				setupImageJson(cap, layer);
				comp.av_.push_back(ll);
				l = ll;
			}
			else if(type_name == "movie") {
				AVLayer *ll = new AVLayer();
				allocated_.layer.push_back(ll);
				setupAVLayerJson(*ll, layer);
				MovieCap *cap = new MovieCap(ll);
				allocated_.cap.push_back(cap);
				setupMovieJson(cap, layer);
				cap->setComposition(&comp);
				comp.av_.push_back(ll);
				l = ll;
			}
			else if(type_name == "camera") {
				CameraLayer *ll = new CameraLayer();
				allocated_.layer.push_back(ll);
				setupCameraLayerJson(*ll, layer, comp);
				comp.camera_.push_back(ll);
				l = ll;
			}
			else if(type_name == "shape") {
				AVLayer *ll = new AVLayer();
				allocated_.layer.push_back(ll);
				setupAVLayerJson(*ll, layer);
				ShapeCap *cap = new ShapeCap(ll);
				allocated_.cap.push_back(cap);
				setupShapeJson(cap, layer);
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
			allocated_.marker.push_back(m);
			setupMarkerJson(*m, marker);
			comp.marker_.push_back(m);
		}
	}
	comp.setFrame(0);
}

void Loader::setupLayerJson(Layer& layer, const Json::Value& json)
{
	layer.name_ = json.get("name", "noname").asString();
	layer.frame_offset_ = json.get("frameOffset", 0).asInt();
	layer.frame_in_ = json.get("inFrame", 0).asInt();
	layer.frame_out_ = json.get("outFrame", 0).asInt();
	layer.setParamByComment(json.get("comment", "").asString());
	const Json::Value& properties = json.get("property", Json::Value::null);
	{
		Property<bool> *prop = new Property<bool>("active");
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, properties.get("active", Json::Value::null));
		layer.addActiveProperty(prop);
	}
	{
		Property<float> *prop = new Property<float>("opacity");
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, properties.get("Opacity", Json::Value::null), 0.01f);
		layer.addOpacityProperty(prop);
	}
	{
		TransformProperty *prop = new TransformProperty();
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, properties);
		layer.addTransformProperty(prop);
	}
	// Markers
	const Json::Value& markers = json.get("marker", Json::Value::null);
	if(markers.isArray()) {
		int marker_count = markers.size();
		for(int i = 0; i < marker_count; ++i) {
			const Json::Value& marker = markers.get(i, Json::Value::null);
			Marker *m = new Marker();
			allocated_.marker.push_back(m);
			setupMarkerJson(*m, marker);
			layer.marker_.push_back(m);
		}
	}
}
void Loader::setupAVLayerJson(AVLayer& layer, const Json::Value& json)
{
	const Json::Value& properties = json.get("property", Json::Value::null);
	layer.allocate(json.get("width", 1).asFloat(), json.get("height", 1).asFloat());
	layer.is_3d_ = json.get("is3d", false).asBool();
	layer.is_collapse_ = json.get("isCollapse", false).asBool();
	const string& blend_mode = json.get("blendingMode", "none").asString();
	if(blend_mode == "none")		{ layer.blend_mode_ = OF_BLENDMODE_ALPHA; }
	if(blend_mode == "add")			{ layer.blend_mode_ = OF_BLENDMODE_ADD; }
	if(blend_mode == "subtract")	{ layer.blend_mode_ = OF_BLENDMODE_SUBTRACT; }
	setupLayerJson(layer, json);
	const Json::Value& masks = properties.get("mask", Json::Value::null);
	if(masks.isArray()) {
		int mask_count = masks.size();
		for(int i = 0; i < mask_count; ++i) {
			const Json::Value& mask = masks.get(i, Json::Value::null);
			Mask *target = new Mask();
			allocated_.property.push_back(target);
			setupMaskJson(*target, mask, ofVec2f(layer.getWidth(),layer.getHeight()));
			layer.addMask(target);
		}
	}
}

void Loader::setupCameraLayerJson(CameraLayer& layer, const Json::Value& json, Composition& comp)
{
	const Json::Value& properties = json.get("property", Json::Value::null);
	setupLayerJson(layer, json);
	{
		Property<ofVec3f> *prop = new Property<ofVec3f>("look at");
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, properties.get("LookAt", Json::Value::null), ofVec3f(1,1,-1));
		layer.addLookAtProperty(prop);
	}
	{
		Property<float> *prop = new Property<float>("fov");
		allocated_.property.push_back(prop);
		if(properties.isMember("Zoom")) {
			const Json::Value& keys = properties.get("Zoom", Json::Value::null);
			for(Json::Value::iterator key = keys.begin(); key != keys.end(); ++key) {
				const string& name = key.key().asString();
				int key_frame = ofToInt(name);
				float value = 2 * atan(comp.getHeight() / (2 * keys[name].asFloat())) * (180 / PI);
				prop->addKey(key_frame, value);
			}
		}
		layer.addFovProperty(prop);
	}
}
void Loader::setupCompositionJson(CompositionCap *cap, const Json::Value& json)
{
	const Json::Value& source_dir = json.get("sourceDirectory", Json::Value::null);
	const Json::Value& source = json.get("source", Json::Value::null);
	if(!source.isNull()) {
		loadComposition(cap->getComposition(), (source_dir.isNull()?"":source_dir.asString())+source.asString());
	}
}
void Loader::setupPlaneJson(PlaneCap *cap, const Json::Value& json)
{
	const Json::Value& color = json.get("color", Json::Value::null);
	if(!color.isNull()) {
		cap->setColor(ofFloatColor(color[0].asFloat(), color[1].asFloat(), color[2].asFloat()));
	}
}
void Loader::setupImageJson(ImageCap *cap, const Json::Value& json)
{
	const Json::Value& source_dir = json.get("sourceDirectory", Json::Value::null);
	const Json::Value& source = json.get("source", Json::Value::null);
	if(!source.isNull()) {
		cap->loadImage(base_path_+(source_dir.isNull()?"":source_dir.asString())+source.asString());
	}
}
void Loader::setupMovieJson(MovieCap *cap, const Json::Value& json)
{
	const Json::Value& source_dir = json.get("sourceDirectory", Json::Value::null);
	const Json::Value& source = json.get("source", Json::Value::null);
	if(!source.isNull()) {
		cap->loadMovie(base_path_+(source_dir.isNull()?"":source_dir.asString())+source.asString());
	}
	bool use_audio = json.get("hasAudio", true).asBool() && json.get("audioActive", true).asBool();
	cap->setUseAudio(use_audio);
}
void Loader::setupSequenceJson(SequenceCap *cap, const Json::Value& json)
{
	const Json::Value& source_dir = json.get("sourceDirectory", Json::Value::null);
	const Json::Value& source = json.get("source", Json::Value::null);
	if(!source.isNull()) {
		cap->setSequenceString(base_path_+(source_dir.isNull()?"":source_dir.asString())+source.asString());
	}
}
void Loader::setupShapeJson(ShapeCap *cap, const Json::Value& json)
{
	const Json::Value& properties = json.get("property", Json::Value::null);
	if(properties.isMember("contents")) {
		const Json::Value& contents = properties.get("contents", Json::Value::null);
		setupShapeContentsJson(cap, contents);
	}
}
void Loader::setupShapeContentsJson(ShapeCap *cap, const Json::Value& contents, ShapeContentGroup *parent)
{
	int content_count = contents.size();
	for(int i = 0; i < content_count; ++i) {
		const Json::Value& content = contents.get(i, Json::Value::null);
		const string& type = content.get("type", "").asString();
		// group
		if(type == "group") {
			ShapeContentGroup *target = new ShapeContentGroup();
			allocated_.property.push_back(target);
			if(content.isMember("contents")) {
				setupShapeContentsJson(cap, content.get("contents", Json::Value::null), target);
			}
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap->addContent(target);
			}
		}
		// ellipse
		else if(type == "ellipse") {
			ShapeContentEllipse *target = new ShapeContentEllipse();
			allocated_.property.push_back(target);
			{
				Property<ofVec2f> *prop = new Property<ofVec2f>("position");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Position", Json::Value::null));
				target->addPositionProperty(prop);
			}
			{
				Property<ofVec2f> *prop = new Property<ofVec2f>("size");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Size", Json::Value::null));
				target->addSizeProperty(prop);
			}
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap->addContent(target);
			}
		}
		// rect
		else if(type == "rect") {
			ShapeContentRect *target = new ShapeContentRect();
			allocated_.property.push_back(target);
			{
				Property<ofVec2f> *prop = new Property<ofVec2f>("position");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Position", Json::Value::null));
				target->addPositionProperty(prop);
			}
			{
				Property<ofVec2f> *prop = new Property<ofVec2f>("size");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Size", Json::Value::null));
				target->addSizeProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("roundness");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Roundness", Json::Value::null));
				target->addRoundnessProperty(prop);
			}
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap->addContent(target);
			}
		}
		// path
		else if(type == "path") {
			ShapeContentPath *target = new ShapeContentPath();
			allocated_.property.push_back(target);
			{
				PathProperty *prop = new PathProperty();
				allocated_.property.push_back(prop);
				prop->setSize(cap->getSize());
				setupPropertyKeysJson(*prop, content.get("path", Json::Value::null));
				target->addPathProperty(prop);
			}
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap->addContent(target);
			}
		}
		// stroke
		else if(type == "stroke") {
			ShapeContentStroke *target = new ShapeContentStroke();
			allocated_.property.push_back(target);
			{
				Property<ofFloatColor> *prop = new Property<ofFloatColor>("color");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Color", Json::Value::null));
				target->addColorProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("opacity");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Opacity", Json::Value::null), 0.01f);
				target->addOpacityProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("stroke width");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("StrokeWidth", Json::Value::null));
				target->addStrokeWidthProperty(prop);
			}
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap->addContent(target);
			}
		}
		// fill
		else if(type == "fill") {
			ShapeContentFill *target = new ShapeContentFill();
			allocated_.property.push_back(target);
			{
				Property<ofFloatColor> *prop = new Property<ofFloatColor>("color");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Color", Json::Value::null));
				target->addColorProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("opacity");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Opacity", Json::Value::null), 0.01f);
				target->addOpacityProperty(prop);
			}
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap->addContent(target);
			}
		}
		// transform
		else if(type == "transform") {
			{
				TransformProperty *prop  = new TransformProperty();
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(prop->translation_, content.get("Position", Json::Value::null));
				setupPropertyKeysJson(prop->anchor_point_, content.get("AnchorPoint", Json::Value::null));
				setupPropertyKeysJson(prop->scale_, content.get("Scale", Json::Value::null), ofVec3f(0.01f,0.01f,0), ofVec3f(0,0,1));
				parent->addTransformProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("rotation");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Rotation", Json::Value::null));
				parent->addRotationZProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("opacity");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Opacity", Json::Value::null), 0.01f);
				parent->addOpacityProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("skew");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("Skew", Json::Value::null));
				parent->addSkewProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("skew axis");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content.get("SkewAxis", Json::Value::null));
				parent->addSkewAxisProperty(prop);
			}
		}
	}
}
void Loader::setupMarkerJson(Marker& marker, const Json::Value& json)
{
	marker.setupByComment(json.get("comment", "").asString());
	marker.from_ = json.get("from", 0).asInt();
	marker.length_ = json.get("length", 0).asInt();
}

void Loader::setupMaskJson(Mask& mask, const Json::Value& json, const ofVec2f &size)
{
	mask.name_ = json.get("name", "noname").asString();
	const string& blend_mode = json.get("mode", "none").asString();
	if(blend_mode == "none")		{ mask.blend_mode_ = OF_BLENDMODE_ALPHA; }
	if(blend_mode == "add")			{ mask.blend_mode_ = OF_BLENDMODE_ADD; }
	if(blend_mode == "subtract")	{ mask.blend_mode_ = OF_BLENDMODE_SUBTRACT; }
	{
		PathProperty *prop = new PathProperty();
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, json);
		prop->setSize(size);
		prop->setInverted(json.get("inverted", false).asBool());
		mask.addPathProperty(prop);
	}
	{
		Property<float> *prop = new Property<float>("opacity");
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, json.get("opacity", Json::Value::null), 0.01f);
		mask.addOpacityProperty(prop);
	}
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