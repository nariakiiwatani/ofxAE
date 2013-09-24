#pragma once

#include "ofxAEAVLayer.h"
#include "ofPath.h"
#include "ofxAEPath.h"

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
class ShapeContentPath : public ShapeContentShape
{
public:
	ShapeContentPath();
	void push(ofPath& path);
	Path& getPath() { return path_; }
private:
	Path path_;
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
class ShapeContentRect : public ShapeContentShape
{
public:
	void push(ofPath& path);
	void setPosition(const ofVec2f& pos) { pos_.set(pos); }
	void setSize(const ofVec2f& size) { size_.set(size); }
	void setRoundness(float roundness) { roundness_=roundness; }
private:
	ofVec2f size_;
	ofVec2f pos_;
	float roundness_;
};
class ShapeContentPoly : public ShapeContentShape
{
public:
	void push(ofPath& path);
	void setIsStar(bool star) { is_star_=star; }
	void setCornerCount(int count) { corner_count_=count; }
	void setPosition(const ofVec2f& pos) { pos_.set(pos); }
	void setRotation(float rotation) { rotation_=rotation; }
	void setOuterRadius(float radius) { outer_radius_=radius; }
	void setOuterRoundness(float roundness) { outer_roundness_=roundness; }
	void setInnerRadius(float radius) { inner_radius_=radius; }
	void setInnerRoundness(float roundness) { inner_roundness_=roundness; }
private:
	bool is_star_;
	float corner_count_;
	ofVec2f pos_;
	float rotation_;
	float outer_radius_;
	float outer_roundness_;
	float inner_radius_;
	float inner_roundness_;
};
class ShapeContentGraphic : public ShapeContent
{
protected:
	ofBlendMode blend_mode_;
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
};
class ShapeContentFill : public ShapeContentGraphic
{
public:
	void pop(ofPath& path);
	void setColor(const ofFloatColor& color) { color_.set(color); }
	void setOpacity(float opacity) { opacity_=opacity; }
private:
	ofFloatColor color_;
	float opacity_;
};



}

/* EOF */