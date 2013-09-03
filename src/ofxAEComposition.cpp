#include "ofxAEComposition.h"
#include "ofxAELayer.h"
#include "ofGraphics.h"
#include "ofxAECameraLayer.h"

namespace ofxAE {

void Composition::allocate(int width, int height)
{
	fbo_.allocate(width, height, GL_RGBA);
	width_ = width;
	height_ = height;
}

void Composition::update()
{
	for(vector<Layer*>::iterator layer = layer_.begin(); layer != layer_.end(); ++layer) {
		(*layer)->update();
	}
	fbo_.begin();
	ofClear(0);
	for(vector<Layer*>::iterator layer = layer_.begin(); layer != layer_.end(); ++layer) {
		(*layer)->draw();
	}
	fbo_.end();
}

void Composition::draw()
{
	for(vector<CameraLayer*>::iterator camera = camera_.begin(); camera != camera_.end(); ++camera) {
		(*camera)->begin();
	}
	fbo_.draw(0,0,width_,height_);
	for(vector<CameraLayer*>::reverse_iterator camera = camera_.rbegin(); camera != camera_.rend(); ++camera) {
		(*camera)->end();
	}
}

void Composition::setFrame(int frame)
{
	for(vector<Layer*>::iterator layer = layer_.begin(); layer != layer_.end(); ++layer) {
		(*layer)->setPropertyFrame(frame);
	}
}
}
/* EOF */