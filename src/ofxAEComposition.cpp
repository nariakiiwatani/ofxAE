#include "ofxAEComposition.h"
#include "ofxAEAVLayer.h"
#include "ofGraphics.h"
#include "ofxAECameraLayer.h"

namespace ofxAE {
Composition::~Composition()
{
	for(vector<AVLayer*>::iterator it = av_.begin(); it != av_.end(); ++it) {
		delete *it;
	}
	for(vector<CameraLayer*>::iterator it = camera_.begin(); it != camera_.end(); ++it) {
		delete *it;
	}
}

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
	fbo_.begin();
	ofClear(0);
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
				if(!work.empty()) {
					if(active_camera) {
						active_camera->begin();
					}
					for(multimap<float,AVLayer*>::iterator w = work.begin(); w != work.end(); ++w) {
						(*w).second->draw();
					}
					work.clear();
					if(active_camera) {
						active_camera->end();
					}
				}
				l->draw();
			}
		}
	}
	if(!work.empty()) {
		if(active_camera) {
			active_camera->begin();
		}
		for(multimap<float,AVLayer*>::iterator w = work.begin(); w != work.end(); ++w) {
			(*w).second->draw();
		}
		work.clear();
		if(active_camera) {
			active_camera->end();
		}
	}
	fbo_.end();
}

void Composition::draw()
{
	fbo_.draw(0,0,width_,height_);
}

void Composition::setFrame(int frame)
{
	for(vector<CameraLayer*>::iterator camera = camera_.begin(); camera != camera_.end(); ++camera) {
		(*camera)->setPropertyFrame(frame);
	}
	for(vector<AVLayer*>::iterator layer = av_.begin(); layer != av_.end(); ++layer) {
		(*layer)->setPropertyFrame(frame);
	}
}

AVLayer* Composition::getAVLayer(const string& name)
{
	for(vector<AVLayer*>::iterator it = av_.begin(); it != av_.end(); ++it) {
		if((*it)->getName() == name) {
			return *it;
		}
	}
	return NULL;
}
CameraLayer* Composition::getCameraLayer(const string& name)
{
	for(vector<CameraLayer*>::iterator it = camera_.begin(); it != camera_.end(); ++it) {
		if((*it)->getName() == name) {
			return *it;
		}
	}
	return NULL;
}

}
/* EOF */