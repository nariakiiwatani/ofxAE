#pragma once

#include "ofxAEDef.h"
#include "ofxAEProperty.h"

OFX_AE_NAMESPACE_BEGIN

class Mask : public PropertyGroup
{
	friend class Loader;
public:
	Mask(const string &name="mask");
	void draw();
	void prepare();
	
	bool isSubtract();
	void setInverted(bool inverted) { is_inverted_=inverted; }
	void setSize(const ofVec2f& size) { size_=size; }
	float getOpacity() { return opacity_; }
	
	void addPathProperty(PathProperty *prop);
	void addOpacityProperty(Property<float> *prop);
private:
	string name_;
	ofBlendMode blend_mode_;
	ofPath path_;
	bool is_inverted_;
	ofVec2f size_;
	float opacity_;
};

OFX_AE_NAMESPACE_END
/* EOF */