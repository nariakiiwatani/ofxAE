#include "ofxAESolidLayer.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

void SolidLayer::render()
{
	ofPushStyle();
	ofSetColor(color_, opacity_*255);
	ofRect(0, 0, size_.x, size_.y);
	ofPopStyle();
}

OFX_AE_NAMESPACE_END
/* EOF */