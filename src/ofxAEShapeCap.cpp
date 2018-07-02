#include "ofxAEShapeCap.h"
#include "ofxAELayer.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

ShapeCap::ShapeCap()
{
	path_.setMode(ofPath::COMMANDS);
}

void ShapeCap::update()
{
	path_.clear();
	for(vector<ShapeContent*>::iterator it = content_.begin(); it != content_.end(); ++it) {
		(*it)->push(path_);
	}
}
void ShapeCap::draw(float alpha)
{
	ofPushStyle();
	ofEnableBlendMode(blend_mode_);
	path_.setStrokeColor(ofColor(ofColor::white, opacity_*alpha*255));
	path_.setFillColor(ofColor(ofColor::white, opacity_*alpha*255));
	for(vector<ShapeContent*>::reverse_iterator it = content_.rbegin(); it != content_.rend(); ++it) {
		(*it)->pop(path_);
	}
	ofPopStyle();
}

void ShapeCap::addContent(ShapeContent *content)
{
	content_.push_back(content);
	if(auto layer = layer_.lock()) {
		layer->addProperty(content);
	}
}

/* =================== */

void ShapeContentGroup::push(ofPath& path)
{
	for(vector<ShapeContent*>::iterator it = content_.begin(); it != content_.end(); ++it) {
		(*it)->push(path);
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
	for(vector<ShapeContent*>::reverse_iterator it = content_.rbegin(); it != content_.rend(); ++it) {
		(*it)->pop(path);
	}
	path.setStrokeColor(prev_st);
	path.setFillColor(prev_fi);
	transform_.popMatrix();
}
void ShapeContentGroup::addContent(ShapeContent *content)
{
	content_.push_back(content);
	addProperty(content);
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

void ShapeContentGroup::addTransformProperty(ofxAE::TransformProperty *prop)
{
	prop->translation_.setCallback(&transform_, &TransformNode::setTranslation);
//	prop->rotation_.setCallback(&transform_, &TransformNode::setRotation);
	prop->scale_.setCallback(&transform_, &TransformNode::setScale);
//	prop->orientation_.setCallback(&transform_, &TransformNode::setOrientation);
	prop->anchor_point_.setCallback(&transform_, &TransformNode::setAnchorPoint);
	addProperty(prop);
}

void ShapeContentGroup::addOpacityProperty(Property<float> *prop)
{
	prop->setTarget(&opacity_);
	addProperty(prop);
}

void ShapeContentGroup::addRotationZProperty(Property<float> *prop)
{
	prop->setCallback(this, &ShapeContentGroup::rotationZPropCallback);
	addProperty(prop);
}
void ShapeContentGroup::rotationZPropCallback(const float &val)
{
	setRotation(val);
}

void ShapeContentGroup::addSkewProperty(Property<float> *prop)
{
	prop->setTarget(&skew_);
	addProperty(prop);
}

void ShapeContentGroup::addSkewAxisProperty(Property<float> *prop)
{
	prop->setTarget(&skew_axis_);
	addProperty(prop);
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

ShapeContentPath::ShapeContentPath(const string &name)
:ShapeContentShape(name)
{
	path_.setMode(ofPath::COMMANDS);
}
void ShapeContentPath::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	vector<ofPath::Command>& my_command = path_.getCommands();
	for(vector<ofPath::Command>::iterator it = my_command.begin(); it != my_command.end(); ++it) {
		command.push_back(*it);
	}
	command_count_ = command.size() - command_count_prev;
}

void ShapeContentPath::addPathProperty(PathProperty *prop)
{
	prop->setTarget(&path_);
	addProperty(prop);
}

/* =================== */

void ShapeContentEllipse::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	path.ellipse(pos_.x, pos_.y, size_.x, size_.y);
	path.close();
	command_count_ = command.size() - command_count_prev;
}

void ShapeContentEllipse::addPositionProperty(Property<ofVec2f> *prop)
{
	prop->setTarget(&pos_);
	addProperty(prop);
}

void ShapeContentEllipse::addSizeProperty(Property<ofVec2f> *prop)
{
	prop->setTarget(&size_);
	addProperty(prop);
}

/* =================== */

void ShapeContentRect::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	path.rectRounded(pos_-size_/2.f, size_.x, size_.y, roundness_);
	command_count_ = command.size() - command_count_prev;
}

void ShapeContentRect::addSizeProperty(Property<ofVec2f> *prop)
{
	prop->setTarget(&size_);
	addProperty(prop);
}

void ShapeContentRect::addPositionProperty(Property<ofVec2f> *prop)
{
	prop->setTarget(&pos_);
	addProperty(prop);
}

void ShapeContentRect::addRoundnessProperty(Property<float> *prop)
{
	prop->setTarget(&roundness_);
	addProperty(prop);
}

/* =================== */

void ShapeContentPoly::push(ofPath& path)
{
	vector<ofPath::Command>& command = path.getCommands();
	int command_count_prev = command.size();
	
	
	
	
	command_count_ = command.size() - command_count_prev;
}

void ShapeContentPoly::addStarProperty(Property<bool> *prop)
{
	prop->setTarget(&is_star_);
	addProperty(prop);
}

void ShapeContentPoly::addCornerCountProperty(Property<float> *prop)
{
	prop->setTarget(&corner_count_);
	addProperty(prop);
}

void ShapeContentPoly::addPositionProperty(Property<ofVec2f> *prop)
{
	prop->setTarget(&pos_);
	addProperty(prop);
}

void ShapeContentPoly::addRotationProperty(Property<float> *prop)
{
	prop->setTarget(&corner_count_);
	addProperty(prop);
}

void ShapeContentPoly::addOuterRadiusProperty(Property<float> *prop)
{
	prop->setTarget(&corner_count_);
	addProperty(prop);
}

void ShapeContentPoly::addOuterRoundnessProperty(Property<float> *prop)
{
	prop->setTarget(&corner_count_);
	addProperty(prop);
}

void ShapeContentPoly::addInnerRadiusProperty(Property<float> *prop)
{
	prop->setTarget(&corner_count_);
	addProperty(prop);
}

void ShapeContentPoly::addInnerRoundnessProperty(Property<float> *prop)
{
	prop->setTarget(&corner_count_);
	addProperty(prop);
}

/* =================== */

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

void ShapeContentStroke::addColorProperty(Property<ofFloatColor> *prop)
{
	prop->setTarget(&color_);
	addProperty(prop);
}

void ShapeContentStroke::addOpacityProperty(Property<float> *prop)
{
	prop->setTarget(&opacity_);
	addProperty(prop);
}

void ShapeContentStroke::addStrokeWidthProperty(Property<float> *prop)
{
	prop->setTarget(&stroke_width_);
	addProperty(prop);
}

/* =================== */

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

void ShapeContentFill::addColorProperty(Property<ofFloatColor> *prop)
{
	prop->setTarget(&color_);
	addProperty(prop);
}

void ShapeContentFill::addOpacityProperty(Property<float> *prop)
{
	prop->setTarget(&opacity_);
	addProperty(prop);
}

OFX_AE_NAMESPACE_END
/* EOF */
