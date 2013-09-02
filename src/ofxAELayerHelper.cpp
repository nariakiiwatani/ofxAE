#include "ofxAELayerHelper.h"
#include "ofxAEComposition.h"
#include "ofGraphics.h"
#include "ofxAEMask.h"
#include "ofxAEAVLayer.h"
#include "ofImage.h"

namespace ofxAE {
void SolidLayerHelper::render()
{
	ofPushStyle();
	ofSetColor(color_, layer_->getOpacity()*255);
	ofRect(0, 0, layer_->getWidth(), layer_->getHeight());
	ofPopStyle();
}

void StillLayerHelper::loadImage(const string& filepath)
{
	ofLoadImage(texture_, filepath);
}
void StillLayerHelper::render()
{
	ofPushStyle();
	ofSetColor(ofColor::white, layer_->getOpacity()*255);
	texture_.draw(0, 0, layer_->getWidth(), layer_->getHeight());
	ofPopStyle();
}

void CompositionLayerHelper::update()
{
	composition_->update();
}
void CompositionLayerHelper::render()
{
	ofPushStyle();
	ofSetColor(ofColor::white, layer_->getOpacity()*255);
	composition_->draw();
	ofPopStyle();
}
void CompositionLayerHelper::setPropertyFrame(int frame)
{
	composition_->setFrame(frame);
}

}
/* EOF */