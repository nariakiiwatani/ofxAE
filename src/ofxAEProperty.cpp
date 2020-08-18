#include "ofxAEProperty.h"
#include "ofColor.h"
#include "ofPoint.h"

using namespace std;

OFX_AE_NAMESPACE_BEGIN

template<typename Type>
void Property<Type>::addKey(int frame, const Type& val)
{
	key_.insert(pair<int,Type>(frame,val));
}

template<typename Type>
void Property<Type>::setFrame(int frame)
{
	if(key_.size() <= 0) {
		return;
	}
	Type val = getValueAtFrame(frame);
	if(target_) {
		*target_ = val;
	}
	ofNotifyEvent(event_, val);
}

template<typename Type>
const Type& Property<Type>::getValueAtFrame(int frame)
{
	auto it = key_.find(frame);
	if(it != end(key_)) {
		return it->second;
	}
	return (--key_.upper_bound(frame))->second;
}

template class Property<float>;
template class Property<bool>;
template class Property<ofVec2f>;
template class Property<ofVec3f>;
template class Property<ofFloatColor>;

/* =================== */

void PropertyGroup::setFrame(int frame)
{
	for(auto &p : properties_) {
		p.second->setFrame(frame);
	}
}

void PropertyGroup::add(const std::string &name, std::shared_ptr<PropertyBase> prop)
{
	properties_.push_back(std::make_pair(name, prop));
}

void PropertyGroup::removeProperty(const string &name)
{
	properties_.erase(remove_if(begin(properties_), end(properties_), [&](const pair<string, shared_ptr<PropertyBase>> &p) {
		return p.first == name;
	}), end(properties_));
}

void PropertyGroup::removeProperty(const string &name, int index)
{
	auto found = find(name, index);
	if(found != end(properties_)) {
		properties_.erase(found);
	}
}

PropertyGroup::Properties::iterator PropertyGroup::find(const string &name, int index)
{
	return find_if(begin(properties_), end(properties_), [&](const pair<string, shared_ptr<PropertyBase>> &p) {
		return p.first == name && --index<0;
	});
}

/* =================== */

void PathProperty::setFrame(int frame)
{
	PropertyGroup::setFrame(frame);
	if(target_) {
		ofPath &path = *target_;
		path.clear();
		if(is_inverted_) {
			path.moveTo(0,0);
			path.lineTo(0,size_.y);
			path.lineTo(size_.x,size_.y);
			path.lineTo(size_.x,0);
			path.lineTo(0,0);
			path.close();
			
			path.moveTo((ofPoint)vertices_.back());
			int vertex_count = vertices_.size();
			for(int i0 = vertex_count; i0-- > 0;) {
				const ofVec2f& p0 = vertices_[i0];
				ofPoint c0 = p0+in_tangents_[i0];
				int i1 = i0>0?(i0-1):(vertex_count-1);
				const ofVec2f& p1 = vertices_[i1];
				ofPoint c1 = p1+out_tangents_[i1];
				path.bezierTo(c0, c1, p1);
			}
			path.close();
		}
		else {
			path.moveTo((ofPoint)vertices_.front());
			int vertex_count = vertices_.size();
			for(int i0 = 0; i0 < vertex_count; ++i0) {
				const ofVec2f& p0 = vertices_[i0];
				ofPoint c0 = p0+out_tangents_[i0];
				int i1 = (i0+1<vertex_count)?i0+1:0;
				const ofVec2f& p1 = vertices_[i1];
				ofPoint c1 = p1+in_tangents_[i1];
				path.bezierTo(c0, c1, p1);
			}
			path.close();
		}
	}
}
	
void PathProperty::setVertexSize(int size)
{
	removeProperty("vertex");
	vertices_.resize(size);
	for(int i = 0; i < size; ++i) {
		addProperty<ofVec2f>("vertex")->setTarget(&vertices_[i]);
	}
}
void PathProperty::setInTangentSize(int size)
{
	removeProperty("in tangent");
	in_tangents_.resize(size);
	for(int i = 0; i < size; ++i) {
		addProperty<ofVec2f>("in tangent")->setTarget(&in_tangents_[i]);
	}
}
void PathProperty::setOutTangentSize(int size)
{
	removeProperty("out tangent");
	out_tangents_.resize(size);
	for(int i = 0; i < size; ++i) {
		addProperty<ofVec2f>("out tangent")->setTarget(&out_tangents_[i]);
	}
}

/* =================== */

OFX_AE_NAMESPACE_END
/* EOF */
