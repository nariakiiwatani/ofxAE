#include "ofxAEShapeCap.h"
#include "ofxAEAVLayer.h"
#include "ofGraphics.h"

using namespace std;

template<> const ofColor ofColor::white;

OFX_AE_NAMESPACE_BEGIN

ShapeCap::ShapeCap()
{
	path_.setMode(ofPath::COMMANDS);
	shape_prop_ = shared_ptr<PropertyGroup>(new PropertyGroup());
}

void ShapeCap::setLayer(shared_ptr<AVLayer> layer)
{
	layer->add("shape", shape_prop_);
}

void ShapeCap::update()
{
	path_.clear();
	for(auto &c : content_) {
		c->push(path_);
	}
}
void ShapeCap::draw(float alpha)
{
	ofPushStyle();
	ofEnableBlendMode(blend_mode_);
	path_.setStrokeColor(ofColor(ofColor::white, opacity_*alpha*255));
	path_.setFillColor(ofColor(ofColor::white, opacity_*alpha*255));
	for(auto &c : content_) {
		c->pop(path_);
	}
	ofPopStyle();
}

void ShapeCap::addContent(shared_ptr<ShapeContent> content)
{
	content_.push_back(content);
	shape_prop_->add("content", content);
}

/* =================== */

ShapeContentGroup::ShapeContentGroup()
{
	auto transform = add<TransformProperty>("transform");
	transform->getTranslation()->setCallback(&transform_, &TransformNode::setTranslation);
	transform->getScale()->setCallback(&transform_, &TransformNode::setScale);
	transform->getAnchorPoint()->setCallback(&transform_, &TransformNode::setAnchorPoint);
	
	addProperty<float>("opacity")->setTarget(&opacity_);
	addProperty<float>("rotation z")->setCallback(this, &ShapeContentGroup::rotationZPropCallback);
	addProperty<float>("skew")->setTarget(&skew_);
	addProperty<float>("skew axis")->setTarget(&skew_axis_);
}

void ShapeContentGroup::push(ofPath& path)
{
	for(auto &c : content_) {
		c->push(path);
	}
}
void ShapeContentGroup::pop(ofPath& path)
{
	if(transform_.isDirty()) {
		transform_.refreshMatrix();
	}
	transform_.pushMatrix();
	ofColor prev_st = path.getStrokeColor();
	ofColor prev_fi = path.getFillColor();
	float opacity_st = prev_st.a/255.f*opacity_;
	float opacity_fi = prev_fi.a/255.f*opacity_;
	path.setStrokeColor(ofColor(prev_st, opacity_st*255));
	path.setFillColor(ofColor(prev_fi, opacity_fi*255));
	for(auto &c : content_) {
		c->pop(path);
	}
	path.setStrokeColor(prev_st);
	path.setFillColor(prev_fi);
	transform_.popMatrix();
}
void ShapeContentGroup::addContent(shared_ptr<ShapeContent> content)
{
	content_.push_back(content);
	add("content", content);
}
void ShapeContentGroup::setPosition(const ofVec2f& position)
{
	transform_.setTranslation(position);
}
void ShapeContentGroup::setRotation(float z)
{
	transform_.setRotation(ofVec3f(0,0,rotation_z_));
}
void ShapeContentGroup::setScale(const ofVec2f& scale)
{
	transform_.setScale(ofVec3f(scale.x,scale.y,1));
}
void ShapeContentGroup::setAnchorPoint(const ofVec2f& anchor)
{
	transform_.setAnchorPoint(anchor);
}

void ShapeContentGroup::rotationZPropCallback(const float &val)
{
	setRotation(val);
}

/* =================== */

void ShapeContentShape::pop(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	while(command_count_-- > 0) {
		command.pop_back();
	}
}

/* =================== */

ShapeContentPath::ShapeContentPath()
{
	path_.setMode(ofPath::COMMANDS);
	add<PathProperty>("path")->setTarget(&path_);
}
void ShapeContentPath::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	vector<ofPath::Command>& my_command = path_.getCommands();
	command.insert(end(command), begin(my_command), end(my_command));
	command_count_ = command.size() - command_count_prev;
}

/* =================== */

ShapeContentEllipse::ShapeContentEllipse()
{
	addProperty<ofVec2f>("position")->setTarget(&pos_);
	addProperty<ofVec2f>("size")->setTarget(&size_);
}

void ShapeContentEllipse::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	path.ellipse(pos_.x, pos_.y, size_.x, size_.y);
	path.close();
	command_count_ = command.size() - command_count_prev;
}

/* =================== */

ShapeContentRect::ShapeContentRect()
{
	addProperty<ofVec2f>("size")->setTarget(&size_);
	addProperty<ofVec2f>("position")->setTarget(&pos_);
	addProperty<float>("roundness")->setTarget(&roundness_);
}

void ShapeContentRect::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	path.rectRounded(pos_-size_/2.f, size_.x, size_.y, roundness_);
	command_count_ = command.size() - command_count_prev;
}

/* =================== */

ShapeContentPoly::ShapeContentPoly()
{
	addProperty<bool>("star")->setTarget(&is_star_);
	addProperty<float>("corner count")->setTarget(&corner_count_);
	addProperty<ofVec2f>("position")->setTarget(&pos_);
	addProperty<float>("rotation")->setTarget(&rotation_);
	addProperty<float>("outer radius")->setTarget(&outer_radius_);
	addProperty<float>("outer roundness")->setTarget(&outer_roundness_);
	addProperty<float>("inner radius")->setTarget(&inner_radius_);
	addProperty<float>("inner roundness")->setTarget(&inner_roundness_);
}

void ShapeContentPoly::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	
	
	
	
	command_count_ = command.size() - command_count_prev;
}

/* =================== */

ShapeContentStroke::ShapeContentStroke()
{
	addProperty<ofFloatColor>("color")->setTarget(&color_);
	addProperty<float>("opacity")->setTarget(&opacity_);
	addProperty<float>("stroke width")->setTarget(&stroke_width_);
}

void ShapeContentStroke::pop(ofPath& path)
{
	ofPushStyle();
	path.setFilled(false);
	ofColor prev = path.getStrokeColor();
	float opacity = prev.a/255.f*opacity_;
	path.setStrokeColor(ofColor(color_, opacity*255));
	path.setStrokeWidth(stroke_width_);
	ofEnableBlendMode(blend_mode_);
	path.draw();
	path.setStrokeColor(prev);
	ofPopStyle();
}

/* =================== */

ShapeContentFill::ShapeContentFill()
{
	addProperty<ofFloatColor>("color")->setTarget(&color_);
	addProperty<float>("opacity")->setTarget(&opacity_);
}

void ShapeContentFill::pop(ofPath& path)
{
	ofPushStyle();
	path.setFilled(true);
	ofColor prev = path.getFillColor();
	float opacity = prev.a/255.f*opacity_;
	path.setFillColor(ofColor(color_, opacity*255));
	ofEnableBlendMode(blend_mode_);
	path.draw();
	path.setFillColor(prev);
	ofPopStyle();
}

OFX_AE_NAMESPACE_END
/* EOF */
