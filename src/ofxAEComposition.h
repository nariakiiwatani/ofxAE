#pragma once

#include "ofConstants.h"
#include "ofFbo.h"

namespace ofxAE {
	class AVLayer;
	class CameraLayer;
	class Marker;
}

namespace ofxAE {
class CompositionFrameCounter {
public:
	CompositionFrameCounter():frame_(0),speed_(1),loop_(ONEWAY){}
	int update() {
		frame_ += speed_;
		if(speed_ < 0 && frame_ < 0) {
			switch(loop_) {
				case NONE:
					return -1;
				case ONEWAY:
					frame_ += length_;
					break;
				case PINGPONG:
					frame_ = -frame_;
					speed_ = -speed_;
					break;
			}
		}
		if(speed_ > 0 && frame_ >= length_) {
			switch(loop_) {
				case NONE:
					return -1;
				case ONEWAY:
					frame_ -= length_;
					break;
				case PINGPONG:
					frame_ = 2*length_-frame_;
					speed_ = -speed_;
					break;
			}
		}
		return frame_;
	}
	enum LoopState {
		NONE,
		ONEWAY,
		PINGPONG,
	};
	void setLoopState(LoopState state) { loop_ = state; }
	void setLength(int length) { length_ = length; }
	void setSpeed(float speed) { speed_ = speed; }
	void setFrame(int frame) { frame_ = frame; }
private:
	float frame_;
	int length_;
	LoopState loop_;
	float speed_;
};
class Composition {
	friend class Loader;
public:
	~Composition();
	void allocate(int width, int height);
	void setLength(int length);
	void update();
	void draw();
	void setFrame(int frame);
	float getWidth() { return width_; }
	float getHeight() { return height_; }
	
	void addAVLayer(AVLayer *layer);
	int getNumAVLayer();
	vector<AVLayer*>& getAVLayers();
	AVLayer *getAVLayer(int index);
	AVLayer *getAVLayer(const string& name);
	
	void addCameraLayer(CameraLayer *layer);
	int getNumCameraLayer();
	vector<CameraLayer*>& getCameraLayers();
	CameraLayer *getCameraLayer(int index);
	CameraLayer *getCameraLayer(const string& name);
	
private:
	ofFbo fbo_;
	string name_;
	float width_;
	float height_;
	vector<AVLayer*> av_;
	vector<CameraLayer*> camera_;
	vector<Marker*> marker_;
	
	CompositionFrameCounter frame_;
private:
	void setPropertyFrame(int frame);
	void prepare();
};
	
	
inline void Composition::addAVLayer(AVLayer *layer)
{
	av_.push_back(layer);
}
inline int Composition::getNumAVLayer()
{
	return av_.size();
}
inline vector<AVLayer*>& Composition::getAVLayers()
{
	return av_;
}
inline AVLayer* Composition::getAVLayer(int index)
{
#ifdef _DEBUG
	if(index < 0 || index <= av_.size()) {
		return NULL;
	}
#endif
	return av_[index];
}


inline void Composition::addCameraLayer(CameraLayer *layer)
{
	camera_.push_back(layer);
}
inline int Composition::getNumCameraLayer()
{
	return camera_.size();
}
inline vector<CameraLayer*>& Composition::getCameraLayers()
{
	return camera_;
}
inline CameraLayer* Composition::getCameraLayer(int index)
{
#ifdef _DEBUG
	if(index < 0 || index <= camera_.size()) {
		return NULL;
	}
#endif
	return camera_[index];
}
}

/* EOF */