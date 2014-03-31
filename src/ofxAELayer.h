#pragma once

#include "ofxAEDef.h"
#include "TransformNode.h"
#include "ofxAEProperty.h"

OFX_AE_NAMESPACE_BEGIN

class Marker;
class LayerCap;

class Layer {
	friend class Loader;
public:
	Layer();
	void update();
	virtual void setPropertyFrame(int frame);
	void setParent(Layer *parent);
	Layer* getParent() { return parent_; }
	
	void addCap(LayerCap *cap);
	void removeCap(LayerCap *cap);
	
	bool isActive() { return active_; }
	float getOpacity() { return opacity_; }
	TransformNode& getNode() { return transform_; }

	void setActive(bool active) { active_=active; }
	void setOpacity(float opacity) { opacity_=opacity; }
	void setTranslation(const ofVec3f& translation) { transform_.setTranslation(translation); }
	void setRotation(const ofVec3f& rotation) { transform_.setRotation(rotation); }
	void setOrientation(const ofVec3f& orientation) { transform_.setOrientation(orientation); }
	void setScale(const ofVec3f& scale) { transform_.setScale(scale); }
	void setAnchorPoint(const ofVec3f& anchor_point) { transform_.setAnchorPoint(anchor_point); }
	
	const string& getName() { return name_; }
	
	void setFreeze(bool freeze) { transform_.setEnable(!freeze); }
		
	void addProperty(PropertyBase *property);

protected:
	string name_;
	vector<LayerCap*> cap_;

	Layer *parent_;
	Property<float> opacity_;
	Property<bool> active_;
	TransformProperty transform_;
	int frame_offset_, frame_in_, frame_out_;
	vector<Marker*> marker_;
	vector<PropertyBase*> properties_;
};

OFX_AE_NAMESPACE_END
/* EOF */