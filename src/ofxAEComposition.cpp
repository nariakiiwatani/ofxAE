#include "ofxAEComposition.h"
#include "ofxAEAVLayer.h"
#include "ofGraphics.h"
#include "ofxAECameraLayer.h"
#include "ofxAEMarker.h"

OFX_AE_NAMESPACE_BEGIN

Composition::Composition()
:width_(0)
,height_(0)
,frame_rate_(ofGetTargetFrameRate())
,is_time_updating_(false)
{
}
void Composition::allocate(int width, int height)
{
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
bool Composition::isForward()
{
	return frame_.isForward();
}
bool Composition::isBackward()
{
	return frame_.isBackward();
}

void Composition::update()
{
	bool use_time = is_time_updating_||ofGetTargetFrameRate()==0;
	int frame = frame_.update(use_time?ofGetLastFrameTime()*frame_rate_:frame_rate_/ofGetTargetFrameRate());

	active_layers_.clear();
	for(vector<AVLayer*>::iterator layer = av_.begin(); layer != av_.end(); ++layer) {
		AVLayer *l = *layer;
		l->setPropertyFrame(frame);
		if(l->isActive()) {
			l->update();
			active_layers_.push_back(l);
		}
	}

	active_camera_ = NULL;
	for(vector<CameraLayer*>::iterator camera = camera_.begin(); camera != camera_.end(); ++camera) {
		CameraLayer *c = *camera;
		c->setPropertyFrame(frame);
		if(c->isActive()) {
			c->update();
			if(!active_camera_) {
				active_camera_ = c;
			}
		}
	}
	if(active_camera_) {
		active_camera_->prepare();
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

void Composition::draw(float alpha)
{
	draw(active_camera_?active_camera_->getCamera():NULL, alpha);
}

void Composition::draw(ofCamera *camera, float alpha)
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
					(*w).second->draw(alpha);
				}
				work.clear();
				if(camera) {
					camera->end();
				}
			}
			l->draw(alpha);
		}
	}
	if(!work.empty()) {
		if(camera) {
			camera->begin();
		}
		for(multimap<float,AVLayer*>::iterator w = work.begin(); w != work.end(); ++w) {
			(*w).second->draw(alpha);
		}
		work.clear();
		if(camera) {
			camera->end();
		}
	}
}

void Composition::setFrame(int frame)
{
	frame_.resetFrame(frame);
	update();
}

void Composition::resetFrame(int frame)
{
	frame_.resetFrame(frame);
}
	
void Composition::setFrameByRatio(float ratio)
{
	setFrame(ofMap(ratio, 0, 1, 0, frame_.getLength()-1));
}
	
void Composition::resetFrameByRatio(float ratio)
{
	resetFrame(ofMap(ratio, 0, 1, 0, frame_.getLength()-1));
}

void Composition::addAVLayer(AVLayer *layer)
{
	av_.push_back(layer);
}
int Composition::getNumAVLayer()
{
	return av_.size();
}
vector<AVLayer*>& Composition::getAVLayers()
{
	return av_;
}
AVLayer* Composition::getAVLayer(int index)
{
	return av_[index];
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

vector<AVLayer*> Composition::getAVLayers(const string& name)
{
	vector<AVLayer*> ret;
	for(vector<AVLayer*>::iterator it = av_.begin(); it != av_.end(); ++it) {
		if((*it)->getName() == name) {
			ret.push_back(*it);
		}
	}
	return ret;
}


void Composition::addCameraLayer(CameraLayer *layer)
{
	camera_.push_back(layer);
}
int Composition::getNumCameraLayer()
{
	return camera_.size();
}
vector<CameraLayer*>& Composition::getCameraLayers()
{
	return camera_;
}
CameraLayer* Composition::getCameraLayer(int index)
{
	return camera_[index];
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

vector<CameraLayer*> Composition::getCameraLayers(const string& name)
{
	vector<CameraLayer*> ret;
	for(vector<CameraLayer*>::iterator it = camera_.begin(); it != camera_.end(); ++it) {
		if((*it)->getName() == name) {
			ret.push_back(*it);
		}
	}
	return ret;
}

OFX_AE_NAMESPACE_END
/* EOF */