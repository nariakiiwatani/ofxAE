#include "ofxAEAVLayer.h"
#include "ofxAEAVLayerCap.h"
#include "ofGraphics.h"
#include "ofxAEMask.h"

OFX_AE_NAMESPACE_BEGIN

AVLayer::AVLayer()
{
}

AVLayer::~AVLayer()
{
	for(vector<Mask*>::iterator it = mask_.begin(); it != mask_.end(); ++it) {
		delete *it;
	}
}
void AVLayer::allocate(int width, int height)
{
	size_.set(width, height);
}

void AVLayer::draw(float alpha)
{
	getNode().pushMatrix();
	if(!mask_.empty()) {
		ofx_mask_.beginMask();
		if(mask_.empty()) {
			ofClear(ofColor::white);
		}
		else {
			vector<Mask*>::iterator it = mask_.begin();
			if((*it)->isSubtract()) {
				ofClear(ofColor::white);
			}
			while(it != mask_.end()) {
				(*it)->draw();
				++it;
			}
		}
		ofx_mask_.endMask();
		ofx_mask_.begin();
		for(vector<LayerCap*>::iterator c = cap_.begin(); c != cap_.end(); ++c) {
			(*c)->draw(alpha*opacity_);
		}
		ofx_mask_.end();
		ofx_mask_.draw();
	}
	else {
		for(vector<LayerCap*>::iterator c = cap_.begin(); c != cap_.end(); ++c) {
			(*c)->draw(alpha*opacity_);
		}
	}
	getNode().popMatrix();
}

void AVLayer::addMask(Mask *mask)
{
	if(mask_.empty()) {
		ofx_mask_.setup(getWidth(), getHeight(), ofxMask::ALPHA);
	}
	mask->setSize(getSize());
	mask_.push_back(mask);
	properties_.push_back(&mask->getPath());
	properties_.push_back(&mask->getOpacity());
}

OFX_AE_NAMESPACE_END
/* EOF */