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
		if(!is_mask_allocated_) {
			ofx_mask_.allocate(getWidth(), getHeight(), ofxMask::ALPHA);
			is_mask_allocated_ = true;
		}
		ofx_mask_.beginMask();
		if(mask_.front()->isSubtract()) {
			ofClear(ofColor::white);
		}
		for(auto &m : mask_) {
			m->draw();
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

void AVLayer::addMask(std::shared_ptr<Mask> mask)
{
	mask_.push_back(mask);
	add("mask", mask);
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
