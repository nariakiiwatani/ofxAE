#pragma once

#include "ofConstants.h"
#include "ofFbo.h"

namespace ofxAE {
	class Layer;
	class CameraLayer;
}

namespace ofxAE {
class Composition {
	friend class Loader;
public:
	void allocate(int width, int height);
	void update();
	void draw();
	void setFrame(int frame);
	float getWidth() { return width_; }
	float getHeight() { return height_; }
private:
	ofFbo fbo_;
	string name_;
	float width_;
	float height_;
	vector<Layer*> layer_;
	vector<CameraLayer*> camera_;
};
}

/* EOF */