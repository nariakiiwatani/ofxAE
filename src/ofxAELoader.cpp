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

using namespace std;

OFX_AE_NAMESPACE_BEGIN

map<string,string> Loader::file_cache_;

Loader::Loader(const string& base_path)
{
	setBasePath(base_path);
}
void Loader::setBasePath(const string &base_path)
{
	base_path_ = ofFilePath::addTrailingSlash(base_path);
}
shared_ptr<Composition> Loader::loadComposition(const string& filepath)
{
	shared_ptr<Composition> comp = shared_ptr<Composition>(new Composition());
	string ext = ofFilePath::getFileExt(filepath);
	if(ext == "json") {
		if(file_cache_.find(base_path_+filepath) == end(file_cache_)) {
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
		map<shared_ptr<Layer>, int> all;
		map<shared_ptr<Layer>, int> children;
		int layer_count = layers.size();
		for(int i = layer_count; i--;) {	// reverse iterate for draw priority
			const ofJson& layer = layers[i];
			const string& type_name = layer.value("layerType", "unknown");
			shared_ptr<Layer> l = nullptr;
			shared_ptr<LayerCap> c = nullptr;
			if(type_name == "composition") {
				auto ll = shared_ptr<AVLayer>(new AVLayer());
				setupAVLayerJson(*ll, layer);
				auto cap = shared_ptr<CompositionCap>(new CompositionCap());
				setupCompositionJson(*cap, layer);
				comp.av_.push_back(ll);
				l = ll;
				c = cap;
			}
			else if(type_name == "solid") {
				auto ll = shared_ptr<AVLayer>(new AVLayer());
				setupAVLayerJson(*ll, layer);
				auto cap = shared_ptr<PlaneCap>(new PlaneCap());
				setupPlaneJson(*cap, layer);
				comp.av_.push_back(ll);
				l = ll;
				c = cap;
			}
			else if(type_name == "sequence") {
				auto ll = shared_ptr<AVLayer>(new AVLayer());
				setupAVLayerJson(*ll, layer);
				auto cap = shared_ptr<SequenceCap>(new SequenceCap());
				setupSequenceJson(*cap, layer);
				comp.av_.push_back(ll);
				l = ll;
				c = cap;
			}
			else if(type_name == "image") {
				auto ll = shared_ptr<AVLayer>(new AVLayer());
				setupAVLayerJson(*ll, layer);
				auto cap = shared_ptr<ImageCap>(new ImageCap());
				setupImageJson(*cap, layer);
				comp.av_.push_back(ll);
				l = ll;
				c = cap;
			}
			else if(type_name == "movie") {
				auto ll = shared_ptr<AVLayer>(new AVLayer());
				setupAVLayerJson(*ll, layer);
				auto cap = shared_ptr<MovieCap>(new MovieCap());
				setupMovieJson(*cap, layer);
				cap->setComposition(&comp);
				comp.av_.push_back(ll);
				l = ll;
				c = cap;
			}
			else if(type_name == "camera") {
				auto ll = shared_ptr<CameraLayer>(new CameraLayer());
				setupCameraLayerJson(*ll, layer, comp);
				comp.camera_.push_back(ll);
				l = ll;
			}
			else if(type_name == "shape") {
				auto ll = shared_ptr<AVLayer>(new AVLayer());
				setupAVLayerJson(*ll, layer);
				auto cap = shared_ptr<ShapeCap>(new ShapeCap());
				setupShapeJson(*cap, layer);
				comp.av_.push_back(ll);
				l = ll;
				c = cap;
			}
			if(!l) {
				continue;
			}
			if(c) {
				l->setCap(c);
			}
			all.insert(pair<shared_ptr<Layer>,int>(l, layer.value("index", 0)));
			auto parent = layer.find("parent");
			if(parent != end(layer)) {
				children.insert(pair<shared_ptr<Layer>,int>(l, *parent));
			}
		}
		// search parent
		for(auto &child : children) {
			for(auto &parent : all) {
				if(child.second == parent.second) {
					child.first->setParent(parent.first);
				}
			}
		}
	}
	// Markers
	auto markers = json.find("marker");
	if(markers != end(json) && markers->is_array()) {
		int marker_count = markers->size();
		for(int i = 0; i < marker_count; ++i) {
			setupMarkerJson(*comp.addMarker(), (*markers)[i]);
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
	setupPropertyKeysJson(*layer.getActiveProperty(), properties["active"]);
	setupPropertyKeysJson(*layer.getOpacityProperty(), properties["Opacity"], 0.01f);
	setupPropertyKeysJson(*layer.getTransformProperty(), properties);
	// Markers
	auto markers = json.find("marker");
	if(markers != end(json) && markers->is_array()) {
		int marker_count = markers->size();
		for(int i = 0; i < marker_count; ++i) {
			const ofJson& marker = (*markers)[i];
			auto m = shared_ptr<Marker>(new Marker());
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
	auto masks = properties.find("mask");
	if(masks != end(properties) && masks->is_array()) {
		int mask_count = masks->size();
		for(int i = 0; i < mask_count; ++i) {
			const ofJson& mask = (*masks)[i];
			auto target = shared_ptr<Mask>(new Mask());
			setupMaskJson(*target, mask, ofVec2f(layer.getWidth(),layer.getHeight()));
			layer.addMask(target);
		}
	}
}

void Loader::setupCameraLayerJson(CameraLayer& layer, const ofJson& json, Composition& comp)
{
	const ofJson& properties = json["property"];
	setupLayerJson(layer, json);
	setupPropertyKeysJson(*layer.getLookAtProperty(), properties["LookAt"], ofVec3f(1,1,-1));
	{
		auto prop = layer.getFovProperty();
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
	}
}
void Loader::setupCompositionJson(CompositionCap &cap, const ofJson& json)
{
	const ofJson& source_dir = json["sourceDirectory"];
	const ofJson& source = json["source"];
	if(!source.is_null()) {
		cap.getComposition() = *loadComposition((source_dir.is_null()?"":source_dir.get<string>())+source.get<string>());
	}
}
void Loader::setupPlaneJson(PlaneCap &cap, const ofJson& json)
{
	const ofJson& color = json["color"];
	if(!color.is_null()) {
		cap.setColor(ofFloatColor(color[0], color[1], color[2]));
	}
}
void Loader::setupImageJson(ImageCap &cap, const ofJson& json)
{
	const ofJson& source_dir = json["sourceDirectory"];
	const ofJson& source = json["source"];
	if(!source.is_null()) {
		cap.loadImage(base_path_+(source_dir.is_null()?"":source_dir.get<string>())+source.get<string>());
	}
}
void Loader::setupMovieJson(MovieCap &cap, const ofJson& json)
{
	const ofJson& source_dir = json["sourceDirectory"];
	const ofJson& source = json["source"];
	if(!source.is_null()) {
		cap.loadMovie(base_path_+(source_dir.is_null()?"":source_dir.get<string>())+source.get<string>());
	}
	bool use_audio = json.value("hasAudio", true) && json.value("audioActive", true);
	cap.setUseAudio(use_audio);
}
void Loader::setupSequenceJson(SequenceCap &cap, const ofJson& json)
{
	const ofJson& source_dir = json["sourceDirectory"];
	const ofJson& source = json["source"];
	if(!source.is_null()) {
		cap.setSequenceString(base_path_+(source_dir.is_null()?"":source_dir.get<string>())+source.get<string>());
	}
}
void Loader::setupShapeJson(ShapeCap &cap, const ofJson& json)
{
	const ofJson& properties = json["property"];
	auto contents = properties.find("contents");
	if(contents != end(properties)) {
		setupShapeContentsJson(cap, *contents);
	}
}
void Loader::setupShapeContentsJson(ShapeCap &cap, const ofJson& contents, shared_ptr<ShapeContentGroup> parent)
{
	int content_count = contents.size();
	for(int i = 0; i < content_count; ++i) {
		const ofJson& content = contents[i];
		const string& type = content.value("type", "");
		if(type == "group") {
			auto target = shared_ptr<ShapeContentGroup>(new ShapeContentGroup());
			auto content = contents.find("contents");
			if(content != end(contents)) {
				setupShapeContentsJson(cap, *content, target);
			}
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap.addContent(target);
			}
		}
		else if(type == "ellipse") {
			auto target = shared_ptr<ShapeContentEllipse>(new ShapeContentEllipse());
			setupPropertyKeysJson(*target->getPositionProperty(), content["Position"]);
			setupPropertyKeysJson(*target->getSizeProperty(), content["Size"]);
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap.addContent(target);
			}
		}
		else if(type == "rect") {
			auto target = shared_ptr<ShapeContentRect>(new ShapeContentRect());
			setupPropertyKeysJson(*target->getPositionProperty(), content["Position"]);
			setupPropertyKeysJson(*target->getSizeProperty(), content["Size"]);
			setupPropertyKeysJson(*target->getRoundnessProperty(), content["Roundness"]);
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap.addContent(target);
			}
		}
		else if(type == "path") {
			auto target = shared_ptr<ShapeContentPath>(new ShapeContentPath());
			{
				auto prop = target->getPathProperty();
				prop->setSize(cap.getSize());
				setupPropertyKeysJson(*prop, content["path"]);
			}
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap.addContent(target);
			}
		}
		else if(type == "stroke") {
			auto target = shared_ptr<ShapeContentStroke>(new ShapeContentStroke());
			setupPropertyKeysJson(*target->getColorProperty(), content["Color"]);
			setupPropertyKeysJson(*target->getOpacityProperty(), content["Opacity"], 0.01f);
			setupPropertyKeysJson(*target->getStrokeWidthProperty(), content["StrokeWidth"]);
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap.addContent(target);
			}
		}
		else if(type == "fill") {
			auto target = shared_ptr<ShapeContentFill>(new ShapeContentFill());
			setupPropertyKeysJson(*target->getColorProperty(), content["Color"]);
			setupPropertyKeysJson(*target->getOpacityProperty(), content["Opacity"], 0.01f);
			if(parent) {
				parent->addContent(target);
			}
			else {
				cap.addContent(target);
			}
		}
		else if(type == "transform") {
			{
				auto prop  = parent->getTransformProperty();
				setupPropertyKeysJson(*prop->getTranslation(), content["Position"]);
				setupPropertyKeysJson(*prop->getAnchorPoint(), content["AnchorPoint"]);
				setupPropertyKeysJson(*prop->getScale(), content["Scale"], ofVec3f(0.01f,0.01f,0), ofVec3f(0,0,1));
			}
			setupPropertyKeysJson(*parent->getRotationZProperty(), content["Rotation"]);
			setupPropertyKeysJson(*parent->getOpacityProperty(), content["Opacity"], 0.01f);
			setupPropertyKeysJson(*parent->getSkewProperty(), content["Skew"]);
			setupPropertyKeysJson(*parent->getSkewAxisProperty(), content["SkewAxis"]);
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
		auto prop = mask.getPathProperty();
		setupPropertyKeysJson(*prop, json);
		prop->setSize(size);
		prop->setInverted(json.value("inverted", false));
	}
	setupPropertyKeysJson(*mask.getOpacityProperty(), json["opacity"], 0.01f);
}

void Loader::setupPropertyKeysJson(TransformProperty& prop, const ofJson& json)
{
	setupPropertyKeysJson(*prop.getTranslation(), json["Position"], ofVec3f(1,1,-1));
	setupPropertyKeysJson(*prop.getScale(), json["Scale"], ofVec3f(0.01f,0.01f,0.01f));
	setupPropertyKeysJson(*prop.getRotation(), json["Rotation"], ofVec3f(-1,-1,1));
	if(json.find("AnchorPoint") != end(json)) {
		setupPropertyKeysJson(*prop.getAnchorPoint(), json["AnchorPoint"], ofVec3f(1,1,-1));
	}
	setupPropertyKeysJson(*prop.getOrientation(), json["Orientation"], ofVec3f(-1,-1,1));
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
			int array_count = array.size();
			prop.setVertexSize(array_count);
			for(int j = 0; j < array_count; ++j) {
				setupPropertyKeysJson(*prop.getVertexProperty(j), array[j]);
			}
		}
	}
	{
		const ofJson& array = json["inTangents"];
		if(array.is_array()) {
			int array_count = array.size();
			prop.setInTangentSize(array_count);
			for(int j = 0; j < array_count; ++j) {
				setupPropertyKeysJson(*prop.getInTangentProperty(j), array[j]);
			}
		}
	}
	{
		const ofJson& array = json["outTangents"];
		if(array.is_array()) {
			int array_count = array.size();
			prop.setOutTangentSize(array_count);
			for(int j = 0; j < array_count; ++j) {
				setupPropertyKeysJson(*prop.getOutTangentProperty(j), array[j]);
			}
		}
	}
}

OFX_AE_NAMESPACE_END

/* EOF */
