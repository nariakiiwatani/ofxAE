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
	frame_default_.setRange(0, length);
}
void Composition::setLoopState(FrameCounter::LoopState loop)
{
	frame_.setLoopState(loop);
	frame_default_.setLoopState(loop);
}
void Composition::setSpeed(float speed)
{
	frame_.setSpeed(speed);
	frame_default_.setSpeed(speed);
}
void Composition::setBackward(bool backward)
{
	frame_.setBackward(backward);
	frame_default_.setBackward(backward);
}

void Composition::update()
{
	int frame = frame_.update();
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
	int length = max(marker->getLength(), 1);
	frame_.setSpeed(speed);
	frame_.setRange(from, length);
	frame_.setLoopState(FrameCounter::LOOP_NONE);
	const string& loop = marker->getParam("loop");
	if(loop != "") {
		if(loop == "oneway") {
			frame_.setLoopState(FrameCounter::LOOP_ONEWAY);
		}
		else if(loop == "pingpong") {
			frame_.setLoopState(FrameCounter::LOOP_PINGPONG);
		}
	}
	active_marker_ = marker;
	resetFrame(0);
}
void Composition::clearActiveMarker(bool reset_frame)
{
	if(reset_frame) {
		frame_ = frame_default_;
	}
	else {
		int current_frame = frame_.getCurrent();
		frame_ = frame_default_;
		frame_.setFrame(current_frame);
	}
	active_marker_ = NULL;
}

bool Composition::isDuringMarker(int index)
{
	Marker *marker = getMarker(index);
	return marker?isDuringMarker(marker):false;
}
bool Composition::isDuringMarker(const string& name)
{
	Marker *marker = getMarker(name);
	return marker?isDuringMarker(marker):false;
}
bool Composition::isDuringMarker(Marker *marker)
{
	int frame = frame_.getCurrent();
	int from = marker->getFrom();
	int to = from + marker->getLength()-1;
	return from <= frame && frame <= to;
}

bool Composition::isMarkerStartFrame(int index)
{
	Marker *marker = getMarker(index);
	return marker?isMarkerStartFrame(marker):false;
}
bool Composition::isMarkerStartFrame(const string& name)
{
	Marker *marker = getMarker(name);
	return marker?isMarkerStartFrame(marker):false;
}
bool Composition::isMarkerStartFrame(Marker *marker)
{
	return frame_.getCurrent() == marker->getFrom();
}

bool Composition::isMarkerEndFrame(int index)
{
	Marker *marker = getMarker(index);
	return marker?isMarkerEndFrame(marker):false;
}
bool Composition::isMarkerEndFrame(const string& name)
{
	Marker *marker = getMarker(name);
	return marker?isMarkerEndFrame(marker):false;
}
bool Composition::isMarkerEndFrame(Marker *marker)
{
	return frame_.getCurrent() == (marker->getFrom()+marker->getLength()-1);
}

bool Composition::isMarkerActive(int index)
{
	Marker *marker = getMarker(index);
	return marker?isMarkerActive(marker):false;
}
bool Composition::isMarkerActive(const string& name)
{
	Marker *marker = getMarker(name);
	return marker?isMarkerActive(marker):false;
}
bool Composition::isMarkerActive(Marker *marker)
{
	return active_marker_ == marker;
}

void Composition::jumpToMarkerStartFrame(int index)
{
	Marker *marker = getMarker(index);
	if(marker) {
		jumpToMarkerStartFrame(marker);
	}
}
void Composition::jumpToMarkerStartFrame(const string& name)
{
	Marker *marker = getMarker(name);
	if(marker) {
		jumpToMarkerStartFrame(marker);
	}
}
void Composition::jumpToMarkerStartFrame(Marker *marker)
{
	int jump_to = marker->getFrom() - frame_.getFrom();
	frame_.resetFrame(jump_to);
}

void Composition::jumpToMarkerEndFrame(int index)
{
	Marker *marker = getMarker(index);
	if(marker) {
		jumpToMarkerEndFrame(marker);
	}
}
void Composition::jumpToMarkerEndFrame(const string& name)
{
	Marker *marker = getMarker(name);
	if(marker) {
		jumpToMarkerEndFrame(marker);
	}
}
void Composition::jumpToMarkerEndFrame(Marker *marker)
{
	int jump_to = marker->getFrom()+marker->getLength() - frame_.getFrom();
	frame_.resetFrame(jump_to);
}


Marker* Composition::getMarker(int index)
{
	if(0 <= index && index < marker_.size()) {
		return marker_[index];
	}
	return NULL;
}
Marker* Composition::getMarker(const string& name)
{
	for(vector<Marker*>::iterator marker = marker_.begin(); marker != marker_.end(); ++marker) {
		if((*marker)->getName() == name) {
			return *marker;
		}
	}
	return NULL;
}

void Composition::prepare()
{
	active_layers_.clear();
	for(vector<AVLayer*>::iterator layer = av_.begin(); layer != av_.end(); ++layer) {
		AVLayer *l = *layer;
		if(l->isActive()) {
			l->update();
			active_layers_.push_back(l);
		}
	}
}

void Composition::draw()
{
	ofCamera *active_camera = NULL;
	for(vector<CameraLayer*>::iterator camera = camera_.begin(); camera != camera_.end(); ++camera) {
		if((*camera)->isActive()) {
			(*camera)->update();
			if(!active_camera) {
				active_camera = (*camera)->getCamera();
			}
		}
	}
	fbo_.begin();
	ofClear(0);
	drawCollapse(active_camera);
	fbo_.end();
	fbo_.draw(0,0,width_,height_);
}

void Composition::drawCollapse(ofCamera *camera)
{
	multimap<float,AVLayer*> work;
	for(vector<AVLayer*>::iterator layer = active_layers_.begin(); layer != active_layers_.end(); ++layer) {
		AVLayer *l = *layer;
		if(l->getOpacity() == 0) {
			continue;
		}
		if(l->is3D()) {
			ofVec3f dist = (*layer)->getNode().getWorldMatrix()->getTranslation();
			if(camera) {
				dist = camera->worldToCamera(dist);
				dist.z = -dist.z;
			}
			work.insert(pair<float,AVLayer*>(dist.z, l));
		}
		else {
			if(!work.empty()) {
				if(camera) {
					camera->begin();
				}
				for(multimap<float,AVLayer*>::iterator w = work.begin(); w != work.end(); ++w) {
					(*w).second->draw();
				}
				work.clear();
				if(camera) {
					camera->end();
				}
			}
			l->draw();
		}
	}
	if(!work.empty()) {
		if(camera) {
			camera->begin();
		}
		for(multimap<float,AVLayer*>::iterator w = work.begin(); w != work.end(); ++w) {
			(*w).second->draw();
		}
		work.clear();
		if(camera) {
			camera->end();
		}
	}
}

void Composition::setFrame(int frame)
{
	frame_.setFrame(frame);
	update();
}

void Composition::resetFrame(int frame)
{
	frame_.resetFrame(frame);
}
	
void Composition::setFrameByRatio(float ratio)
{
	setFrame(ofMap(ratio, 0, 1, 0, getLength()-1));
}
	
void Composition::resetFrameByRatio(float ratio)
{
	resetFrame(ofMap(ratio, 0, 1, 0, getLength()-1));
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