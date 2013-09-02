#pragma once

#include "ofxAELayer.h"
#include "ofxMask.h"

namespace ofxAE {
	class Mask;
	class Marker;
}
namespace ofxAE {
class AVLayer : public Layer {
	friend class Loader;
public:
	void allocate(int width, int height, bool use_mask);
	void render();
	float getWidth() { return width_; }
	float getHeight() { return height_; }

protected:
	float width_;
	float height_;
	bool is_use_mask_;
	vector<Mask*> mask_;
	ofxMask ofx_mask_;
};
}

/* EOF */