#pragma once

#include "ofxAEDef.h"
#include "TransformNode.h"
#include "ofxAEProperty.h"

OFX_AE_NAMESPACE_BEGIN

class Marker;
class LayerCap;

class Layer : public std::enable_shared_from_this<Layer>, public ofxAE::PropertyGroup {
	friend class Loader;
public:
	Layer();
	void update();
	virtual void setPropertyFrame(int frame);
	void setParent(std::shared_ptr<Layer> parent);
	std::shared_ptr<Layer> getParent() { return parent_.lock(); }
	
	void setCap(std::shared_ptr<LayerCap> cap);
	std::shared_ptr<LayerCap> getCap() { return cap_; }
	
	bool isActive() { return is_active_; }
	float getOpacity() { return opacity_; }
	TransformNode& getNode() { return transform_; }
	const string& getName() { return name_; }
	int getFrame() { return frame_; }

	void setActive(bool active);
	void setOpacity(float opacity) { opacity_=opacity; }
	void setTranslation(const ofVec3f& translation) { transform_.setTranslation(translation); }
	void setRotation(const ofVec3f& rotation) { transform_.setRotation(rotation); }
	void setOrientation(const ofVec3f& orientation) { transform_.setOrientation(orientation); }
	void setScale(const ofVec3f& scale) { transform_.setScale(scale); }
	void setAnchorPoint(const ofVec3f& anchor_point) { transform_.setAnchorPoint(anchor_point); }
	
	std::shared_ptr<Property<float>> getOpacityProperty() { return getProperty<float>("opacity"); }
	std::shared_ptr<Property<bool>> getActiveProperty() { return getProperty<bool>("active"); }
	std::shared_ptr<TransformProperty> getTransformProperty() { return get<TransformProperty>("transform"); }
	
	void setParamByComment(const std::string &comment);
	const std::string& getParam(const std::string &key);
	
	std::shared_ptr<Marker> addNewMarker();
	void addMarker(std::shared_ptr<Marker> marker);
protected:
	std::string name_;
	std::weak_ptr<Layer> parent_;
	std::shared_ptr<LayerCap> cap_;
	int frame_=0;

	float opacity_=1;
	bool is_active_=true;
	TransformNode transform_;
	int frame_offset_, frame_in_, frame_out_;
	std::vector<std::shared_ptr<Marker>> marker_;
	std::map<std::string, std::string> param_;
	
private:
	void setActiveCallback(const bool &active) { setActive(active); }
};

OFX_AE_NAMESPACE_END
/* EOF */
