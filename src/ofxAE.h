#pragma once

#include "ofConstants.h"
#include "json.h"
#include "ofxAEProperty.h"

namespace ofxAE {
	class Composition;
	class Marker;
	class Layer;
	class AVLayer;
	class Mask;
	class CameraLayer;
	class CompositionLayer;
	class SolidLayer;
	class StillLayer;
	class ShapeLayer;
	class ShapeContentGroup;
}
namespace ofxAE {
class Loader {
public:
	void loadComposition(Composition& comp, const string& filepath);
private:
	void setupCompositionJson(Composition& comp, const Json::Value& json);
	void setupLayerJson(Layer& layer, const Json::Value& json);
	void setupMarkerJson(Marker& marker, const Json::Value& json);
	void setupMaskJson(Mask& mask, const Json::Value& json);
	void setupAVLayerJson(AVLayer& layer, const Json::Value& json);
	void setupCameraLayerJson(CameraLayer& layer, const Json::Value& json, Composition& comp);
	void setupCompositionLayerJson(CompositionLayer& layer, const Json::Value& json);
	void setupSolidLayerJson(SolidLayer& layer, const Json::Value& json);
	void setupStillLayerJson(StillLayer& layer, const Json::Value& json);
	void setupShapeLayerJson(ShapeLayer& layer, const Json::Value& json);
	void setupShapeContentsJson(ShapeLayer& layer, const Json::Value& contents, ShapeContentGroup *parent=NULL);
	template<class Target>
	void setupPropertyKeysJson(Property_<Target,bool>& prop, const Json::Value& json);
	template<class Target>
	void setupPropertyKeysJson(Property_<Target,float>& prop, const Json::Value& json, float scale=1, float offset=0);
	template<class Target>
	void setupPropertyKeysJson(Property_<Target,ofVec2f>& prop, const Json::Value& json, const ofVec2f& scale=ofVec2f(1,1), const ofVec2f& offset=ofVec2f(0,0));
	template<class Target>
	void setupPropertyKeysJson(Property_<Target,ofVec3f>& prop, const Json::Value& json, const ofVec3f& scale=ofVec3f(1,1,1), const ofVec3f& offset=ofVec3f(0,0,0));
	void setupPathJson(Path& path, const Json::Value& json, Layer& layer);
};
}

/* EOF */