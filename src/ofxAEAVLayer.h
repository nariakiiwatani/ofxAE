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
	virtual ~AVLayer();
	void allocate(int width, int height, bool use_mask);
	void draw();
	float getWidth() { return width_; }
	float getHeight() { return height_; }
	bool is3D() { return is_3d_; }

protected:
	virtual void render(){};
	bool is_3d_;
	float width_;
	float height_;
	bool is_use_mask_;
	vector<Mask*> mask_;
	ofxMask ofx_mask_;
};
}

/* EOF */