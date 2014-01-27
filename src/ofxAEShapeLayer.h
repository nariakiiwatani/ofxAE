#pragma once

#include "ofxAEDef.h"
#include "ofxAEAVLayer.h"
#include "ofPath.h"
#include "ofxAEProperty.h"

OFX_AE_NAMESPACE_BEGIN

class ShapeContent;
class ShapeLayer : public AVLayer
{
	friend class Loader;
public:
	ShapeLayer();
private:
	void prepare();
	void render(float alpha=1);
	void addContent(ShapeContent *content);
private:
	ofPath path_;
	vector<ShapeContent*> content_;
};

class ShapeContent : public PropertyGroup
{
	friend class Loader;
public:
	virtual void push(ofPath& path){};
	virtual void pop(ofPath& path){};
};

class ShapeContentGroup : public ShapeContent
{
	friend class Loader;
public:
	ShapeContentGroup();
	void push(ofPath& path);
	void pop(ofPath& path);
	
	void addContent(ShapeContent *content);
	
	void setPosition(const ofVec2f& position);
	void setRotation(float z);
	void setScale(const ofVec2f& scale);
	void setAnchorPoint(const ofVec2f& anchor);
	void setOpacity(float opacity) { opacity_ = opacity; }
	void setSkew(float skew) { skew_ = skew; }	// not supoprted
	void setSkewAxis(float axis) { skew_axis_ = axis; }	// not supported
private:
	vector<ShapeContent*> content_;
	TransformProperty transform_;
	Property<float> rotation_z_;
	Property<float> opacity_;
	Property<float> skew_;
	Property<float> skew_axis_;
private:
	void prepare();
};
class ShapeContentShape : public ShapeContent
{
	friend class Loader;
public:
	void pop(ofPath& path);
protected:
	int command_count_;
};
class ShapeContentPath : public ShapeContentShape
{
	friend class Loader;
public:
	ShapeContentPath();
	void push(ofPath& path);
	PathProperty& getPath() { return path_; }
	void setSize(const ofVec2f& size) { path_.setSize(size); }
private:
	PathProperty path_;
};

class ShapeContentEllipse : public ShapeContentShape
{
	friend class Loader;
public:
	ShapeContentEllipse();
	void push(ofPath& path);
	void setSize(const ofVec2f& size) { size_=size; }
	void setPosition(const ofVec2f& pos) { pos_=pos; }
private:
	Property<ofVec2f> size_;
	Property<ofVec2f> pos_;
};
class ShapeContentRect : public ShapeContentShape
{
	friend class Loader;
public:
	ShapeContentRect();
	void push(ofPath& path);
	void setSize(const ofVec2f& size) { size_=size; }
	void setPosition(const ofVec2f& pos) { pos_=pos; }
	void setRoundness(float roundness) { roundness_=roundness; }
private:
	Property<ofVec2f> size_;
	Property<ofVec2f> pos_;
	Property<float> roundness_;
};
class ShapeContentPoly : public ShapeContentShape
{
	friend class Loader;
public:
	ShapeContentPoly();
	void push(ofPath& path);
	void setIsStar(bool star) { is_star_=star; }
	void setCornerCount(int count) { corner_count_=count; }
	void setPosition(const ofVec2f& pos) { pos_=pos; }
	void setRotation(float rotation) { rotation_=rotation; }
	void setOuterRadius(float radius) { outer_radius_=radius; }
	void setOuterRoundness(float roundness) { outer_roundness_=roundness; }
	void setInnerRadius(float radius) { inner_radius_=radius; }
	void setInnerRoundness(float roundness) { inner_roundness_=roundness; }
private:
	bool is_star_;
	Property<float> corner_count_;
	Property<ofVec2f> pos_;
	Property<float> rotation_;
	Property<float> outer_radius_;
	Property<float> outer_roundness_;
	Property<float> inner_radius_;
	Property<float> inner_roundness_;
};
class ShapeContentGraphic : public ShapeContent
{
	friend class Loader;
protected:
	ofBlendMode blend_mode_;
};
class ShapeContentStroke : public ShapeContentGraphic
{
	friend class Loader;
public:
	ShapeContentStroke();
	void pop(ofPath& path);
	void setColor(const ofFloatColor& color) { color_=color; }
	void setOpacity(float opacity) { opacity_=opacity; }
	void setWidth(float width) { width_=width; }
private:
	Property<ofFloatColor> color_;
	Property<float> opacity_;
	Property<float> width_;
};
class ShapeContentFill : public ShapeContentGraphic
{
	friend class Loader;
public:
	ShapeContentFill();
	void pop(ofPath& path);
	void setColor(const ofFloatColor& color) { color_=color; }
	void setOpacity(float opacity) { opacity_=opacity; }
private:
	Property<ofFloatColor> color_;
	Property<float> opacity_;
};

OFX_AE_NAMESPACE_END
/* EOF */