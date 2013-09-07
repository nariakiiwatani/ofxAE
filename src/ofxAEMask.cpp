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
		int vertex_count = vertices_.size();
		for(int i0 = vertex_count; i0-- > 0;) {
			const ofPoint& p0 = vertices_[i0];
			ofPoint c0 = p0+in_tangents_[i0];
			int i1 = i0>0?(i0-1):(vertex_count-1);
			const ofPoint& p1 = vertices_[i1];
			ofPoint c1 = p1+out_tangents_[i1];
			path_.bezierTo(c0, c1, p1);
		}
	}
	else {
		path_.moveTo(vertices_.front());
		int vertex_count = vertices_.size();
		for(int i0 = 0; i0 < vertex_count; ++i0) {
			const ofPoint& p0 = vertices_[i0];
			ofPoint c0 = p0+out_tangents_[i0];
			int i1 = (i0+1<vertex_count)?i0+1:0;
			const ofPoint& p1 = vertices_[i1];
			ofPoint c1 = p1+in_tangents_[i1];
			path_.bezierTo(c0, c1, p1);
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