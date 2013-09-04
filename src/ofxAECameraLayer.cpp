#include "ofxAECameraLayer.h"

namespace ofxAE {

void CameraLayer::update()
{
	Layer::update();
	camera_.setTransformMatrix(*getWorldMatrix());
	camera_.lookAt(look_at_);
}
void CameraLayer::begin()
{
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

ofVec3f CameraLayer::worldToCamera(const ofVec3f& world)
{
	return camera_.worldToCamera(world);
}

}
 /**/
/* EOF */