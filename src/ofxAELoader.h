#pragma once

#include "ofxAEDef.h"
#include "ofConstants.h"
#include "json.h"
#include "ofxAEProperty.h"

OFX_AE_NAMESPACE_BEGIN

class Composition;
class Marker;
class Layer;
class AVLayer;
class Mask;
class CameraLayer;
class ShapeContent;
class ShapeContentGroup;

class LayerCap;
class PlaneCap;
class ImageCap;
class SequenceCap;
class MovieCap;
class ShapeCap;
class CompositionCap;

class Loader {
public:
	Loader(const string& base_path="");
	~Loader();
	void setBasePath(const string& base_path);
	Composition* loadComposition(const string& filepath);
	Composition* getComposition(int index);
	
	void loadComposition(Composition& comp, const string& filepath);	// will be deleted on next update
private:
	void setupCompositionJson(Composition& comp, const Json::Value& json);
	void setupLayerJson(Layer& layer, const Json::Value& json);
	void setupMarkerJson(Marker& marker, const Json::Value& json);
	void setupMaskJson(Mask& mask, const Json::Value& json, const ofVec2f &size);
	void setupAVLayerJson(AVLayer& layer, const Json::Value& json);
	void setupCameraLayerJson(CameraLayer& layer, const Json::Value& json, Composition& comp);
	
	void setupCompositionJson(CompositionCap *cap, const Json::Value& json);
	void setupPlaneJson(PlaneCap *cap, const Json::Value &json);
	void setupImageJson(ImageCap *cap, const Json::Value &json);
	void setupSequenceJson(SequenceCap *cap, const Json::Value &json);
	void setupMovieJson(MovieCap *cap, const Json::Value &json);
	void setupShapeJson(ShapeCap *cap, const Json::Value &json);
	void setupShapeContentsJson(ShapeCap *cap, const Json::Value& contents, ShapeContentGroup *parent=NULL);

	void setupPropertyKeysJson(Property<bool>& prop, const Json::Value& json);
	void setupPropertyKeysJson(Property<float>& prop, const Json::Value& json, float scale=1, float offset=0);
	void setupPropertyKeysJson(Property<ofVec2f>& prop, const Json::Value& json, const ofVec2f& scale=ofVec2f(1,1), const ofVec2f& offset=ofVec2f(0,0));
	void setupPropertyKeysJson(Property<ofVec3f>& prop, const Json::Value& json, const ofVec3f& scale=ofVec3f(1,1,1), const ofVec3f& offset=ofVec3f(0,0,0));
	void setupPropertyKeysJson(Property<ofFloatColor>& prop, const Json::Value& json, const ofFloatColor& scale=ofFloatColor(1,1,1,1), const ofFloatColor& offset=ofFloatColor(0,0,0,0));
	void setupPropertyKeysJson(TransformProperty& prop, const Json::Value& json);
	void setupPropertyKeysJson(PathProperty& prop, const Json::Value& json);
private:
	string base_path_;
	static map<string,string> file_cache_;
	struct {
		vector<Composition*> comp;
		vector<Layer*> layer;
		vector<LayerCap*> cap;
		vector<PropertyBase*> property;
		vector<Marker*> marker;
	} allocated_;
};

OFX_AE_NAMESPACE_END

/* EOF */