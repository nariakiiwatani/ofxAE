#include "ofxAEImageLayer.h"
#include "ofImage.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

void ImageLayer::loadImage(const string& filepath)
{
	ofLoadImage(texture_, filepath);
}
	
void ImageLayer::render(float alpha)
{
	ofPushStyle();
	ofSetColor(ofColor::white, opacity_*alpha*255);
	texture_.draw(0, 0, size_.x, size_.y);
	ofPopStyle();
}

OFX_AE_NAMESPACE_END
/* EOF */