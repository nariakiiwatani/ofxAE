#include "ofxAECameraLayer.h"

using namespace std;

OFX_AE_NAMESPACE_BEGIN

CameraLayer::CameraLayer()
{
	camera_.setVFlip(true);
	addProperty<ofVec3f>("look at")->setTarget(&look_at_);
	addProperty<float>("fov")->setTarget(&fov_);
}
void CameraLayer::prepare()
{
	camera_.setFov(fov_);
	camera_.lookAt(look_at_);
	auto &&matrix = *getNode().getWorldMatrix();
	camera_.setPosition(matrix.getTranslation());
	camera_.setOrientation(matrix.getRotate());
	camera_.setScale(matrix.getScale());
}

OFX_AE_NAMESPACE_END
/* EOF */
