#include "ofxAESolidLayer.h"
#include "ofGraphics.h"

namespace ofxAE {
void SolidLayer::render()
{
	ofPushStyle();
	ofSetColor(color_, opacity_*255);
	ofRect(0, 0, size_.x, size_.y);
	ofPopStyle();
}
}
/* EOF */