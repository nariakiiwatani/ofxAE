#include "ofxAEComposition.h"
#include "ofxAEAVLayer.h"
#include "ofGraphics.h"
#include "ofxAECameraLayer.h"
#include "ofxAEMarker.h"

namespace ofxAE {

Composition::~Composition()
{
	for(vector<AVLayer*>::iterator it = av_.begin(); it != av_.end(); ++it) {
		delete *it;
	}
	for(vector<CameraLayer*>::iterator it = camera_.begin(); it != camera_.end(); ++it) {
		delete *it;
	}
	for(vector<Marker*>::iterator it = marker_.begin(); it != marker_.end(); ++it) {
		delete *it;
	}
}

void Composition::allocate(int width, int height)
{
	fbo_.allocate(width, height, GL_RGBA);
	width_ = width;
	height_ = height;
}
void Composition::setLength(int length)
{
	frame_.setRange(0, length);
	length_default_ = length;
}

void Composition::update()
{
	int frame = frame_.update();
	if(frame_.isEnd()) {
		clearActiveMarker();
	}
	if(frame >= 0) {
		setPropertyFrame(frame);
		prepare();
	}
}

void Composition::setActiveMarker(int index, float speed)
{
	if(0 <= index && index < marker_.size()) {
		setActiveMarker(marker_[index], speed);
	}
}
void Composition::setActiveMarker(const string& name, float speed)
{
	for(vector<Marker*>::iterator marker = marker_.begin(); marker != marker_.end(); ++marker) {
		if((*marker)->getName() == name) {
			setActiveMarker(*marker, speed);
			return;
		}
	}
}
void Composition::setActiveMarker(Marker *marker, float speed)
{
	int from = marker->getFrom();
	int length = marker->getLength();
	frame_.setSpeed(speed);
	frame_.setRange(from, length);
	frame_.setLoopState(FrameCounter::NONE);
	const string& loop = marker->getParam("loop");
	if(loop != "") {
		if(loop == "oneway") {
			frame_.setLoopState(FrameCounter::ONEWAY);
		}
		else if(loop == "pingpong") {
			frame_.setLoopState(FrameCounter::PINGPONG);
		}
	}
	if(speed >= 0) {
		frame_.resetFrame(from);
	}
	else {
		frame_.resetFrame(from+length-1);
	}
	active_marker_ = marker;
}
void Composition::clearActiveMarker()
{
	frame_.setSpeed(1);
	frame_.setRange(0, length_default_);
	frame_.setLoopState(FrameCounter::ONEWAY);
	active_marker_ = NULL;
}

void Composition::prepare()
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
	vector<AVLayer*> active;
	for(vector<AVLayer*>::iterator layer = av_.begin(); layer != av_.end(); ++layer) {
		AVLayer *l = *layer;
		if(l->isActive()) {
			l->update();
			active.push_back(l);
		}
	}
	fbo_.begin();
	ofClear(0);
	multimap<float,AVLayer*> work;
	for(vector<AVLayer*>::iterator layer = active.begin(); layer != active.end(); ++layer) {
		AVLayer *l = *layer;
		if(l->is3D()) {
			ofVec3f dist = (*layer)->getNode().getWorldMatrix()->getTranslation();
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
	setPropertyFrame(frame);
	prepare();
	frame_.setFrame(frame);
}

void Composition::resetFrame(int frame)
{
	setPropertyFrame(frame);
	prepare();
	frame_.resetFrame(frame);
}

void Composition::setPropertyFrame(int frame)
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