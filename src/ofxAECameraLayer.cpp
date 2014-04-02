#include "ofxAECameraLayer.h"

OFX_AE_NAMESPACE_BEGIN

CameraLayer::CameraLayer()
{
	camera_.setVFlip(true);
}
void CameraLayer::prepare()
{
	camera_.setFov(fov_);
	camera_.lookAt(look_at_);
	camera_.setTransformMatrix(*(getNode().getWorldMatrix()));
}

void CameraLayer::addLookAtProperty(Property<ofVec3f> *prop)
{
	prop->setTarget(&look_at_);
	addProperty(prop);
}

void CameraLayer::addFovProperty(Property<float> *prop)
{
	prop->setTarget(&fov_);
	addProperty(prop);
}

OFX_AE_NAMESPACE_END
/* EOF */