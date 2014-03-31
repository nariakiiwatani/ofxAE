#include "ofxAECameraLayer.h"

OFX_AE_NAMESPACE_BEGIN

CameraLayer::CameraLayer()
{
	camera_.setVFlip(true);
	addProperty(&fov_);
	addProperty(&look_at_);
}
void CameraLayer::prepare()
{
	camera_.setFov(fov_);
	camera_.lookAt(look_at_);
	camera_.setTransformMatrix(*(getNode().getWorldMatrix()));
}

OFX_AE_NAMESPACE_END
/* EOF */