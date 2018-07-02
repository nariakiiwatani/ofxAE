#include "ofxAEAVLayer.h"
#include "ofxAEAVLayerCap.h"
#include "ofGraphics.h"
#include "ofxAEMask.h"

OFX_AE_NAMESPACE_BEGIN

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
		if(cap_) {
			cap_->draw(alpha*opacity_);
		}
		ofx_mask_.end();
		ofx_mask_.draw();
	}
	else {
		if(cap_) {
			cap_->draw(alpha*opacity_);
		}
	}
	getNode().popMatrix();
}

void AVLayer::addMask(Mask *mask)
{
	if(mask_.empty()) {
		ofx_mask_.allocate(getWidth(), getHeight(), ofxMask::ALPHA);
	}
	mask_.push_back(mask);
	addProperty(mask);
}


bool AVLayer::isHit(float x, float y)
{
	return isHit(ofVec3f(x,y));
}

bool AVLayer::isHit(const ofVec3f &point)
{
	ofVec3f inv = getNode().getWorldMatrixInversed()->preMult(point);
	return 0 <= inv.x && inv.x < size_.x && 0 <= inv.y && inv.y < size_.y;
}

OFX_AE_NAMESPACE_END
/* EOF */
