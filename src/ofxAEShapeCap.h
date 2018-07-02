#pragma once

#include "ofxAEDef.h"
#include "ofxAEAVLayerCap.h"
#include "ofxAEProperty.h"
#include "ofPath.h"

OFX_AE_NAMESPACE_BEGIN

class ShapeContent;
class ShapeCap : public AVCap
{
public:
	ShapeCap();
	void update();
	void draw(float alpha=1);
	void addContent(ShapeContent *content);
private:
	ofPath path_;
	vector<ShapeContent*> content_;
};

class ShapeContent : public PropertyGroup
{
	friend class Loader;
public:
	ShapeContent(const string &name="content"):PropertyGroup(name){}
	virtual void push(ofPath& path){};
	virtual void pop(ofPath& path){};
};

class ShapeContentGroup : public ShapeContent
{
	friend class Loader;
public:
	ShapeContentGroup(const string &name="group"):ShapeContent(name){}

	void push(ofPath& path);
	void pop(ofPath& path);
	
	void addContent(ShapeContent *content);
	
	void addTransformProperty(TransformProperty *prop);
	void addRotationZProperty(Property<float> *prop);
	void addOpacityProperty(Property<float> *prop);
	void addSkewProperty(Property<float> *prop);
	void addSkewAxisProperty(Property<float> *prop);
	
	void setPosition(const ofVec2f& position);
	void setRotation(float z);
	void setScale(const ofVec2f& scale);
	void setAnchorPoint(const ofVec2f& anchor);
	void setOpacity(float opacity) { opacity_ = opacity; }
	void setSkew(float skew) { skew_ = skew; }	// not supoprted
	void setSkewAxis(float axis) { skew_axis_ = axis; }	// not supported
private:
	vector<ShapeContent*> content_;
	TransformNode transform_;
	float rotation_z_;
	float opacity_;
	float skew_;
	float skew_axis_;
private:
	void rotationZPropCallback(const float &val);
	void prepare();
};
class ShapeContentShape : public ShapeContent
{
	friend class Loader;
public:
	ShapeContentShape(const string &name="shape"):ShapeContent(name){}
	void pop(ofPath& path);
protected:
	int command_count_;
};

class ShapeContentPath : public ShapeContentShape
{
	friend class Loader;
public:
	ShapeContentPath(const string &name="path");
	void push(ofPath& path);
	void addPathProperty(PathProperty *prop);
private:
	ofPath path_;
};

class ShapeContentEllipse : public ShapeContentShape
{
	friend class Loader;
public:
	ShapeContentEllipse(const string &name="ellipse"):ShapeContentShape(name){}
	void push(ofPath& path);
	void setSize(const ofVec2f& size) { size_=size; }
	void setPosition(const ofVec2f& pos) { pos_=pos; }
	
	void addSizeProperty(Property<ofVec2f> *prop);
	void addPositionProperty(Property<ofVec2f> *prop);
private:
	ofVec2f size_;
	ofVec2f pos_;
};

class ShapeContentRect : public ShapeContentShape
{
	friend class Loader;
public:
	ShapeContentRect(const string &name="rect"):ShapeContentShape(name){}
	void push(ofPath& path);
	void setSize(const ofVec2f& size) { size_=size; }
	void setPosition(const ofVec2f& pos) { pos_=pos; }
	void setRoundness(float roundness) { roundness_=roundness; }

	void addSizeProperty(Property<ofVec2f> *prop);
	void addPositionProperty(Property<ofVec2f> *prop);
	void addRoundnessProperty(Property<float> *prop);
private:
	ofVec2f size_;
	ofVec2f pos_;
	float roundness_;
};

class ShapeContentPoly : public ShapeContentShape
{
	friend class Loader;
public:
	ShapeContentPoly(const string &name="poly");
	void push(ofPath& path);
	void setIsStar(bool star) { is_star_=star; }
	void setCornerCount(float count) { corner_count_=count; }
	void setPosition(const ofVec2f& pos) { pos_=pos; }
	void setRotation(float rotation) { rotation_=rotation; }
	void setOuterRadius(float radius) { outer_radius_=radius; }
	void setOuterRoundness(float roundness) { outer_roundness_=roundness; }
	void setInnerRadius(float radius) { inner_radius_=radius; }
	void setInnerRoundness(float roundness) { inner_roundness_=roundness; }
	
	void addStarProperty(Property<bool> *prop);
	void addCornerCountProperty(Property<float> *prop);
	void addPositionProperty(Property<ofVec2f> *prop);
	void addRotationProperty(Property<float> *prop);
	void addOuterRadiusProperty(Property<float> *prop);
	void addOuterRoundnessProperty(Property<float> *prop);
	void addInnerRadiusProperty(Property<float> *prop);
	void addInnerRoundnessProperty(Property<float> *prop);

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
	friend class Loader;
public:
	ShapeContentGraphic(const string &name="graphic"):ShapeContent(name){}
protected:
	ofBlendMode blend_mode_;
};

class ShapeContentStroke : public ShapeContentGraphic
{
	friend class Loader;
public:
	ShapeContentStroke(const string &name="stroke"):ShapeContentGraphic(name){}
	void pop(ofPath& path);
	void setColor(const ofFloatColor& color) { color_=color; }
	void setOpacity(float opacity) { opacity_=opacity; }
	void setStrokeWidth(float width) { stroke_width_=width; }

	void addColorProperty(Property<ofFloatColor> *prop);
	void addOpacityProperty(Property<float> *prop);
	void addStrokeWidthProperty(Property<float> *prop);
private:
	ofFloatColor color_;
	float opacity_;
	float stroke_width_;
};

class ShapeContentFill : public ShapeContentGraphic
{
	friend class Loader;
public:
	ShapeContentFill(const string &name="fill"):ShapeContentGraphic(name){}
	void pop(ofPath& path);
	void setColor(const ofFloatColor& color) { color_=color; }
	void setOpacity(float opacity) { opacity_=opacity; }
	
	void addColorProperty(Property<ofFloatColor> *prop);
	void addOpacityProperty(Property<float> *prop);
private:
	ofFloatColor color_;
	float opacity_;
};

OFX_AE_NAMESPACE_END
/* EOF */
