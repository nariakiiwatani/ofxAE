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
	virtual void setFrame(int frame)=0;
};
template<typename Type>
class Property : public PropertyBase
{
public:
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
	virtual void setFrame(int frame);
	
	void add(const std::string &name, std::shared_ptr<PropertyBase> prop);
	template<typename T> std::shared_ptr<T> add(const std::string &name);
	template<typename T> std::shared_ptr<Property<T>> addProperty(const std::string &name);
	template<typename T> std::shared_ptr<T> get(const std::string &name, int index=0);
	template<typename T> std::shared_ptr<Property<T>> getProperty(const std::string &name, int index=0);
	void removeProperty(const std::string &name);
	void removeProperty(const std::string &name, int index);
protected:
	using Properties = std::vector<std::pair<std::string, std::shared_ptr<PropertyBase>>>;
	Properties properties_;
private:
	Properties::iterator find(const std::string &name, int index);
};

template<typename T>
inline std::shared_ptr<T> PropertyGroup::add(const std::string &name) {
	auto prop = std::make_shared<T>();
	add(name, prop);
	return prop;
}
template<typename T>
inline std::shared_ptr<Property<T>> PropertyGroup::addProperty(const std::string &name) {
	return add<Property<T>>(name);
}
template<typename T>
inline std::shared_ptr<T> PropertyGroup::get(const std::string &name, int index) {
	auto found = find(name, index);
	if(found != std::end(properties_)) {
		return std::static_pointer_cast<T>(found->second);
	}
	return nullptr;
}
template<typename T>
inline std::shared_ptr<Property<T>> PropertyGroup::getProperty(const std::string &name, int index) {
	return get<Property<T>>(name, index);
}

class TransformProperty : public PropertyGroup
{
	friend class Loader;
public:
	TransformProperty() {
		addProperty<ofVec3f>("translation");
		addProperty<ofVec3f>("rotation");
		addProperty<ofVec3f>("orientation");
		addProperty<ofVec3f>("scale");
		addProperty<ofVec3f>("anchor point");
	}
	virtual ~TransformProperty() {
		removeProperty("translation");
		removeProperty("rotation");
		removeProperty("orientation");
		removeProperty("scale");
		removeProperty("anchor point");
	}
	std::shared_ptr<Property<ofVec3f>> getTranslation() { return getProperty<ofVec3f>("translation"); }
	std::shared_ptr<Property<ofVec3f>> getRotation() { return getProperty<ofVec3f>("rotation"); }
	std::shared_ptr<Property<ofVec3f>> getOrientation() { return getProperty<ofVec3f>("orientation"); }
	std::shared_ptr<Property<ofVec3f>> getScale() { return getProperty<ofVec3f>("scale"); }
	std::shared_ptr<Property<ofVec3f>> getAnchorPoint() { return getProperty<ofVec3f>("anchor point"); }
};

class PathProperty : public PropertyGroup
{
	friend class Loader;
public:
	void setTarget(ofPath *target) { target_=target; }
	void setFrame(int frame);
	void setInverted(bool inverted) { is_inverted_ = inverted; }
	void setSize(const ofVec2f& size) { size_ = size; }
	
	void setVertexSize(int size);
	void setInTangentSize(int size);
	void setOutTangentSize(int size);
	
	std::shared_ptr<Property<ofVec2f>> getVertexProperty(int index) { return getProperty<ofVec2f>("vertex", index); } 
	std::shared_ptr<Property<ofVec2f>> getInTangentProperty(int index) { return getProperty<ofVec2f>("in tangent", index); } 
	std::shared_ptr<Property<ofVec2f>> getOutTangentProperty(int index) { return getProperty<ofVec2f>("out tangent", index); } 
private:
	ofPath *target_=nullptr;
	bool is_inverted_;
	ofVec2f size_;
	std::vector<ofVec2f> vertices_, in_tangents_, out_tangents_;
};

OFX_AE_NAMESPACE_END
/* EOF */
