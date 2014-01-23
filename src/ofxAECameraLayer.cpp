#include "ofxAECameraLayer.h"

OFX_AE_NAMESPACE_BEGIN

CameraLayer::CameraLayer()
{
	camera_.setVFlip(true);
	properties_.push_back(&fov_);
	properties_.push_back(&look_at_);
}
void CameraLayer::prepare()
{
	camera_.setFov(fov_);
	camera_.lookAt(look_at_);
	camera_.setTransformMatrix(*(getNode().getWorldMatrix()));
}

OFX_AE_NAMESPACE_END
/* EOF */