#include "ofxAEProperty.h"
#include "ofColor.h"

namespace ofxAE {
template<typename Type>
void Property<Type>::addKey(int frame, const Type& val)
{
	key_.insert(std::pair<int,Type>(frame,val));
}

template<typename Type>
bool Property<Type>::setFrame(int frame)
{
	if(key_.size() <= 0) {
		return false;
	}
	typename std::map<int, Type>::iterator it = key_.find(frame);
	if(it != key_.end()) {
		current_ = it->second;
		return true;
	}
	Type& next = (--key_.upper_bound(frame))->second;
	bool ret = (current_ != next);
	current_ = next;
	return ret;
}

template class Property<float>;
template class Property<bool>;
template class Property<ofVec2f>;
template class Property<ofVec3f>;
template class Property<ofFloatColor>;

bool PropertyGroup::setFrame(int frame)
{
	bool ret = false;
	for(vector<PropertyBase*>::iterator it = properties_.begin(); it != properties_.end(); ++it) {
		ret |= (*it)->setFrame(frame);
	}
	if(ret) {
		dirty();
	}
	return ret;
}
void PropertyGroup::update()
{
	for(vector<PropertyBase*>::iterator it = properties_.begin(); it != properties_.end(); ++it) {
		if((*it)->isDirty()) {
			(*it)->update();
		}
	}
	prepare();
	is_dirty_ = false;
}

TransformProperty::TransformProperty()
{
	scale_ = ofVec3f(1,1,1);
	properties_.push_back(&translation_);
	properties_.push_back(&rotation_);
	properties_.push_back(&orientation_);
	properties_.push_back(&scale_);
	properties_.push_back(&anchor_point_);
}

static void calcOrientation(TransformNode& node, const ofVec3f& orientation)
{
	switch(node.getRotationOrder()) {
		case TransformNode::ROTATION_ORDER_XYZ:
			node.setOrientation(orientation.x,ofVec3f(1,0,0),
								orientation.y,ofVec3f(0,1,0),
								orientation.z,ofVec3f(0,0,1));
			break;
		case TransformNode::ROTATION_ORDER_YZX:
			node.setOrientation(orientation.y,ofVec3f(0,1,0),
								orientation.z,ofVec3f(0,0,1),
								orientation.x,ofVec3f(1,0,0));
			break;
		case TransformNode::ROTATION_ORDER_ZXY:
			node.setOrientation(orientation.z,ofVec3f(0,0,1),
								orientation.x,ofVec3f(1,0,0),
								orientation.y,ofVec3f(0,1,0));
			break;
		case TransformNode::ROTATION_ORDER_XZY:
			node.setOrientation(orientation.x,ofVec3f(1,0,0),
								orientation.z,ofVec3f(0,0,1),
								orientation.y,ofVec3f(0,1,0));
			break;
		case TransformNode::ROTATION_ORDER_YXZ:
			node.setOrientation(orientation.y,ofVec3f(0,1,0),
								orientation.x,ofVec3f(1,0,0),
								orientation.z,ofVec3f(0,0,1));
			break;
		case TransformNode::ROTATION_ORDER_ZYX:
			node.setOrientation(orientation.z,ofVec3f(0,0,1),
								orientation.y,ofVec3f(0,1,0),
								orientation.x,ofVec3f(1,0,0));
			break;
	}
}
void TransformProperty::prepare()
{
	current_.setTranslation(translation_);
	current_.setRotation(rotation_);
	calcOrientation(current_, orientation_);
	current_.setScale(scale_);
	current_.setAnchorPoint(anchor_point_);
	current_.refreshMatrix();
}

bool TransformProperty::isDirty()
{
	return is_dirty_ || current_.isDirty();
}
void TransformProperty::setTranslation(const ofVec3f& translation)
{
	translation_ = translation;
	dirty();
}
void TransformProperty::setRotation(const ofVec3f& rotation)
{
	rotation_ = rotation;
	dirty();
}
void TransformProperty::setOrientation(const ofVec3f& orientation)
{
	orientation_ = orientation;
	dirty();
}
void TransformProperty::setScale(const ofVec3f& scale)
{
	scale_ = scale;
	dirty();
}
void TransformProperty::setAnchorPoint(const ofVec3f& anchor_point)
{
	anchor_point_ = anchor_point;
	dirty();
}

void PathProperty::prepare()
{
	ofPath& path = get();
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
	is_dirty_ = false;
}
	
void PathProperty::setVertex(int index, const ofVec2f& val)
{
	vertices_[index] = val;
	dirty();
}
void PathProperty::setInTangent(int index, const ofVec2f& val)
{
	in_tangents_[index] = val;
	dirty();
}
void PathProperty::setOutTangent(int index, const ofVec2f& val)
{
	out_tangents_[index] = val;
	dirty();
}
void PathProperty::setVertexSize(int size)
{
	for(vector<Property<ofVec2f> >::iterator it = vertices_.begin(); it != vertices_.end(); ++it) {
		vector<PropertyBase*>::iterator element = find(properties_.begin(), properties_.end(), &vertices_.front());
		if(element != properties_.end()) {
			properties_.erase(element);
		}
	}
	vertices_.resize(size);
	for(vector<Property<ofVec2f> >::iterator it = vertices_.begin(); it != vertices_.end(); ++it) {
		properties_.push_back(&*it);
	}
}
void PathProperty::setInTangentSize(int size)
{
	for(vector<Property<ofVec2f> >::iterator it = in_tangents_.begin(); it != in_tangents_.end(); ++it) {
		vector<PropertyBase*>::iterator element = find(properties_.begin(), properties_.end(), &in_tangents_.front());
		if(element != properties_.end()) {
			properties_.erase(element);
		}
	}
	in_tangents_.resize(size);
	for(vector<Property<ofVec2f> >::iterator it = in_tangents_.begin(); it != in_tangents_.end(); ++it) {
		properties_.push_back(&*it);
	}
}
void PathProperty::setOutTangentSize(int size)
{
	for(vector<Property<ofVec2f> >::iterator it = out_tangents_.begin(); it != out_tangents_.end(); ++it) {
		vector<PropertyBase*>::iterator element = find(properties_.begin(), properties_.end(), &out_tangents_.front());
		if(element != properties_.end()) {
			properties_.erase(element);
		}
	}
	out_tangents_.resize(size);
	for(vector<Property<ofVec2f> >::iterator it = out_tangents_.begin(); it != out_tangents_.end(); ++it) {
		properties_.push_back(&*it);
	}
}

}
/* EOF */