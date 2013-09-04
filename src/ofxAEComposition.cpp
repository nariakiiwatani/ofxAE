#include "ofxAEComposition.h"
#include "ofxAEAVLayer.h"
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
	CameraLayer *active_camera = NULL;
	for(vector<CameraLayer*>::iterator camera = camera_.begin(); camera != camera_.end(); ++camera) {
		if((*camera)->isActive()) {
			(*camera)->update();
			if(!active_camera) {
				active_camera = *camera;
			}
		}
	}
	vector<AVLayer*> sorted;
	multimap<float,AVLayer*> work;
	for(vector<AVLayer*>::iterator layer = av_.begin(); layer != av_.end(); ++layer) {
		AVLayer *l = *layer;
		if(l->isActive()) {
			l->update();
			if(l->is3D()) {
				ofVec3f dist = (*layer)->getWorldMatrix()->getTranslation();
				if(active_camera) {
					dist = active_camera->worldToCamera(dist);
					dist.z = -dist.z;
				}
				work.insert(pair<float,AVLayer*>(dist.z, l));
			}
			else {
				for(multimap<float,AVLayer*>::iterator w = work.begin(); w != work.end(); ++w) {
					sorted.push_back((*w).second);
				}
				work.clear();
				sorted.push_back(l);
			}
		}
	}
	for(multimap<float,AVLayer*>::iterator w = work.begin(); w != work.end(); ++w) {
		sorted.push_back((*w).second);
	}
	work.clear();
	fbo_.begin();
	ofClear(0);
	for(vector<AVLayer*>::iterator layer = sorted.begin(); layer != sorted.end(); ++layer) {
		(*layer)->draw();
	}
	fbo_.end();
}

void Composition::draw()
{
	CameraLayer *active_camera = NULL;
	for(vector<CameraLayer*>::iterator camera = camera_.begin(); camera != camera_.end(); ++camera) {
		if((*camera)->isActive()) {
			(*camera)->begin();
			active_camera = *camera;
			break;
		}
	}
	fbo_.draw(0,0,width_,height_);
	if(active_camera) {
		active_camera->end();
	}
}

void Composition::setFrame(int frame)
{
	for(vector<CameraLayer*>::iterator camera = camera_.begin(); camera != camera_.end(); ++camera) {
		if((*camera)->isActive()) {
			(*camera)->setPropertyFrame(frame);
		}
	}
	for(vector<AVLayer*>::iterator layer = av_.begin(); layer != av_.end(); ++layer) {
		if((*layer)->isActive()) {
			(*layer)->setPropertyFrame(frame);
		}
	}
}
}
/* EOF */