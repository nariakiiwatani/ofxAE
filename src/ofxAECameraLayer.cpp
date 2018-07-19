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
//	camera_.setTransformMatrix(*(getNode().getWorldMatrix()));
}

OFX_AE_NAMESPACE_END
/* EOF */
