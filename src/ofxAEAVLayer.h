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
	float getWidth() { return size_.x; }
	float getHeight() { return size_.y; }
	const ofVec2f& getSize() { return size_; }
	bool is3D() { return is_3d_; }
	bool isCollapse() { return is_collapse_; }
	void addMask(Mask *mask);

protected:
	virtual void render(){};
	bool is_3d_;
	bool is_collapse_;
	ofVec2f size_;
	bool is_use_mask_;
	vector<Mask*> mask_;
	ofxMask ofx_mask_;
};
}

/* EOF */