#pragma once

#include "ofxAEProperty.h"

namespace ofxAE {
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
private:
	string name_;
	ofBlendMode blend_mode_;
	PathProperty path_;
};

}

/* EOF */