#include "ofxAECompositionCap.h"
#include "ofGraphics.h"
#include "ofxAEAVLayer.h"

OFX_AE_NAMESPACE_BEGIN

CompositionCap::CompositionCap(AVLayer *layer)
:AVCap(layer)
{
	fbo_.allocate(layer->getWidth(), layer->getHeight(), GL_RGBA);
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
		fbo_.begin();
		ofClear(0);
		composition_.draw(1);
		fbo_.end();
		ofPushStyle();
		ofSetColor(ofColor::white, opacity_*alpha*255);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		fbo_.draw(0,0);
		ofPopStyle();
	}
}
void CompositionCap::setPropertyFrame(int frame)
{
	composition_.setFrame(frame-start_frame_);
}

OFX_AE_NAMESPACE_END
/* EOF */