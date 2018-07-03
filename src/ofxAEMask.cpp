#include "ofxAEMask.h"
#include "ofGraphics.h"

template<> const ofColor ofColor::white;

OFX_AE_NAMESPACE_BEGIN

Mask::Mask()
{
	path_.setPolyWindingMode(OF_POLY_WINDING_POSITIVE);
	path_.setColor(ofColor::white);
	
	add<PathProperty>("path")->setTarget(&path_);
	addProperty<float>("opacity")->setTarget(&opacity_);
}

void Mask::draw()
{
	ofPushStyle();
	ofEnableBlendMode(blend_mode_);
	path_.setColor(ofColor(ofColor::white, opacity_*255));
	path_.draw();
	ofPopStyle();
}

bool Mask::isSubtract()
{
	return blend_mode_==OF_BLENDMODE_SUBTRACT;
}

OFX_AE_NAMESPACE_END
/* EOF */
