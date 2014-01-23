#pragma once

#include "ofxAEDef.h"
#include "ofxAELayer.h"
#include "ofCamera.h"
#include "ofxAEProperty.h"

OFX_AE_NAMESPACE_BEGIN

class CameraLayer : public Layer {
	friend class Loader;
public:
	CameraLayer();
	ofCamera* getCamera() { return &camera_; }

private:
	void prepare();
	ofCamera camera_;
	Property<float> fov_;
	Property<ofVec3f> look_at_;
};

OFX_AE_NAMESPACE_END
/* EOF */