#include "ofxAECompositionCap.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

CompositionCap::CompositionCap(AVLayer *layer)
:AVCap(layer)
{
}

void CompositionCap::update()
{
	composition_.update();
}
void CompositionCap::draw(float alpha)
{
	if(isCollapseTransformation()) {
		composition_.draw(NULL, opacity_*alpha);
	}
	else {
		composition_.draw(opacity_*alpha);
	}
}
void CompositionCap::setPropertyFrame(int frame)
{
	composition_.setFrame(frame-start_frame_);
}

OFX_AE_NAMESPACE_END
/* EOF */