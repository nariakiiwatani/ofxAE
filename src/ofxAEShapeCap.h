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
	void setLayer(std::shared_ptr<AVLayer> layer);
	void update();
	void draw(float alpha=1);
	void addContent(std::shared_ptr<ShapeContent> content);
private:
	ofPath path_;
	std::vector<std::shared_ptr<ShapeContent>> content_;

	std::shared_ptr<PropertyGroup> shape_prop_;
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
	
	void addContent(std::shared_ptr<ShapeContent> content);
	
	std::shared_ptr<TransformProperty> getTransformProperty() { return get<TransformProperty>("transform"); }
	std::shared_ptr<Property<float>> getRotationZProperty() { return getProperty<float>("rotation z"); }
	std::shared_ptr<Property<float>> getOpacityProperty() { return getProperty<float>("opacity"); }
	std::shared_ptr<Property<float>> getSkewProperty() { return getProperty<float>("skew"); }
	std::shared_ptr<Property<float>> getSkewAxisProperty() { return getProperty<float>("skew axis"); }
	
	void setPosition(const ofVec2f& position);
	void setRotation(float z);
	void setScale(const ofVec2f& scale);
	void setAnchorPoint(const ofVec2f& anchor);
	void setOpacity(float opacity) { opacity_ = opacity; }
	void setSkew(float skew) { skew_ = skew; }	// not supoprted
	void setSkewAxis(float axis) { skew_axis_ = axis; }	// not supported
private:
	std::vector<std::shared_ptr<ShapeContent>> content_;
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
	std::shared_ptr<PathProperty> getPathProperty() { return get<PathProperty>("path"); }
private:
	ofPath path_;
};

class ShapeContentEllipse : public ShapeContentShape
{
	friend class Loader;
public:
	ShapeContentEllipse();
	void push(ofPath& path);
	void setSize(const ofVec2f& size) { size_=size; }
	void setPosition(const ofVec2f& pos) { pos_=pos; }
	
	std::shared_ptr<Property<ofVec2f>> getPositionProperty() { return getProperty<ofVec2f>("position"); }
	std::shared_ptr<Property<ofVec2f>> getSizeProperty() { return getProperty<ofVec2f>("size"); }
private:
	ofVec2f size_;
	ofVec2f pos_;
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

	std::shared_ptr<Property<ofVec2f>> getSizeProperty() { return getProperty<ofVec2f>("size"); }
	std::shared_ptr<Property<ofVec2f>> getPositionProperty() { return getProperty<ofVec2f>("position"); }
	std::shared_ptr<Property<float>> getRoundnessProperty() { return getProperty<float>("roundness"); }
private:
	ofVec2f size_;
	ofVec2f pos_;
	float roundness_;
};

class ShapeContentPoly : public ShapeContentShape
{
	friend class Loader;
public:
	ShapeContentPoly();
	void push(ofPath& path);
	void setIsStar(bool star) { is_star_=star; }
	void setCornerCount(float count) { corner_count_=count; }
	void setPosition(const ofVec2f& pos) { pos_=pos; }
	void setRotation(float rotation) { rotation_=rotation; }
	void setOuterRadius(float radius) { outer_radius_=radius; }
	void setOuterRoundness(float roundness) { outer_roundness_=roundness; }
	void setInnerRadius(float radius) { inner_radius_=radius; }
	void setInnerRoundness(float roundness) { inner_roundness_=roundness; }
	
	std::shared_ptr<Property<float>> getStarProperty() { return getProperty<float>("star"); }
	std::shared_ptr<Property<float>> addCornerCountProperty() { return getProperty<float>("corner count"); }
	std::shared_ptr<Property<ofVec2f>> addPositionProperty() { return getProperty<ofVec2f>("position"); }
	std::shared_ptr<Property<float>> addRotationProperty() { return getProperty<float>("rotation"); }
	std::shared_ptr<Property<float>> addOuterRadiusProperty() { return getProperty<float>("outer radius"); }
	std::shared_ptr<Property<float>> addOuterRoundnessProperty() { return getProperty<float>("outer roundness"); }
	std::shared_ptr<Property<float>> addInnerRadiusProperty() { return getProperty<float>("inner radius"); }
	std::shared_ptr<Property<float>> addInnerRoundnessProperty() { return getProperty<float>("inner roundness"); }

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
	void setStrokeWidth(float width) { stroke_width_=width; }

	std::shared_ptr<Property<ofFloatColor>> getColorProperty() { return getProperty<ofFloatColor>("color"); }
	std::shared_ptr<Property<float>> getOpacityProperty() { return getProperty<float>("opacity"); }
	std::shared_ptr<Property<float>> getStrokeWidthProperty() { return getProperty<float>("stroke width"); }
private:
	ofFloatColor color_;
	float opacity_;
	float stroke_width_;
};

class ShapeContentFill : public ShapeContentGraphic
{
	friend class Loader;
public:
	ShapeContentFill();
	void pop(ofPath& path);
	void setColor(const ofFloatColor& color) { color_=color; }
	void setOpacity(float opacity) { opacity_=opacity; }
	
	std::shared_ptr<Property<ofFloatColor>> getColorProperty() { return getProperty<ofFloatColor>("color"); }
	std::shared_ptr<Property<float>> getOpacityProperty() { return getProperty<float>("opacity"); }
private:
	ofFloatColor color_;
	float opacity_;
};

OFX_AE_NAMESPACE_END
/* EOF */
