#include "ofxAEMask.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

Mask::Mask(const string &name)
:PropertyGroup(name)
{
	path_.setPolyWindingMode(OF_POLY_WINDING_POSITIVE);
	path_.setColor(ofColor::white);
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

void Mask::addPathProperty(PathProperty *prop)
{
	prop->setTarget(&path_);
	addProperty(prop);
}

void Mask::addOpacityProperty(Property<float> *prop)
{
	prop->setTarget(&opacity_);
	addProperty(prop);
}

OFX_AE_NAMESPACE_END
/* EOF */