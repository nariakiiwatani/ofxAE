#include "ofxAEPlaneCap.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

void PlaneCap::draw(float alpha)
{
	ofPushStyle();
	ofEnableBlendMode(blend_mode_);
	ofSetColor(color_, opacity_*alpha*color_.a);
	ofDrawRectangle(rect_);
	ofPopStyle();
}

OFX_AE_NAMESPACE_END
/* EOF */
