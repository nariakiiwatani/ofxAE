#include "ofxAEStillLayer.h"
#include "ofImage.h"
#include "ofGraphics.h"

namespace ofxAE {
void StillLayer::loadImage(const string& filepath)
{
	ofLoadImage(texture_, filepath);
}
	
void StillLayer::render()
{
	ofPushStyle();
	ofSetColor(ofColor::white, opacity_*255);
	texture_.draw(0, 0, size_.x, size_.y);
	ofPopStyle();
}
}
/* EOF */