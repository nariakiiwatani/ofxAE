#pragma once

#include "ofxAEDef.h"
#include "ofxAELayer.h"
#include "ofxMask.h"

OFX_AE_NAMESPACE_BEGIN

class Mask;
class Marker;

class AVLayer : public Layer {
	friend class Loader;
public:
	void allocate(int width, int height);
	void draw(float alpha=1);
	float getWidth() { return size_.x; }
	float getHeight() { return size_.y; }
	const ofVec2f& getSize() { return size_; }
	bool is3D() { return is_3d_; }
	bool isCollapse() { return is_collapse_; }
	void addMask(std::shared_ptr<Mask> mask);
	std::shared_ptr<Mask> addNewMask(const std::string &name);
	ofBlendMode getBlendMode() { return blend_mode_; }
	
	bool isHit(float x, float y);
	bool isHit(const ofVec3f &point);

protected:
	bool is_3d_=false;
	bool is_collapse_=false;
	ofBlendMode blend_mode_;
	ofVec2f size_;
	std::vector<std::shared_ptr<Mask>> mask_;
	ofxMask ofx_mask_;
	bool is_mask_allocated_=false;
};

OFX_AE_NAMESPACE_END
/* EOF */
