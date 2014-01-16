#include "ofxAECameraLayer.h"

namespace ofxAE {

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

}
 /**/
/* EOF */