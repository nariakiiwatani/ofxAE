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
	for(auto &l : av_) {
		l->setPropertyFrame(frame);
		if(l->isActive()) {
			l->update();
			active_layers_.push_back(l);
		}
	}

	active_camera_.reset();
	for(auto &c : camera_) {
		c->setPropertyFrame(frame);
		if(c->isActive()) {
			c->update();
			if(!active_camera_.expired()) {
				active_camera_ = c;
			}
		}
	}
	if(auto c = active_camera_.lock()) {
		c->prepare();
	}
	for(auto &marker : marker_) {
		MarkerWork &w = marker.second;
		std::shared_ptr<Marker> m = marker.first;
		w.is_in_prev = w.is_in;
		int from = m->getFrom();
		int to = from + m->getLength()-1;
		w.is_in = (m->getFrom() <= frame) && (frame <= from + m->getLength()-1);
	}
}

void Composition::setActiveMarker(int index, float speed)
{
	std::shared_ptr<Marker> marker = getMarker(index);
	if(marker) {
		setActiveMarker(marker, speed);
	}
}
void Composition::setActiveMarker(const string& name, float speed)
{
	std::shared_ptr<Marker> marker = getMarker(name);
	if(marker) {
		setActiveMarker(marker, speed);
	}
}
void Composition::setActiveMarker(std::shared_ptr<Marker> marker, float speed)
{
	int from = marker->getFrom();
	int length = max(marker->getLength(), 1);
	frame_.setSpeed(speed);
	frame_.setRange(from, length);
	frame_.setLoopState(FrameCounter::LOOP_NONE);
	frame_.setLoopState(marker->getLoopState());
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
	active_marker_.reset();
}

bool Composition::isDuringMarker(int index)
{
	if(0 <= index && index < marker_.size()) {
		auto &w = marker_[index].second;
		return w.is_in;
	}
	return false;
}
bool Composition::isDuringMarker(const string& name)
{
	return isDuringMarker(getMarkerIndex(name));
}
bool Composition::isDuringMarker(std::shared_ptr<Marker> marker)
{
	return isDuringMarker(getMarkerIndex(marker));
}

bool Composition::isMarkerBegin(int index)
{
	if(0 <= index && index < marker_.size()) {
		auto &w = marker_[index].second;
		return !w.is_in_prev && w.is_in;
	}
	return false;
}
bool Composition::isMarkerBegin(const string& name)
{
	return isMarkerBegin(getMarkerIndex(name));
}
bool Composition::isMarkerBegin(std::shared_ptr<Marker> marker)
{
	return isMarkerBegin(getMarkerIndex(marker));
}

bool Composition::isMarkerEnd(int index)
{
	if(0 <= index && index < marker_.size()) {
		auto &w = marker_[index].second;
		return w.is_in_prev && !w.is_in;
	}
	return false;
}
bool Composition::isMarkerEnd(const string& name)
{
	return isMarkerEnd(getMarkerIndex(name));
}
bool Composition::isMarkerEnd(std::shared_ptr<Marker> marker)
{
	return isMarkerEnd(getMarkerIndex(marker));
}

bool Composition::isMarkerActive(int index)
{
	std::shared_ptr<Marker> marker = getMarker(index);
	return marker?isMarkerActive(marker):false;
}
bool Composition::isMarkerActive(const string& name)
{
	std::shared_ptr<Marker> marker = getMarker(name);
	return marker?isMarkerActive(marker):false;
}
bool Composition::isMarkerActive(std::shared_ptr<Marker> marker)
{
	return active_marker_.lock() == marker;
}

void Composition::jumpToMarkerStartFrame(int index)
{
	std::shared_ptr<Marker> marker = getMarker(index);
	if(marker) {
		jumpToMarkerStartFrame(marker);
	}
}
void Composition::jumpToMarkerStartFrame(const string& name)
{
	std::shared_ptr<Marker> marker = getMarker(name);
	if(marker) {
		jumpToMarkerStartFrame(marker);
	}
}
void Composition::jumpToMarkerStartFrame(std::shared_ptr<Marker> marker)
{
	int jump_to = marker->getFrom() - frame_.getFrom();
	frame_.resetFrame(jump_to);
}

void Composition::jumpToMarkerEndFrame(int index)
{
	std::shared_ptr<Marker> marker = getMarker(index);
	if(marker) {
		jumpToMarkerEndFrame(marker);
	}
}
void Composition::jumpToMarkerEndFrame(const string& name)
{
	std::shared_ptr<Marker> marker = getMarker(name);
	if(marker) {
		jumpToMarkerEndFrame(marker);
	}
}
void Composition::jumpToMarkerEndFrame(std::shared_ptr<Marker> marker)
{
	int jump_to = marker->getFrom()+marker->getLength() - frame_.getFrom();
	frame_.resetFrame(jump_to);
}


std::shared_ptr<Marker>  Composition::getMarker(int index)
{
	if(0 <= index && index < marker_.size()) {
		return marker_[index].first;
	}
	return nullptr;
}
std::shared_ptr<Marker>  Composition::getMarker(const string& name)
{
	return getMarker(getMarkerIndex(name));
}

int Composition::getMarkerIndex(const string &name)
{
	int size = marker_.size();
	for(int i = 0; i < size; ++i) {
		if(name == marker_[i].first->getName()) {
			return i;
		}
	}
	return -1;
}

int Composition::getMarkerIndex(std::shared_ptr<Marker> marker)
{
	int size = marker_.size();
	for(int i = 0; i < size; ++i) {
		if(marker == marker_[i].first) {
			return i;
		}
	}
	return -1;
}

void Composition::addMarker(std::shared_ptr<Marker> marker)
{
	marker_.push_back(std::make_pair(marker, MarkerWork()));
}

void Composition::draw(float alpha)
{
	auto camera = active_camera_.lock();
	draw(camera?camera->getCamera():nullptr, alpha);
}

void Composition::draw(ofCamera *camera, float alpha)
{
	multimap<float,std::shared_ptr<AVLayer>> work;
	for(auto &l : active_layers_) {
		if(l->getOpacity() == 0) {
			continue;
		}
		if(l->is3D()) {
			ofVec3f dist = l->getNode().getWorldMatrix()->getTranslation();
			if(camera) {
				dist = camera->worldToCamera(dist);
				dist.z = -dist.z;
			}
			work.insert(pair<float,std::shared_ptr<AVLayer>>(dist.z, l));
		}
		else {
			if(!work.empty()) {
				if(camera) {
					camera->begin();
				}
				for(auto &w : work) {
					w.second->draw(alpha);
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
		for(auto &w : work) {
			w.second->draw(alpha);
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

void Composition::addAVLayer(std::shared_ptr<AVLayer> layer)
{
	av_.push_back(layer);
}
int Composition::getNumAVLayer()
{
	return av_.size();
}
vector<std::shared_ptr<AVLayer>>& Composition::getAVLayers()
{
	return av_;
}
std::shared_ptr<AVLayer> Composition::getAVLayer(int index)
{
	return av_[index];
}

std::shared_ptr<AVLayer> Composition::getAVLayer(const string& name)
{
	for(auto &l : av_) {
		if(l->getName() == name) {
			return l;
		}
	}
	return nullptr;
}

vector<std::shared_ptr<AVLayer>> Composition::getAVLayers(const string& name)
{
	vector<std::shared_ptr<AVLayer>> ret;
	for(auto &l :av_) {
		if(l->getName() == name) {
			ret.push_back(l);
		}
	}
	return ret;
}


void Composition::addCameraLayer(std::shared_ptr<CameraLayer> layer)
{
	camera_.push_back(layer);
}
int Composition::getNumCameraLayer()
{
	return camera_.size();
}
vector<std::shared_ptr<CameraLayer>>& Composition::getCameraLayers()
{
	return camera_;
}
std::shared_ptr<CameraLayer> Composition::getCameraLayer(int index)
{
	return camera_[index];
}

std::shared_ptr<CameraLayer> Composition::getCameraLayer(const string& name)
{
	for(auto &c : camera_) {
		if(c->getName() == name) {
			return c;
		}
	}
	return nullptr;
}

vector<std::shared_ptr<CameraLayer>> Composition::getCameraLayers(const string& name)
{
	vector<std::shared_ptr<CameraLayer>> ret;
	for(auto &c : camera_) {
		if(c->getName() == name) {
			ret.push_back(c);
		}
	}
	return ret;
}

OFX_AE_NAMESPACE_END
/* EOF */
