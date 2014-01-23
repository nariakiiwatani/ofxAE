#pragma once

#include "ofxAEDef.h"
#include "ofxAEProperty.h"

OFX_AE_NAMESPACE_BEGIN

class Mask
{
	friend class Loader;
public:
	Mask();
	void draw();
	void prepare();
	
	bool isSubtract();
	void setInverted(bool inverted) { path_.setInverted(inverted); }
	void setSize(const ofVec2f& size) { path_.setSize(size); }
	PathProperty& getPath() { return path_; }
	Property<float>& getOpacity() { return opacity_; }
private:
	string name_;
	ofBlendMode blend_mode_;
	PathProperty path_;
	Property<float> opacity_;
};

OFX_AE_NAMESPACE_END
/* EOF */