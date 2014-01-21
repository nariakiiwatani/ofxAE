#include "ofxAEMask.h"
#include "ofxAEAVLayer.h"
#include "ofGraphics.h"

namespace ofxAE {
Mask::Mask()
{
	ofPath& path = path_.get();
	path.setPolyWindingMode(OF_POLY_WINDING_POSITIVE);
	path.setColor(ofColor::white);
}

void Mask::draw()
{
	ofPath& path = path_.get();
	ofPushStyle();
	ofEnableBlendMode(blend_mode_);
	path.setColor(ofColor(ofColor::white, opacity_*255));
	path.draw();
	ofPopStyle();
}

bool Mask::isSubtract()
{
	return blend_mode_==OF_BLENDMODE_SUBTRACT;
}
}

/* EOF */