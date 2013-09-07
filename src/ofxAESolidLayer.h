#pragma once

#include "ofxAEAVLayer.h"
#include "ofColor.h"

namespace ofxAE {

class SolidLayer : AVLayer
{
	friend class Loader;
public:
	void setColor(const ofFloatColor& color) { color_=color; }
private:
	void render();
	ofFloatColor color_;
};
}

/* EOF */