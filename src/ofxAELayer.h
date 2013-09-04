#pragma once

#include "TransformNode.h"
#include "ofxAEProperty.h"
#include "ofxAELayerHelper.h"

namespace ofxAE {
class Layer : public TransformNode {
	friend class Loader;
public:
	virtual ~Layer();
	virtual void update();
	void setPropertyFrame(int frame);
	void resetPropertyFrame();
	
	bool isActive() { return active_; }
	float getOpacity() { return opacity_; }

	void setActive(bool active) { active_=active; }
	void setOpacity(float opacity) { opacity_=opacity; }
	void setPosition(const ofVec3f& position) { setTranslation(position); }
	void setScale(const ofVec3f& scale) { TransformNode::setScale(scale); }
	void setRotation(const ofVec3f& rotation) { TransformNode::setRotation(rotation); }
	virtual void setAnchorPoint(const ofVec3f& anchor_point) { TransformNode::setAnchorPoint(anchor_point); }
	void setOrientation(const ofVec3f& orientation);

protected:
	string name_;
	float opacity_;
	bool active_;
	vector<Marker*> marker_;
	vector<LayerHelper_*> helper_;
	vector<PropertyBase_*> property_;
};
}

/* EOF */