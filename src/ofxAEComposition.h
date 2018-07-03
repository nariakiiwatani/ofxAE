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
	std::vector<std::shared_ptr<AVLayer>>& getAVLayers();
	std::shared_ptr<AVLayer> getAVLayer(int index);
	std::shared_ptr<AVLayer> getAVLayer(const std::string& name);
	std::vector<std::shared_ptr<AVLayer>> getAVLayers(const std::string& name);
	
	void addCameraLayer(std::shared_ptr<CameraLayer> layer);
	int getNumCameraLayer();
	std::vector<std::shared_ptr<CameraLayer>>& getCameraLayers();
	std::shared_ptr<CameraLayer> getCameraLayer(int index);
	std::shared_ptr<CameraLayer> getCameraLayer(const std::string& name);
	std::vector<std::shared_ptr<CameraLayer>> getCameraLayers(const std::string& name);
	
	void setActiveMarker(int index, float speed=1);
	void setActiveMarker(const std::string& name, float speed=1);
	void setActiveMarker(std::shared_ptr<Marker> marker, float speed=1);
	void clearActiveMarker(bool reset_frame=false);
	
	bool isDuringMarker(int index);
	bool isDuringMarker(const std::string& name);
	bool isDuringMarker(std::shared_ptr<Marker> marker);
	
	bool isMarkerBegin(int index);
	bool isMarkerBegin(const std::string &name);
	bool isMarkerBegin(std::shared_ptr<Marker> marker);
	
	bool isMarkerEnd(int index);
	bool isMarkerEnd(const std::string &name);
	bool isMarkerEnd(std::shared_ptr<Marker> marker);
	
	bool isMarkerActive(int index);
	bool isMarkerActive(const std::string& name);
	bool isMarkerActive(std::shared_ptr<Marker> marker);
	bool isSetActiveMarker() { return !active_marker_.expired(); }
	
	void jumpToMarkerStartFrame(int index);
	void jumpToMarkerStartFrame(const std::string& name);
	void jumpToMarkerStartFrame(std::shared_ptr<Marker> marker);
	
	void jumpToMarkerEndFrame(int index);
	void jumpToMarkerEndFrame(const std::string& name);
	void jumpToMarkerEndFrame(std::shared_ptr<Marker> marker);
	
	std::shared_ptr<Marker> getMarker(int index);
	std::shared_ptr<Marker> getMarker(const std::string& name);
	int getMarkerIndex(const std::string &name);
	int getMarkerIndex(std::shared_ptr<Marker> marker);
	
	std::shared_ptr<Marker> addNewMarker();
	void addMarker(std::shared_ptr<Marker> marker);
	
private:
	string name_;
	float width_;
	float height_;
	int frame_rate_;
	bool is_time_updating_;
	std::vector<std::shared_ptr<AVLayer>> av_;
	std::vector<std::shared_ptr<CameraLayer>> camera_;
	std::vector<std::shared_ptr<AVLayer>> active_layers_;
	std::weak_ptr<CameraLayer> active_camera_;
	struct MarkerWork {
		bool is_in=false, is_in_prev=false;
	};
	using Markers = std::vector<std::pair<std::shared_ptr<Marker>, MarkerWork>>;
	Markers marker_;
	std::weak_ptr<Marker> active_marker_;
	
	FrameCounter frame_;
	FrameCounter frame_default_;
};

OFX_AE_NAMESPACE_END
/* EOF */
