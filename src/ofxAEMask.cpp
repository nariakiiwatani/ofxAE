#include "ofxAEMask.h"
#include "ofxAEAVLayer.h"
#include "ofGraphics.h"

namespace ofxAE {
Mask::Mask()
{
	path_.setPolyWindingMode(OF_POLY_WINDING_POSITIVE);
	path_.setColor(ofColor::white);
}
	
void Mask::update(AVLayer *layer)
{
	if(path_.isDirty()) {
		if(is_inverted_) {
			path_.updateInverted(layer->getSize());
		}
		else {
			path_.update();
		}
	}
}
void Mask::draw()
{
	ofPushStyle();
	ofEnableBlendMode(blend_mode_);
	path_.draw();
	ofPopStyle();
}

bool Mask::isSubtract()
{
	return blend_mode_==OF_BLENDMODE_SUBTRACT;
}
}

/* EOF */