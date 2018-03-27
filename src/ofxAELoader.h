#pragma once

#include "ofxAEDef.h"
#include "ofConstants.h"
#include "ofxAEProperty.h"
#include "ofJson_compatible.h"

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
	
private:
	void setupCompositionJson(Composition& comp, const ofJson& json);
	void setupLayerJson(Layer& layer, const ofJson& json);
	void setupMarkerJson(Marker& marker, const ofJson& json);
	void setupMaskJson(Mask& mask, const ofJson& json, const ofVec2f &size);
	void setupAVLayerJson(AVLayer& layer, const ofJson& json);
	void setupCameraLayerJson(CameraLayer& layer, const ofJson& json, Composition& comp);
	
	void setupCompositionJson(CompositionCap *cap, const ofJson& json);
	void setupPlaneJson(PlaneCap *cap, const ofJson &json);
	void setupImageJson(ImageCap *cap, const ofJson &json);
	void setupSequenceJson(SequenceCap *cap, const ofJson &json);
	void setupMovieJson(MovieCap *cap, const ofJson &json);
	void setupShapeJson(ShapeCap *cap, const ofJson &json);
	void setupShapeContentsJson(ShapeCap *cap, const ofJson& contents, ShapeContentGroup *parent=NULL);

	void setupPropertyKeysJson(Property<bool>& prop, const ofJson& json);
	void setupPropertyKeysJson(Property<float>& prop, const ofJson& json, float scale=1, float offset=0);
	void setupPropertyKeysJson(Property<ofVec2f>& prop, const ofJson& json, const ofVec2f& scale=ofVec2f(1,1), const ofVec2f& offset=ofVec2f(0,0));
	void setupPropertyKeysJson(Property<ofVec3f>& prop, const ofJson& json, const ofVec3f& scale=ofVec3f(1,1,1), const ofVec3f& offset=ofVec3f(0,0,0));
	void setupPropertyKeysJson(Property<ofFloatColor>& prop, const ofJson& json, const ofFloatColor& scale=ofFloatColor(1,1,1,1), const ofFloatColor& offset=ofFloatColor(0,0,0,0));
	void setupPropertyKeysJson(TransformProperty& prop, const ofJson& json);
	void setupPropertyKeysJson(PathProperty& prop, const ofJson& json);
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
