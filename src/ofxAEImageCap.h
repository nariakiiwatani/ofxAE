#pragma once

#include "ofxAEDef.h"
#include "ofxAEAVLayerCap.h"
#include "ofTexture.h"

OFX_AE_NAMESPACE_BEGIN

class ImageCap : public AVCap
{
public:
	ImageCap(AVLayer *layer);
	void loadImage(const string& filepath);
	void draw(float alpha=1);
private:
	ofTexture texture_;
};

OFX_AE_NAMESPACE_END
/* EOF */