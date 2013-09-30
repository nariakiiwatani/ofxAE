#include "ofxAEShapeLayer.h"
#include "ofGraphics.h"

namespace ofxAE {
ShapeLayer::ShapeLayer()
{
	path_.setMode(ofPath::COMMANDS);
}
void ShapeLayer::prepare()
{
	path_.clear();
	for(vector<ShapeContent*>::iterator it = content_.begin(); it != content_.end(); ++it) {
		(*it)->push(path_);
	}
}
void ShapeLayer::render()
{
	for(vector<ShapeContent*>::reverse_iterator it = content_.rbegin(); it != content_.rend(); ++it) {
		(*it)->pop(path_);
	}
}

void ShapeLayer::addContent(ShapeContent *content)
{
	content_.push_back(content);
	properties_.push_back(content);
}
	
ShapeContentGroup::ShapeContentGroup()
{
	properties_.push_back(&transform_);
	properties_.push_back(&rotation_z_);
	properties_.push_back(&opacity_);
	properties_.push_back(&skew_);
	properties_.push_back(&skew_axis_);
}
void ShapeContentGroup::push(ofPath& path)
{
	for(vector<ShapeContent*>::iterator it = content_.begin(); it != content_.end(); ++it) {
		(*it)->push(path);
	}
}
void ShapeContentGroup::pop(ofPath& path)
{
	transform_.get().pushMatrix();
	ofColor prev_st = path.getStrokeColor();
	ofColor prev_fi = path.getFillColor();
	float opacity_st = prev_st.a/255.f*opacity_;
	float opacity_fi = prev_fi.a/255.f*opacity_;
	path.setStrokeColor(ofColor(prev_st, opacity_st*255));
	path.setFillColor(ofColor(prev_fi, opacity_fi*255));
	for(vector<ShapeContent*>::reverse_iterator it = content_.rbegin(); it != content_.rend(); ++it) {
		(*it)->pop(path);
	}
	path.setStrokeColor(prev_st);
	path.setFillColor(prev_fi);
	transform_.get().popMatrix();
}
void ShapeContentGroup::addContent(ShapeContent *content)
{
	content_.push_back(content);
	properties_.push_back(content);
}
void ShapeContentGroup::setPosition(const ofVec2f& position)
{
	transform_.setTranslation(position);
	dirty();
}
void ShapeContentGroup::setRotation(float z)
{
	rotation_z_ = z;
	dirty();
}
void ShapeContentGroup::setScale(const ofVec2f& scale)
{
	transform_.setScale(ofVec3f(scale.x,scale.y,1));
	dirty();
}
void ShapeContentGroup::setAnchorPoint(const ofVec2f& anchor)
{
	transform_.setAnchorPoint(anchor);
	dirty();
}
void ShapeContentGroup::prepare()
{
	transform_.setRotation(ofVec3f(0,0,rotation_z_));
	transform_.get().refreshMatrix();
}
void ShapeContentShape::pop(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	while(command_count_-- > 0) {
		command.pop_back();
	}
}
ShapeContentPath::ShapeContentPath()
{
	path_.get().setMode(ofPath::COMMANDS);
	properties_.push_back(&path_);
}
void ShapeContentPath::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	vector<ofPath::Command>& my_command = path_.get().getCommands();
	for(vector<ofPath::Command>::iterator it = my_command.begin(); it != my_command.end(); ++it) {
		command.push_back(*it);
	}
	command_count_ = command.size() - command_count_prev;
}

ShapeContentEllipse::ShapeContentEllipse()
{
	properties_.push_back(&size_);
	properties_.push_back(&pos_);
}

void ShapeContentEllipse::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	path.ellipse(pos_.get().x, pos_.get().y, size_.get().x, size_.get().y);
	path.close();
	command_count_ = command.size() - command_count_prev;
}

ShapeContentRect::ShapeContentRect()
{
	properties_.push_back(&size_);
	properties_.push_back(&pos_);
	properties_.push_back(&roundness_);
}
void ShapeContentRect::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	path.rectRounded(pos_.get()-size_.get()/2.f, size_.get().x, size_.get().y, roundness_);
	command_count_ = command.size() - command_count_prev;
}

ShapeContentPoly::ShapeContentPoly()
{
	properties_.push_back(&corner_count_);
	properties_.push_back(&pos_);
	properties_.push_back(&rotation_);
	properties_.push_back(&outer_radius_);
	properties_.push_back(&outer_roundness_);
	properties_.push_back(&inner_radius_);
	properties_.push_back(&inner_roundness_);
}
void ShapeContentPoly::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	
	
	
	
	command_count_ = command.size() - command_count_prev;
}

ShapeContentStroke::ShapeContentStroke()
{
	properties_.push_back(&color_);
	properties_.push_back(&opacity_);
	properties_.push_back(&width_);
}

void ShapeContentStroke::pop(ofPath& path)
{
	path.setFilled(false);
	ofColor prev = path.getStrokeColor();
	float opacity = prev.a/255.f*opacity_;
	path.setStrokeColor(ofColor(color_.get(), opacity*255));
	path.setStrokeWidth(width_);
	ofEnableBlendMode(blend_mode_);
	path.draw();
	path.setStrokeColor(prev);
}

ShapeContentFill::ShapeContentFill()
{
	properties_.push_back(&color_);
	properties_.push_back(&opacity_);
}
void ShapeContentFill::pop(ofPath& path)
{
	path.setFilled(true);
	ofColor prev = path.getFillColor();
	float opacity = prev.a/255.f*opacity_;
	path.setFillColor(ofColor(color_.get(), opacity*255));
	ofEnableBlendMode(blend_mode_);
	path.draw();
	path.setFillColor(prev);
}
	
}

/* EOF */