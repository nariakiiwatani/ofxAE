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
	Composition* loadComposition(const string& filepath);
private:
	Composition* loadCompositionJson(const Json::Value& json);
	void loadLayerJson(const Json::Value& json, Composition *comp);
	Marker* loadMarkerJson(const Json::Value& json);
	Mask* loadMaskJson(const Json::Value& json);
	AVLayer* loadAVLayerJson(const Json::Value& json);
	CameraLayer* loadCameraLayerJson(const Json::Value& json, Composition *comp);
};
}

/* EOF */