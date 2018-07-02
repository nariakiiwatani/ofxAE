#pragma once

#include "ofxAEDef.h"
#include <map>
#include "TransformNode.h"
#include "ofPath.h"
#include "ofEventUtils.h"

OFX_AE_NAMESPACE_BEGIN

class PropertyBase
{
public:
	PropertyBase(){}
	PropertyBase(const string &name):name_(name){}
	virtual ~PropertyBase(){}
	const string& getName() { return name_; }
	virtual void setFrame(int frame)=0;
protected:
	string name_="";
};
template<typename Type>
class Property : public PropertyBase
{
public:
	using PropertyBase::PropertyBase;
	void setTarget(Type *target) { target_=target; }
	template<class ListenerClass>
	void setCallback(ListenerClass *listener, void (ListenerClass::*listenerMethod)(const Type&)) { ofAddListener(event_, listener, listenerMethod); }
	void addKey(int frame, const Type& val);
	void setFrame(int frame);
	const Type& getValueAtFrame(int frame);
private:
	Type *target_=nullptr;
	ofEvent<const Type> event_;
	std::map<int, Type> key_;
};
class PropertyGroup : public PropertyBase
{
public:
	using PropertyBase::PropertyBase;
	virtual void setFrame(int frame);
	void addProperty(PropertyBase *property);
	void removeProperty(PropertyBase *property);
protected:
	vector<PropertyBase*> properties_;
};

class TransformProperty : public PropertyGroup
{
	friend class Loader;
public:
	TransformProperty(const string &name="transform");
	Property<ofVec3f> translation_;
	Property<ofVec3f> rotation_;
	Property<ofVec3f> orientation_;
	Property<ofVec3f> scale_;
	Property<ofVec3f> anchor_point_;
};

class PathProperty : public PropertyGroup
{
	friend class Loader;
public:
	PathProperty(const string &name="path"):PropertyGroup(name){}
	void setTarget(ofPath *target) { target_=target; }
	void setFrame(int frame);
	void setInverted(bool inverted) { is_inverted_ = inverted; }
	void setSize(const ofVec2f& size) { size_ = size; }
	
	void setVertex(int index, const ofVec2f& val);
	void setInTangent(int index, const ofVec2f& val);
	void setOutTangent(int index, const ofVec2f& val);
	void setVertexSize(int size);
	void setInTangentSize(int size);
	void setOutTangentSize(int size);
private:
	ofPath *target_=nullptr;
	bool is_inverted_;
	ofVec2f size_;
	vector<Property<ofVec2f> > vertices_;
	vector<Property<ofVec2f> > in_tangents_;
	vector<Property<ofVec2f> > out_tangents_;
};

OFX_AE_NAMESPACE_END
/* EOF */
