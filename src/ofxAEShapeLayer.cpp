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

void ShapeContentGroup::push(ofPath& path)
{
	if(trans_.isDirty()) {
		trans_.refreshMatrix();
	}
	for(vector<ShapeContent*>::iterator it = content_.begin(); it != content_.end(); ++it) {
		(*it)->push(path);
	}
}
void ShapeContentGroup::pop(ofPath& path)
{
	trans_.pushMatrix();
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
	trans_.popMatrix();
}
	
void ShapeContentShape::pop(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	while(command_count_-- > 0) {
		command.pop_back();
	}
}

void ShapeContentEllipse::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	path.ellipse(pos_.x, pos_.y, size_.x, size_.y);
	command_count_ = command.size() - command_count_prev;
}

void ShapeContentStroke::pop(ofPath& path)
{
	path.setFilled(false);
	ofColor prev = path.getStrokeColor();
	float opacity = prev.a/255.f*opacity_;
	path.setStrokeColor(ofColor(color_, opacity*255));
	path.setStrokeWidth(width_);
	path.draw();
	path.setStrokeColor(prev);
}

}

/* EOF */