#include "ofxAEProperty.h"
#include "ofColor.h"

OFX_AE_NAMESPACE_BEGIN

template<typename Type>
void Property<Type>::addKey(int frame, const Type& val)
{
	key_.insert(std::pair<int,Type>(frame,val));
}

template<typename Type>
void Property<Type>::setFrame(int frame)
{
	if(key_.size() <= 0) {
		return;
	}
	const Type &val = getValueAtFrame(frame);
	if(target_) {
		*target_ = val;
	}
	ofNotifyEvent(event_, val);
}

template<typename Type>
const Type& Property<Type>::getValueAtFrame(int frame)
{
	typename std::map<int, Type>::iterator it = key_.find(frame);
	if(it != key_.end()) {
		return it->second;
	}
	return (--key_.upper_bound(frame))->second;
}

template class Property<float>;
template class Property<bool>;
template class Property<ofVec2f>;
template class Property<ofVec3f>;
template class Property<ofFloatColor>;

void PropertyGroup::setFrame(int frame)
{
	for(vector<PropertyBase*>::iterator it = properties_.begin(); it != properties_.end(); ++it) {
		(*it)->setFrame(frame);
	}
}
void PropertyGroup::addProperty(PropertyBase *property)
{
	properties_.push_back(property);
}

void PropertyGroup::removeProperty(PropertyBase *property)
{
	vector<PropertyBase*>::iterator element = find(properties_.begin(), properties_.end(), property);
	if(element != properties_.end()) {
		properties_.erase(element);
	}
}

TransformProperty::TransformProperty(const string &name)
:PropertyGroup(name)
,translation_("translation")
,rotation_("rotation")
,orientation_("orientation")
,scale_("scale")
,anchor_point_("anchor point")
{
	addProperty(&translation_);
	addProperty(&rotation_);
//	addProperty(&orientation_);
	addProperty(&scale_);
	addProperty(&anchor_point_);
}

void PathProperty::setFrame(int frame)
{
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
			
			path.moveTo((ofPoint)vertices_.back().getValueAtFrame(frame));
			int vertex_count = vertices_.size();
			for(int i0 = vertex_count; i0-- > 0;) {
				const ofVec2f& p0 = vertices_[i0].getValueAtFrame(frame);
				ofPoint c0 = p0+in_tangents_[i0].getValueAtFrame(frame);
				int i1 = i0>0?(i0-1):(vertex_count-1);
				const ofVec2f& p1 = vertices_[i1].getValueAtFrame(frame);
				ofPoint c1 = p1+out_tangents_[i1].getValueAtFrame(frame);
				path.bezierTo(c0, c1, p1);
			}
			path.close();
		}
		else {
			path.moveTo((ofPoint)vertices_.front().getValueAtFrame(frame));
			int vertex_count = vertices_.size();
			for(int i0 = 0; i0 < vertex_count; ++i0) {
				const ofVec2f& p0 = vertices_[i0].getValueAtFrame(frame);
				ofPoint c0 = p0+out_tangents_[i0].getValueAtFrame(frame);
				int i1 = (i0+1<vertex_count)?i0+1:0;
				const ofVec2f& p1 = vertices_[i1].getValueAtFrame(frame);
				ofPoint c1 = p1+in_tangents_[i1].getValueAtFrame(frame);
				path.bezierTo(c0, c1, p1);
			}
			path.close();
		}
	}
}
	
void PathProperty::setVertexSize(int size)
{
	for(vector<Property<ofVec2f> >::iterator it = vertices_.begin(); it != vertices_.end(); ++it) {
		removeProperty(&*it);
	}
	vertices_.resize(size);
	for(vector<Property<ofVec2f> >::iterator it = vertices_.begin(); it != vertices_.end(); ++it) {
		addProperty(&*it);
	}
}
void PathProperty::setInTangentSize(int size)
{
	for(vector<Property<ofVec2f> >::iterator it = in_tangents_.begin(); it != in_tangents_.end(); ++it) {
		removeProperty(&*it);
	}
	in_tangents_.resize(size);
	for(vector<Property<ofVec2f> >::iterator it = in_tangents_.begin(); it != in_tangents_.end(); ++it) {
		addProperty(&*it);
	}
}
void PathProperty::setOutTangentSize(int size)
{
	for(vector<Property<ofVec2f> >::iterator it = out_tangents_.begin(); it != out_tangents_.end(); ++it) {
		removeProperty(&*it);
	}
	out_tangents_.resize(size);
	for(vector<Property<ofVec2f> >::iterator it = out_tangents_.begin(); it != out_tangents_.end(); ++it) {
		addProperty(&*it);
	}
}

OFX_AE_NAMESPACE_END
/* EOF */