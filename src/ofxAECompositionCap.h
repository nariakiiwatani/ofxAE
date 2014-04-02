#pragma once

#include "ofxAEDef.h"
#include "ofxAEAVLayerCap.h"
#include "ofxAEComposition.h"
#include "ofFbo.h"

OFX_AE_NAMESPACE_BEGIN

class CompositionCap : public AVCap
{
public:
	CompositionCap(AVLayer *layer);
	Composition& getComposition() { return composition_; }
	void update();
	void draw(float alpha=1);
private:
	Composition composition_;
	ofFbo fbo_;
};

OFX_AE_NAMESPACE_END
/* EOF */