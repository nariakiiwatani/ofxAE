#pragma once

#include "ofxAEAVLayer.h"
#include "ofxAEComposition.h"

namespace ofxAE {

class CompositionLayer : public AVLayer
{
	friend class Loader;
public:
	void setPropertyFrame(int frame);
	Composition& getComposition() { return composition_; }
private:
	void prepare();
	void render();
	Composition composition_;
};
}

/* EOF */