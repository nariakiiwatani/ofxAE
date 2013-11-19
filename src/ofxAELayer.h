#pragma once

#include "TransformNode.h"
#include "ofxAEProperty.h"

namespace ofxAE {
	class Marker;
class Layer {
	friend class Loader;
public:
	Layer();
	virtual ~Layer();
	void update();
	virtual void setPropertyFrame(int frame);
	void setParent(Layer *parent);
	
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

protected:
	virtual void prepare(){};
protected:
	string name_;
	Property<float> opacity_;
	Property<bool> active_;
	TransformProperty transform_;
	int start_frame_;
	vector<Marker*> marker_;
	vector<PropertyBase*> properties_;
};
}

/* EOF */