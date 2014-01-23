#pragma once

#include "ofxAEDef.h"
#include "ofxAEAVLayer.h"
#include "ofColor.h"

OFX_AE_NAMESPACE_BEGIN

class SolidLayer : public AVLayer
{
	friend class Loader;
public:
	void setColor(const ofFloatColor& color) { color_=color; }
private:
	void render();
	ofFloatColor color_;
};

OFX_AE_NAMESPACE_END
/* EOF */