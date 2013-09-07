#include "ofxAEAVLayer.h"
#include "ofGraphics.h"
#include "ofxAEMask.h"

namespace ofxAE {
AVLayer::~AVLayer()
{
	for(vector<Mask*>::iterator it = mask_.begin(); it != mask_.end(); ++it) {
		delete *it;
	}
}
void AVLayer::allocate(int width, int height, bool use_mask)
{
	width_ = width;
	height_ = height;

	if(use_mask) {
		ofx_mask_.setup(width, height);
	}
	is_use_mask_ = use_mask;
}

void AVLayer::draw()
{
	pushMatrix();
	if(is_use_mask_) {
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
				if((*it)->isDirty()) {
					(*it)->update(this);
				}
				(*it)->draw();
				++it;
			}
		}
		ofx_mask_.endMask();
		ofx_mask_.begin();
		render();
		ofx_mask_.end();
		ofx_mask_.draw();
	}
	else {
		render();
	}
	popMatrix();
}

}
/* EOF */