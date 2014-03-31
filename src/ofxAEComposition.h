#pragma once

#include "ofxAEDef.h"
#include "ofConstants.h"
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
	bool isForward();
	bool isBackward();
	void update();
	void draw(float alpha=1);
	void draw(ofCamera *camera, float alpha=1);
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
	vector<AVLayer*> getAVLayers(const string& name);
	
	void addCameraLayer(CameraLayer *layer);
	int getNumCameraLayer();
	vector<CameraLayer*>& getCameraLayers();
	CameraLayer *getCameraLayer(int index);
	CameraLayer *getCameraLayer(const string& name);
	vector<CameraLayer*> getCameraLayers(const string& name);
	
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
	bool isSetActiveMarker() { return active_marker_!=NULL; }
	
	void jumpToMarkerStartFrame(int index);
	void jumpToMarkerStartFrame(const string& name);
	void jumpToMarkerStartFrame(Marker *marker);
	
	void jumpToMarkerEndFrame(int index);
	void jumpToMarkerEndFrame(const string& name);
	void jumpToMarkerEndFrame(Marker *marker);
	
	Marker* getMarker(int index);
	Marker* getMarker(const string& name);
	
private:
	string name_;
	float width_;
	float height_;
	vector<AVLayer*> av_;
	vector<CameraLayer*> camera_;
	vector<AVLayer*> active_layers_;
	CameraLayer *active_camera_;
	vector<Marker*> marker_;
	Marker *active_marker_;
	
	FrameCounter frame_;
	FrameCounter frame_default_;
};

OFX_AE_NAMESPACE_END
/* EOF */