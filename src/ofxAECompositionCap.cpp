#include "ofxAECompositionCap.h"
#include "ofGraphics.h"
#include "ofxAEAVLayer.h"

OFX_AE_NAMESPACE_BEGIN

void CompositionCap::setLayer(std::shared_ptr<AVLayer> layer)
{
	AVCap::setLayer(layer);
	fbo_.allocate(layer->getWidth(), layer->getHeight(), GL_RGBA);
}

void CompositionCap::update()
{
	if(auto layer = layer_.lock()) {
		composition_.setFrame(layer->getFrame());
	}
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
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
		fbo_.draw(0,0);
		ofPopStyle();
	}
}

OFX_AE_NAMESPACE_END
/* EOF */
