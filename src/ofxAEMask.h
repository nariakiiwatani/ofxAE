#pragma once

#include "ofPath.h"

namespace ofxAE {
	class AVLayer;
}
namespace ofxAE {
class Mask {
	friend class Loader;
public:
	Mask();
	bool isDirty() { return is_dirty_; }
	void update(AVLayer *layer);
	void draw();

	void setVertex(int index, const ofVec2f& val);
	void setInTangent(int index, const ofVec2f& val);
	void setOutTangent(int index, const ofVec2f& val);
	void setVertexSize(int size);
	void setInTangentSize(int size);
	void setOutTangentSize(int size);
	
	bool isSubtract();
private:
	void dirty() { is_dirty_=true; }
	string name_;
	ofBlendMode blend_mode_;
	bool is_inverted_;
	vector<ofVec2f> vertices_;
	vector<ofVec2f> in_tangents_;
	vector<ofVec2f> out_tangents_;
	ofPath path_;
	bool is_dirty_;
};

}

/* EOF */