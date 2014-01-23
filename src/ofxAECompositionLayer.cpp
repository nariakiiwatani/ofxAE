#include "ofxAECompositionLayer.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

void CompositionLayer::prepare()
{
	composition_.update();
}
void CompositionLayer::render()
{
	ofPushStyle();
	ofSetColor(ofColor::white, getOpacity()*255);
	if(isCollapse()) {
		composition_.drawCollapse(NULL);
	}
	else {
		composition_.draw();
	}
	ofPopStyle();
}
void CompositionLayer::setPropertyFrame(int frame)
{
	Layer::setPropertyFrame(frame);
	composition_.setFrame(frame-start_frame_);
}

OFX_AE_NAMESPACE_END
/* EOF */