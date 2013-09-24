#pragma once

#include "ofPath.h"

namespace ofxAE {
	
class Path : public ofPath
{
public:
	bool isDirty() { return is_dirty_; }
	void update();
	void updateInverted(const ofVec2f& size);

	void setVertex(int index, const ofVec2f& val);
	void setInTangent(int index, const ofVec2f& val);
	void setOutTangent(int index, const ofVec2f& val);
	void setVertexSize(int size);
	void setInTangentSize(int size);
	void setOutTangentSize(int size);
protected:
	void dirty() { is_dirty_=true; }
	bool is_inverted_;
	vector<ofVec2f> vertices_;
	vector<ofVec2f> in_tangents_;
	vector<ofVec2f> out_tangents_;
	bool is_dirty_;
};

}

/* EOF */