#pragma once

#include "ofxAEAVLayer.h"
#include "ofxAEComposition.h"

namespace ofxAE {

class CompositionLayer : public AVLayer
{
	friend class Loader;
public:
	void setPropertyFrame(int frame);
private:
	void prepare();
	void render();
	Composition composition_;
};
}

/* EOF */