#pragma once

#include "ofxAEDef.h"
#include "ofxAELayer.h"
#include "ofCamera.h"

OFX_AE_NAMESPACE_BEGIN

class CameraLayer : public Layer {
	friend class Loader;
public:
	CameraLayer();
	ofCamera* getCamera() { return &camera_; }
	void prepare();
	
	void addLookAtProperty(Property<ofVec3f> *prop);
	void addFovProperty(Property<float> *prop);

private:
	ofCamera camera_;
	float fov_;
	ofVec3f look_at_;
};

OFX_AE_NAMESPACE_END
/* EOF */