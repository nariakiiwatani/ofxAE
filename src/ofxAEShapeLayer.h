#pragma once

#include "ofxAEAVLayer.h"
#include "ofPath.h"

namespace ofxAE {

class ShapeContent;
class ShapeLayer : public AVLayer
{
	friend class Loader;
public:
	ShapeLayer();
private:
	void prepare();
	void render();
private:
	ofPath path_;
	vector<ShapeContent*> content_;
};

class ShapeContent
{
public:
	virtual void push(ofPath& path){};
	virtual void pop(ofPath& path){};
};

class ShapeContentGroup : public ShapeContent
{
	friend class Loader;
public:
	void push(ofPath& path);
	void pop(ofPath& path);
	void setPosition(const ofVec2f& position) { trans_.setTranslation(position); }
	void setRotation(float z) { trans_.setRotationZ(z); }
	void setScale(const ofVec2f& scale) { trans_.setScale(scale.x,scale.y,1); }
	void setAnchorPoint(const ofVec2f& anchor) { trans_.setAnchorPoint(anchor); }
	void setOpacity(float opacity) { opacity_ = opacity; }
	void setSkew(float skew) { skew_ = skew; }	// not supoprted
	void setSkewAxis(float axis) { skew_axis_ = axis; }	// not supported
private:
	vector<ShapeContent*> content_;
	TransformNode trans_;
	float opacity_;
	float skew_;
	float skew_axis_;
};
class ShapeContentShape : public ShapeContent
{
public:
	void pop(ofPath& path);
protected:
	int command_count_;
};
class ShapeContentEllipse : public ShapeContentShape
{
public:
	void push(ofPath& path);
	void setPosition(const ofVec2f& pos) { pos_.set(pos); }
	void setSize(const ofVec2f& size) { size_.set(size); }
private:
	ofVec2f size_;
	ofVec2f pos_;
};
class ShapeContentGraphic : public ShapeContent
{
};
class ShapeContentStroke : public ShapeContentGraphic
{
public:
	void pop(ofPath& path);
	void setColor(const ofFloatColor& color) { color_.set(color); }
	void setOpacity(float opacity) { opacity_=opacity; }
	void setWidth(float width) { width_=width; }
private:
	ofFloatColor color_;
	float opacity_;
	float width_;
};}

/* EOF */