#pragma once

#include "ofxAELayer.h"
#include "ofCamera.h"
#include "ofxAEProperty.h"

namespace ofxAE {
class CameraLayer : public Layer {
	friend class Loader;
public:
	CameraLayer();
	void begin();
	void end();
	
	ofVec3f worldToCamera(const ofVec3f& world);

private:
	void prepare();
	ofCamera camera_;
	Property<float> fov_;
	Property<ofVec3f> look_at_;
};
}

/* EOF */