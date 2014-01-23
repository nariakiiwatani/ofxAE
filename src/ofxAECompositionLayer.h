#pragma once

#include "ofxAEDef.h"
#include "ofxAEAVLayer.h"
#include "ofxAEComposition.h"

OFX_AE_NAMESPACE_BEGIN

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

OFX_AE_NAMESPACE_END
/* EOF */