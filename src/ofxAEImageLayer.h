#pragma once

#include "ofxAEDef.h"
#include "ofxAEAVLayer.h"
#include "ofTexture.h"

OFX_AE_NAMESPACE_BEGIN

class ImageLayer : public AVLayer
{
	friend class Loader;
public:
	void loadImage(const string& filepath);
private:
	void render(float alpha=1);
	ofTexture texture_;
};

OFX_AE_NAMESPACE_END
/* EOF */