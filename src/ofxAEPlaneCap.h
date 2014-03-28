#pragma once

#include "ofxAEDef.h"
#include "ofxAEAVLayerCap.h"
#include "ofColor.h"

OFX_AE_NAMESPACE_BEGIN

class PlaneCap : public AVCap
{
public:
	PlaneCap(AVLayer *layer);
	void draw(float alpha=1);
	
	void setColor(const ofColor &color) { color_=color; }
	
	const ofColor& getColor() { return color_; }
private:
	ofColor color_;
};


OFX_AE_NAMESPACE_END
/* EOF */