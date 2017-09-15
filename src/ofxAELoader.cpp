#include "ofxAELoader.h"

#include "ofFileUtils.h"
#include "ofxJsonUtils.h"
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
		if(file_cache_.find(base_path_+filepath) == file_cache_.end()) {
			file_cache_.insert(pair<string,string>(base_path_+filepath,ofBufferFromFile(base_path_+filepath).getText()));
		}
		ofJson json = ofxJsonUtils::parse(file_cache_[base_path_+filepath]);
		if(!json.is_null()) {
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
void Loader::setupCompositionJson(Composition& comp, const ofJson& json)
{
	// Basics
	comp.name_ = json.value("name", "noname");
	comp.allocate(json.value("width", 1), json.value("height", 1));
	comp.setLength(json.value("length", 0));
	comp.setFrameRate(json.value("frameRate", 0));
	// Layers
	const ofJson& layers = json["layer"];
	if(layers.is_array()) {
		map<Layer*, int> all;
		map<Layer*, int> children;
		int layer_count = layers.size();
		for(int i = layer_count; i--;) {	// reverse iterate for draw priority
			const ofJson& layer = layers[i];
			const string& type_name = layer.value("layerType", "unknown");
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
			all.insert(pair<Layer*,int>(l, layer.value("index", 0)));
			auto parent = layer.find("parent");
			if(parent != end(layer)) {
				children.insert(pair<Layer*,int>(l, *parent));
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
	auto markers = json.find("marker");
	if(markers != end(json) && markers->is_array()) {
		int marker_count = markers->size();
		for(int i = 0; i < marker_count; ++i) {
			const ofJson& marker = (*markers)[i];
			Marker *m = new Marker();
			allocated_.marker.push_back(m);
			setupMarkerJson(*m, marker);
			comp.addMarker(m);
		}
	}
	comp.setFrame(0);
}

void Loader::setupLayerJson(Layer& layer, const ofJson& json)
{
	layer.name_ = json.value("name", "noname");
	layer.frame_offset_ = json.value("frameOffset", 0);
	layer.frame_in_ = json.value("inFrame", 0);
	layer.frame_out_ = json.value("outFrame", 0);
	layer.setParamByComment(json.value("comment", ""));
	const ofJson& properties = json["property"];
	{
		Property<bool> *prop = new Property<bool>("active");
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, properties["active"]);
		layer.addActiveProperty(prop);
	}
	{
		Property<float> *prop = new Property<float>("opacity");
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, properties["Opacity"], 0.01f);
		layer.addOpacityProperty(prop);
	}
	{
		TransformProperty *prop = new TransformProperty();
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, properties);
		layer.addTransformProperty(prop);
	}
	// Markers
	auto markers = json.find("marker");
	if(markers != end(json) && markers->is_array()) {
		int marker_count = markers->size();
		for(int i = 0; i < marker_count; ++i) {
			const ofJson& marker = (*markers)[i];
			Marker *m = new Marker();
			allocated_.marker.push_back(m);
			setupMarkerJson(*m, marker);
			layer.marker_.push_back(m);
		}
	}
}
void Loader::setupAVLayerJson(AVLayer& layer, const ofJson& json)
{
	const ofJson& properties = json["property"];
	layer.allocate(json.value("width", 1), json.value("height", 1));
	layer.is_3d_ = json.value("is3d", false);
	layer.is_collapse_ = json.value("isCollapse", false);
	const string& blend_mode = json.value("blendingMode", "none");
	if(blend_mode == "none")		{ layer.blend_mode_ = OF_BLENDMODE_ALPHA; }
	if(blend_mode == "add")			{ layer.blend_mode_ = OF_BLENDMODE_ADD; }
	if(blend_mode == "subtract")	{ layer.blend_mode_ = OF_BLENDMODE_SUBTRACT; }
	setupLayerJson(layer, json);
	auto masks = json.find("marker");
	if(masks != end(json) && masks->is_array()) {
		int mask_count = masks->size();
		for(int i = 0; i < mask_count; ++i) {
			const ofJson& mask = (*masks)[i];
			Mask *target = new Mask();
			allocated_.property.push_back(target);
			setupMaskJson(*target, mask, ofVec2f(layer.getWidth(),layer.getHeight()));
			layer.addMask(target);
		}
	}
}

void Loader::setupCameraLayerJson(CameraLayer& layer, const ofJson& json, Composition& comp)
{
	const ofJson& properties = json["property"];
	setupLayerJson(layer, json);
	{
		Property<ofVec3f> *prop = new Property<ofVec3f>("look at");
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, properties["LookAt"], ofVec3f(1,1,-1));
		layer.addLookAtProperty(prop);
	}
	{
		Property<float> *prop = new Property<float>("fov");
		allocated_.property.push_back(prop);
		auto zoom = properties.find("Zoom");
		if(zoom != end(properties)) {
			const ofJson& keys = properties["Zoom"];
			for(auto key = keys.begin(); key != keys.end(); ++key) {
				const string& name = key.key();
				int key_frame = ofToInt(name);
				float value = 2 * atan(comp.getHeight() / (2 * keys[name].get<float>())) * (180 / PI);
				prop->addKey(key_frame, value);
			}
		}
		layer.addFovProperty(prop);
	}
}
void Loader::setupCompositionJson(CompositionCap *cap, const ofJson& json)
{
	const ofJson& source_dir = json["sourceDirectory"];
	const ofJson& source = json["source"];
	if(!source.is_null()) {
		cap->getComposition() = *loadComposition((source_dir.is_null()?"":source_dir.get<string>())+source.get<string>());
	}
}
void Loader::setupPlaneJson(PlaneCap *cap, const ofJson& json)
{
	const ofJson& color = json["color"];
	if(!color.is_null()) {
		cap->setColor(ofFloatColor(color[0], color[1], color[2]));
	}
}
void Loader::setupImageJson(ImageCap *cap, const ofJson& json)
{
	const ofJson& source_dir = json["sourceDirectory"];
	const ofJson& source = json["source"];
	if(!source.is_null()) {
		cap->loadImage(base_path_+(source_dir.is_null()?"":source_dir.get<string>())+source.get<string>());
	}
}
void Loader::setupMovieJson(MovieCap *cap, const ofJson& json)
{
	const ofJson& source_dir = json["sourceDirectory"];
	const ofJson& source = json["source"];
	if(!source.is_null()) {
		cap->loadMovie(base_path_+(source_dir.is_null()?"":source_dir.get<string>())+source.get<string>());
	}
	bool use_audio = json.value("hasAudio", true) && json.value("audioActive", true);
	cap->setUseAudio(use_audio);
}
void Loader::setupSequenceJson(SequenceCap *cap, const ofJson& json)
{
	const ofJson& source_dir = json["sourceDirectory"];
	const ofJson& source = json["source"];
	if(!source.is_null()) {
		cap->setSequenceString(base_path_+(source_dir.is_null()?"":source_dir.get<string>())+source.get<string>());
	}
}
void Loader::setupShapeJson(ShapeCap *cap, const ofJson& json)
{
	const ofJson& properties = json["property"];
	auto contents = properties.find("contents");
	if(contents != end(properties)) {
		setupShapeContentsJson(cap, *contents);
	}
}
void Loader::setupShapeContentsJson(ShapeCap *cap, const ofJson& contents, ShapeContentGroup *parent)
{
	int content_count = contents.size();
	for(int i = 0; i < content_count; ++i) {
		const ofJson& content = contents[i];
		const string& type = content.value("type", "");
		// group
		if(type == "group") {
			ShapeContentGroup *target = new ShapeContentGroup();
			allocated_.property.push_back(target);
			auto content = contents.find("contents");
			if(content != end(contents)) {
				setupShapeContentsJson(cap, *content, target);
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
				setupPropertyKeysJson(*prop, content["Position"]);
				target->addPositionProperty(prop);
			}
			{
				Property<ofVec2f> *prop = new Property<ofVec2f>("size");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content["Size"]);
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
				setupPropertyKeysJson(*prop, content["Position"]);
				target->addPositionProperty(prop);
			}
			{
				Property<ofVec2f> *prop = new Property<ofVec2f>("size");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content["Size"]);
				target->addSizeProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("roundness");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content["Roundness"]);
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
				setupPropertyKeysJson(*prop, content["path"]);
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
				setupPropertyKeysJson(*prop, content["Color"]);
				target->addColorProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("opacity");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content["Opacity"], 0.01f);
				target->addOpacityProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("stroke width");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content["StrokeWidth"]);
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
				setupPropertyKeysJson(*prop, content["Color"]);
				target->addColorProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("opacity");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content["Opacity"], 0.01f);
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
				setupPropertyKeysJson(prop->translation_, content["Position"]);
				setupPropertyKeysJson(prop->anchor_point_, content["AnchorPoint"]);
				setupPropertyKeysJson(prop->scale_, content["Scale"], ofVec3f(0.01f,0.01f,0), ofVec3f(0,0,1));
				parent->addTransformProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("rotation");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content["Rotation"]);
				parent->addRotationZProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("opacity");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content["Opacity"], 0.01f);
				parent->addOpacityProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("skew");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content["Skew"]);
				parent->addSkewProperty(prop);
			}
			{
				Property<float> *prop = new Property<float>("skew axis");
				allocated_.property.push_back(prop);
				setupPropertyKeysJson(*prop, content["SkewAxis"]);
				parent->addSkewAxisProperty(prop);
			}
		}
	}
}
void Loader::setupMarkerJson(Marker& marker, const ofJson& json)
{
	marker.setupByComment(json.value("comment", ""));
	marker.from_ = json.value("from", 0);
	marker.length_ = json.value("length", 0);
}

void Loader::setupMaskJson(Mask& mask, const ofJson& json, const ofVec2f &size)
{
	mask.name_ = json.value("name", "noname");
	const string& blend_mode = json.value("mode", "none");
	if(blend_mode == "none")		{ mask.blend_mode_ = OF_BLENDMODE_ALPHA; }
	if(blend_mode == "add")			{ mask.blend_mode_ = OF_BLENDMODE_ADD; }
	if(blend_mode == "subtract")	{ mask.blend_mode_ = OF_BLENDMODE_SUBTRACT; }
	{
		PathProperty *prop = new PathProperty();
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, json);
		prop->setSize(size);
		prop->setInverted(json.value("inverted", false));
		mask.addPathProperty(prop);
	}
	{
		Property<float> *prop = new Property<float>("opacity");
		allocated_.property.push_back(prop);
		setupPropertyKeysJson(*prop, json["opacity"], 0.01f);
		mask.addOpacityProperty(prop);
	}
}

void Loader::setupPropertyKeysJson(TransformProperty& prop, const ofJson& json)
{
	setupPropertyKeysJson(prop.translation_, json["Position"], ofVec3f(1,1,-1));
	setupPropertyKeysJson(prop.scale_, json["Scale"], ofVec3f(0.01f,0.01f,0.01f));
	setupPropertyKeysJson(prop.rotation_, json["Rotation"], ofVec3f(-1,-1,1));
	if(json.find("AnchorPoint") != end(json)) {
		setupPropertyKeysJson(prop.anchor_point_, json["AnchorPoint"], ofVec3f(1,1,-1));
	}
	setupPropertyKeysJson(prop.orientation_, json["Orientation"], ofVec3f(-1,-1,1));
}

void Loader::setupPropertyKeysJson(Property<bool>& prop, const ofJson& json)
{
	for(auto key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key();
		int key_frame = ofToInt(name);
		bool value = json[name];
		prop.addKey(key_frame, value);
	}
}
void Loader::setupPropertyKeysJson(Property<float>& prop, const ofJson& json, float scale, float offset)
{
	for(auto key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key();
		int key_frame = ofToInt(name);
		float value = json.value(name, 0)*scale+offset;
		prop.addKey(key_frame, value);
	}
}
void Loader::setupPropertyKeysJson(Property<ofVec2f>& prop, const ofJson& json, const ofVec2f& scale, const ofVec2f& offset)
{
	for(auto key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key();
		int key_frame = ofToInt(name);
		ofVec2f value = ofVec2f(json[name][0], json[name][1])*scale+offset;
		prop.addKey(key_frame, value);
	}
}
void Loader::setupPropertyKeysJson(Property<ofVec3f>& prop, const ofJson& json, const ofVec3f& scale, const ofVec3f& offset)
{
	for(auto key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key();
		int key_frame = ofToInt(name);
		ofVec3f value = ofVec3f(json[name][0], json[name][1], json[name][2])*scale+offset;
		prop.addKey(key_frame, value);
	}
}
void Loader::setupPropertyKeysJson(Property<ofFloatColor>& prop, const ofJson& json, const ofFloatColor& scale, const ofFloatColor& offset)
{
	for(auto key = json.begin(); key != json.end(); ++key) {
		const string& name = key.key();
		int key_frame = ofToInt(name);
		ofFloatColor value = ofFloatColor(json[name][0], json[name][1], json[name][2])*scale+offset;
		prop.addKey(key_frame, value);
	}
}
void Loader::setupPropertyKeysJson(PathProperty& prop, const ofJson& json)
{
	{
		const ofJson& array = json["vertices"];
		if(array.is_array()) {
			prop.setVertexSize(array.size());
			int array_count = array.size();
			for(int j = 0; j < array_count; ++j) {
				setupPropertyKeysJson(prop.vertices_[j], array[j]);
			}
		}
	}
	{
		const ofJson& array = json["inTangents"];
		if(array.is_array()) {
			prop.setInTangentSize(array.size());
			int array_count = array.size();
			for(int j = 0; j < array_count; ++j) {
				setupPropertyKeysJson(prop.in_tangents_[j], array[j]);
			}
		}
	}
	{
		const ofJson& array = json["outTangents"];
		if(array.is_array()) {
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
