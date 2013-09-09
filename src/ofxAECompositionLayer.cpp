#include "ofxAECompositionLayer.h"
#include "ofGraphics.h"

namespace ofxAE {
void CompositionLayer::prepare()
{
	composition_.update();
}
void CompositionLayer::render()
{
	ofPushStyle();
	ofSetColor(ofColor::white, getOpacity()*255);
	composition_.draw();
	ofPopStyle();
}
void CompositionLayer::setPropertyFrame(int frame)
{
	Layer::setPropertyFrame(frame);
	composition_.setFrame(frame-start_frame_);
}

}
/* EOF */