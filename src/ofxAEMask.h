#pragma once

#include "ofxAEPath.h"

namespace ofxAE {
	class AVLayer;
}
namespace ofxAE {
class Mask {
	friend class Loader;
public:
	Mask();
	void update(AVLayer *layer);
	void draw();
	
	bool isSubtract();
	
	Path& getPath() { return path_; }

private:
	string name_;
	ofBlendMode blend_mode_;
	bool is_inverted_;
	Path path_;
};

}

/* EOF */