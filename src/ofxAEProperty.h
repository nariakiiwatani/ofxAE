#pragma once

#include <map>
#include "TransformNode.h"
#include "ofPath.h"

namespace ofxAE {
	
class PropertyBase
{
public:
	PropertyBase():is_dirty_(true){}
	virtual bool setFrame(int frame)=0;
	virtual void update(){}
	virtual bool isDirty() { return is_dirty_; }
	void dirty() { is_dirty_ = true; }
protected:
	bool is_dirty_;
};
template<typename Type>
class Property : public PropertyBase
{
public:
	operator const Type&() { return current_; }
	Type& get() { return current_; }
	const Type& operator =(const Type& val) { current_ = val; return val; }
	void addKey(int frame, const Type& val);
	bool setFrame(int frame);
private:
	Type current_;
	std::map<int, Type> key_;
};
class PropertyGroup : public PropertyBase
{
public:
	bool setFrame(int frame);
	void update();
protected:
	virtual void prepare(){}
	vector<PropertyBase*> properties_;
};

class TransformProperty : public PropertyGroup
{
	friend class Loader;
public:
	TransformProperty();
	operator const TransformNode&() { return current_; }
	TransformNode& get() { return current_; }
	operator TransformNode&() { return current_; }
	void setTranslation(const ofVec3f& translation);
	void setRotation(const ofVec3f& rotation);
	void setOrientation(const ofVec3f& orientation);
	void setScale(const ofVec3f& scale);
	void setAnchorPoint(const ofVec3f& anchor_point);
	bool isDirty();
private:
	TransformNode current_;
	Property<ofVec3f> translation_;
	Property<ofVec3f> rotation_;
	Property<ofVec3f> orientation_;
	Property<ofVec3f> scale_;
	Property<ofVec3f> anchor_point_;
private:
	void prepare();
};

class PathProperty : public PropertyGroup
{
	friend class Loader;
public:
	operator const ofPath&() { return current_; }
	ofPath& get() { return current_; }
	operator ofPath&() { return current_; }
	
	void setInverted(bool inverted) { is_inverted_ = inverted; }
	void setSize(const ofVec2f& size) { size_ = size; }
	
	void setVertex(int index, const ofVec2f& val);
	void setInTangent(int index, const ofVec2f& val);
	void setOutTangent(int index, const ofVec2f& val);
	void setVertexSize(int size);
	void setInTangentSize(int size);
	void setOutTangentSize(int size);
private:
	ofPath current_;
	bool is_inverted_;
	ofVec2f size_;
	vector<Property<ofVec2f> > vertices_;
	vector<Property<ofVec2f> > in_tangents_;
	vector<Property<ofVec2f> > out_tangents_;
private:
	void prepare();
};

}

/* EOF */