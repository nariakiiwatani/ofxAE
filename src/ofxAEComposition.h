#pragma once

#include "ofxAEDef.h"
#include "ofConstants.h"
#include "ofFbo.h"
#include "FrameCounter.h"

class ofCamera;

OFX_AE_NAMESPACE_BEGIN

class AVLayer;
class CameraLayer;
class Marker;

class Composition {
	friend class Loader;
public:
	~Composition();
	void allocate(int width, int height);
	void setLength(int length);
	void setLoopState(FrameCounter::LoopState loop);
	void setSpeed(float speed);
	void setBackward(bool backward);
	void update();
	void draw();
	void drawCollapse(ofCamera *camera);
	void setFrame(int frame);
	void resetFrame(int frame);
	void setFrameByRatio(float ratio);
	void resetFrameByRatio(float ratio);
	bool isEnd() { return frame_.isEnd(); }
	float getWidth() { return width_; }
	float getHeight() { return height_; }
	int getLength() { return frame_default_.getLength(); }
	int getFrame() { return frame_.getCurrent(); }
	
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
	
	void setActiveMarker(int index, float speed=1);
	void setActiveMarker(const string& name, float speed=1);
	void setActiveMarker(Marker *marker, float speed=1);
	void clearActiveMarker(bool reset_frame=false);
	
	bool isDuringMarker(int index);
	bool isDuringMarker(const string& name);
	bool isDuringMarker(Marker *marker);
	
	bool isMarkerStartFrame(int index);
	bool isMarkerStartFrame(const string& name);
	bool isMarkerStartFrame(Marker *marker);
	
	bool isMarkerEndFrame(int index);
	bool isMarkerEndFrame(const string& name);
	bool isMarkerEndFrame(Marker *marker);
	
	bool isMarkerActive(int index);
	bool isMarkerActive(const string& name);
	bool isMarkerActive(Marker *marker);
	
	void jumpToMarkerStartFrame(int index);
	void jumpToMarkerStartFrame(const string& name);
	void jumpToMarkerStartFrame(Marker *marker);
	
	void jumpToMarkerEndFrame(int index);
	void jumpToMarkerEndFrame(const string& name);
	void jumpToMarkerEndFrame(Marker *marker);
	
	Marker* getMarker(int index);
	Marker* getMarker(const string& name);
	
private:
	ofFbo fbo_;
	string name_;
	float width_;
	float height_;
	vector<AVLayer*> av_;
	vector<CameraLayer*> camera_;
	vector<AVLayer*> active_layers_;
	vector<Marker*> marker_;
	Marker *active_marker_;
	
	FrameCounter frame_;
	FrameCounter frame_default_;
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

OFX_AE_NAMESPACE_END
/* EOF */