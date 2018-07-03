#pragma once

#include "ofxAEDef.h"
#include "ofxAEProperty.h"

OFX_AE_NAMESPACE_BEGIN

class Mask : public PropertyGroup
{
	friend class Loader;
public:
	Mask();
	void draw();
	void prepare();
	
	bool isSubtract();
	float getOpacity() { return opacity_; }
	
	std::shared_ptr<PathProperty> getPathProperty() { return get<PathProperty>("path"); }
	std::shared_ptr<Property<float>> getOpacityProperty() { return getProperty<float>("opacity"); }
private:
	string name_;
	ofBlendMode blend_mode_;
	ofPath path_;
	float opacity_;
};

OFX_AE_NAMESPACE_END
/* EOF */
