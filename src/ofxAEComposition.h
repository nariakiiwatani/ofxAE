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
	Composition();
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
	void setFrameRate(float rate) { frame_rate_=rate; }
	bool isEnd() { return frame_.isEnd(); }
	float getWidth() { return width_; }
	float getHeight() { return height_; }
	int getLength() { return frame_default_.getLength(); }
	int getFrame() { return frame_.getCurrent(); }
	float getFrameRate() { return frame_rate_; }
	void setEnableTimeUpdating(bool time_updating) { is_time_updating_=time_updating; }
	float getSpeed() { return frame_.getSpeed(); }
	
	void addAVLayer(std::shared_ptr<AVLayer> layer);
	int getNumAVLayer();
	vector<std::shared_ptr<AVLayer>>& getAVLayers();
	std::shared_ptr<AVLayer> getAVLayer(int index);
	std::shared_ptr<AVLayer> getAVLayer(const string& name);
	vector<std::shared_ptr<AVLayer>> getAVLayers(const string& name);
	
	void addCameraLayer(std::shared_ptr<CameraLayer> layer);
	int getNumCameraLayer();
	vector<std::shared_ptr<CameraLayer>>& getCameraLayers();
	std::shared_ptr<CameraLayer> getCameraLayer(int index);
	std::shared_ptr<CameraLayer> getCameraLayer(const string& name);
	vector<std::shared_ptr<CameraLayer>> getCameraLayers(const string& name);
	
	void setActiveMarker(int index, float speed=1);
	void setActiveMarker(const string& name, float speed=1);
	void setActiveMarker(Marker *marker, float speed=1);
	void clearActiveMarker(bool reset_frame=false);
	
	bool isDuringMarker(int index);
	bool isDuringMarker(const string& name);
	bool isDuringMarker(Marker *marker);
	
	bool isMarkerStartFrame(int index);				// duplicated. use isMarkerBegin
	bool isMarkerStartFrame(const string& name);	// duplicated. use isMarkerBegin
	bool isMarkerStartFrame(Marker *marker);		// duplicated. use isMarkerBegin
	
	bool isMarkerEndFrame(int index);				// duplicated. use isMarkerEnd
	bool isMarkerEndFrame(const string& name);		// duplicated. use isMarkerEnd
	bool isMarkerEndFrame(Marker *marker);			// duplicated. use isMarkerEnd

	bool isMarkerBegin(int index);
	bool isMarkerBegin(const string &name);
	bool isMarkerBegin(Marker *marker);
	
	bool isMarkerEnd(int index);
	bool isMarkerEnd(const string &name);
	bool isMarkerEnd(Marker *marker);
	
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
	int getMarkerIndex(const string &name);
	int getMarkerIndex(Marker *marker);
	
	void addMarker(Marker *marker);
	
private:
	string name_;
	float width_;
	float height_;
	int frame_rate_;
	bool is_time_updating_;
	vector<std::shared_ptr<AVLayer>> av_;
	vector<std::shared_ptr<CameraLayer>> camera_;
	vector<std::shared_ptr<AVLayer>> active_layers_;
	std::shared_ptr<CameraLayer> active_camera_;
	struct MarkerWrapper {
		bool is_in, is_in_prev;
		Marker *ptr;
		MarkerWrapper(Marker *marker):is_in(false),is_in_prev(false),ptr(marker) {}
	};
	vector<MarkerWrapper> marker_;
	Marker *active_marker_;
	
	FrameCounter frame_;
	FrameCounter frame_default_;
};

OFX_AE_NAMESPACE_END
/* EOF */
