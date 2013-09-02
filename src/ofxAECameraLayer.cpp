#include "ofxAECameraLayer.h"

namespace ofxAE {

void CameraLayer::begin()
{
	camera_.setTransformMatrix(*getWorldMatrix());
	camera_.lookAt(look_at_);
	camera_.begin();
}

void CameraLayer::end()
{
	camera_.end();
}

void CameraLayer::setFov(float fov)
{
	camera_.setFov(fov);
}

void CameraLayer::setAnchorPoint(const ofVec3f& anchor)
{
	look_at_.set(anchor);
}

}
 /**/
/* EOF */