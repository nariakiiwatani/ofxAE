#include "ofxAEPath.h"

namespace ofxAE {
	
void Path::update()
{
	clear();
	if(vertices_.empty()) {
		return;
	}
	moveTo(vertices_.front());
	int vertex_count = vertices_.size();
	for(int i0 = 0; i0 < vertex_count; ++i0) {
		const ofPoint& p0 = vertices_[i0];
		ofPoint c0 = p0+out_tangents_[i0];
		int i1 = (i0+1<vertex_count)?i0+1:0;
		const ofPoint& p1 = vertices_[i1];
		ofPoint c1 = p1+in_tangents_[i1];
		bezierTo(c0, c1, p1);
	}
	close();
	is_dirty_ = false;
}
void Path::updateInverted(const ofVec2f& size)
{
	clear();
	if(vertices_.empty()) {
		return;
	}
	moveTo(0,0);
	lineTo(0,size.y);
	lineTo(size.x,size.y);
	lineTo(size.x,0);
	lineTo(0,0);
	close();
	
	moveTo(vertices_.back());
	int vertex_count = vertices_.size();
	for(int i0 = vertex_count; i0-- > 0;) {
		const ofPoint& p0 = vertices_[i0];
		ofPoint c0 = p0+in_tangents_[i0];
		int i1 = i0>0?(i0-1):(vertex_count-1);
		const ofPoint& p1 = vertices_[i1];
		ofPoint c1 = p1+out_tangents_[i1];
		bezierTo(c0, c1, p1);
	}
	close();
	is_dirty_ = false;
}

void Path::setVertex(int index, const ofVec2f& val)
{
	vertices_[index] = val;
	dirty();
}
void Path::setInTangent(int index, const ofVec2f& val)
{
	in_tangents_[index] = val;
	dirty();
}
void Path::setOutTangent(int index, const ofVec2f& val)
{
	out_tangents_[index] = val;
	dirty();
}
void Path::setVertexSize(int size)
{
	vertices_.resize(size);
}
void Path::setInTangentSize(int size)
{
	in_tangents_.resize(size);
}
void Path::setOutTangentSize(int size)
{
	out_tangents_.resize(size);
}
}

/* EOF */