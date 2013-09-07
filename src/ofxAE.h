#pragma once

#include "ofConstants.h"
#include "json.h"

namespace ofxAE {
	class Composition;
	class Marker;
	class Layer;
	class AVLayer;
	class Mask;
	class CameraLayer;
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
	void setupCameraLayerJson(CameraLayer& camera, const Json::Value& json, Composition& comp);
};
}

/* EOF */