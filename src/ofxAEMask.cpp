#include "ofxAEMask.h"
#include "ofxAEAVLayer.h"
#include "ofGraphics.h"

namespace ofxAE {
Mask::Mask()
{
	path_.setPolyWindingMode(OF_POLY_WINDING_POSITIVE);
	path_.setColor(ofColor::white);
}
	
void Mask::update(AVLayer *layer)
{
	path_.clear();
	if(vertices_.empty()) {
		return;
	}
	if(is_inverted_) {
		path_.moveTo(0,0);
		path_.lineTo(0,layer->getHeight());
		path_.lineTo(layer->getWidth(),layer->getHeight());
		path_.lineTo(layer->getWidth(),0);
		path_.lineTo(0,0);
		path_.close();
		
		path_.moveTo(vertices_.back());
		vector<ofVec2f>::reverse_iterator it = vertices_.rbegin();
		while(it != vertices_.rend()) {
			path_.lineTo(*it++);
		}
	}
	else {
		path_.moveTo(vertices_.front());
		vector<ofVec2f>::iterator it = vertices_.begin();
		while(it != vertices_.end()) {
			path_.lineTo(*it++);
		}
	}
	
	is_dirty_ = false;
}
void Mask::draw()
{
	ofPushStyle();
	ofEnableBlendMode(blend_mode_);
	path_.draw();
	ofPopStyle();
}

void Mask::setVertex(int index, const ofVec2f& val)
{
	vertices_[index] = val;
	dirty();
}
void Mask::setInTangent(int index, const ofVec2f& val)
{
	in_tangents_[index] = val;
	dirty();
}
void Mask::setOutTangent(int index, const ofVec2f& val)
{
	out_tangents_[index] = val;
	dirty();
}
void Mask::setVertexSize(int size)
{
	vertices_.resize(size);
}
void Mask::setInTangentSize(int size)
{
	in_tangents_.resize(size);
}
void Mask::setOutTangentSize(int size)
{
	out_tangents_.resize(size);
}

bool Mask::isSubtract()
{
	return blend_mode_==OF_BLENDMODE_SUBTRACT;
}
}

/* EOF */