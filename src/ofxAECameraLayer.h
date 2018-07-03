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
	
	std::shared_ptr<Property<ofVec3f>> getLookAtProperty() { return getProperty<ofVec3f>("look at"); }
	std::shared_ptr<Property<float>> getFovProperty() { return getProperty<float>("fov"); }

private:
	ofCamera camera_;
	float fov_;
	ofVec3f look_at_;
};

OFX_AE_NAMESPACE_END
/* EOF */
