#pragma once

#include "ofxAELayer.h"
#include "ofCamera.h"

namespace ofxAE {
class CameraLayer : public Layer {
	friend class Loader;
public:
	CameraLayer();
	void update();
	void begin();
	void end();
	
	void setFov(float fov);
	void setAnchorPoint(const ofVec3f& anchor);
	ofVec3f worldToCamera(const ofVec3f& world);

protected:
	ofCamera camera_;
	ofVec3f look_at_;
};
}

/* EOF */