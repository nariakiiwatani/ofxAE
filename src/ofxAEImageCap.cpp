#include "ofxAEImageCap.h"
#include "ofImage.h"
#include "ofGraphics.h"

using namespace std;

OFX_AE_NAMESPACE_BEGIN

void ImageCap::loadImage(const string& filepath)
{
	ofLoadImage(texture_, filepath);
}

void ImageCap::draw(float alpha)
{
	ofPushStyle();
	ofEnableBlendMode(blend_mode_);
	ofSetColor(ofColor::white, opacity_*alpha*255);
	texture_.draw(rect_);
	ofPopStyle();
}

OFX_AE_NAMESPACE_END
/* EOF */
