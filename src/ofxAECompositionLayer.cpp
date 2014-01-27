#include "ofxAECompositionLayer.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

void CompositionLayer::prepare()
{
	composition_.update();
}
void CompositionLayer::render(float alpha)
{
	if(isCollapse()) {
		composition_.drawCollapse(NULL, opacity_*alpha);
	}
	else {
		composition_.draw(opacity_*alpha);
	}
}
void CompositionLayer::setPropertyFrame(int frame)
{
	Layer::setPropertyFrame(frame);
	composition_.setFrame(frame-start_frame_);
}

OFX_AE_NAMESPACE_END
/* EOF */