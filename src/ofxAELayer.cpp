#include "ofxAELayer.h"
#include "ofxAELayerCap.h"
#include "ofxAEMarker.h"

OFX_AE_NAMESPACE_BEGIN

Layer::Layer()
:is_active_(true)
,opacity_(1)
,parent_(NULL)
,frame_(0)
{
}

void Layer::addCap(ofxAE::LayerCap *cap)
{
	cap_.push_back(cap);
}

void Layer::removeCap(ofxAE::LayerCap *cap)
{
	for(vector<LayerCap*>::iterator c = cap_.begin(); c != cap_.end(); ++c) {
		if(*c == cap) {
			cap_.erase(c);
			return;
		}
	}
	ofLog(OF_LOG_WARNING, "tried removing unexiting cap.");
}

void Layer::addProperty(ofxAE::PropertyBase *prop)
{
	properties_.push_back(prop);
}

void Layer::removeProperty(ofxAE::PropertyBase *prop)
{
	for(vector<PropertyBase*>::iterator p = properties_.begin(); p != properties_.end(); ++p) {
		if(prop == *p) {
			properties_.erase(p);
			return;
		}
	}
	ofLog(OF_LOG_WARNING, "tried removing unexiting property: %s", prop->getName().c_str());
}

void Layer::setActive(bool active)
{
	for(vector<LayerCap*>::iterator c = cap_.begin(); c != cap_.end(); ++c) {
		(*c)->setActive(active);
	}
	is_active_ = active;
}

void Layer::setParent(Layer *layer)
{
	parent_ = layer;
	if(layer) {
		getNode().setParent(&layer->getNode());
	}
}

void Layer::update()
{
	if(parent_) {
		parent_->update();
	}
	if(transform_.isDirty()) {
		transform_.refreshMatrix();
	}
	for(vector<LayerCap*>::iterator c = cap_.begin(); c != cap_.end(); ++c) {
		(*c)->update();
	}
}
	
void Layer::setPropertyFrame(int frame)
{
	frame -= frame_offset_;
	if(frame_in_ <= frame && frame < frame_out_) {
		for(vector<PropertyBase*>::iterator prop = properties_.begin(); prop != properties_.end(); ++prop) {
			if((*prop)->isEnable()) {
				(*prop)->setFrame(frame);
			}
		}
	}
	else {
		setActive(false);
	}
	frame_ = frame;
}


void Layer::addOpacityProperty(Property<float> *prop)
{
	prop->setTarget(&opacity_);
	addProperty(prop);
}
void Layer::addActiveProperty(Property<bool> *prop)
{
	prop->setCallback(this, &Layer::setActiveCallback);
//	prop->setTarget(&is_active_);
	addProperty(prop);
}
void Layer::addTransformProperty(TransformProperty *prop)
{
	prop->translation_.setCallback(&transform_, &TransformNode::setTranslation);
	prop->rotation_.setCallback(&transform_, &TransformNode::setRotation);
	prop->scale_.setCallback(&transform_, &TransformNode::setScale);
	prop->orientation_.setCallback(&transform_, &TransformNode::setOrientation);
	prop->anchor_point_.setCallback(&transform_, &TransformNode::setAnchorPoint);
	addProperty(prop);
}


OFX_AE_NAMESPACE_END
/* EOF */