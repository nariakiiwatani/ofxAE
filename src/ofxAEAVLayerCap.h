#pragma once

#include "ofxAEDef.h"
#include "ofxAELayerCap.h"
#include "ofVec2f.h"
#include "ofRectangle.h"

OFX_AE_NAMESPACE_BEGIN

class AVLayer;

class AVCap : public LayerCap
{
public:
	virtual void setLayer(std::shared_ptr<AVLayer> layer);
	void setLayer(std::shared_ptr<Layer> layer);
	
	void setCollapseTansformation(bool collapse) { is_collapse_=collapse; }
	void setOffsetX(float x) { rect_.x=x; }
	void setOffsetY(float y) { rect_.y=y; }
	void setOffset(float x, float y) { rect_.x=x;rect_.y=y; }
	void setOffset(const ofVec2f &offset) { rect_.x=offset.x;rect_.y=offset.y; }
	void setWidth(float width) { rect_.width=width; }
	void setHeight(float height) { rect_.height=height; }
	void setSize(float width, float height) { rect_.width=width;rect_.height=height; }
	void setSize(const ofVec2f &size) { rect_.width=size.x;rect_.height=size.y; }
	void setRectangle(const ofRectangle &rect) { rect_=rect; }
	void setBlendMode(ofBlendMode mode) { blend_mode_=mode; }
	void setOpacity(float opacity) { opacity_=opacity; }
	
	bool isCollapseTransformation() { return is_collapse_; }
	float getOffsetX() { return rect_.x; }
	float getOffsetY() { return rect_.y; }
	ofVec2f getOffset() { return ofVec2f(rect_.x,rect_.y); }
	float getWidth() { return rect_.width; }
	float getHeight() { return rect_.height; }
	ofVec2f getSize() { return ofVec2f(rect_.width,rect_.height); }
	const ofRectangle& getRectangle() { return rect_; }
	ofBlendMode getBlendMode() { return blend_mode_; }
	float getOpacity() { return opacity_; }
protected:
	bool is_collapse_;
	ofRectangle rect_;
	ofBlendMode blend_mode_;
	float opacity_=1;
};

OFX_AE_NAMESPACE_END
/* EOF */
