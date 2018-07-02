#include "ofxAELayer.h"
#include "ofxAELayerCap.h"
#include "ofxAEMarker.h"

OFX_AE_NAMESPACE_BEGIN

void Layer::setCap(std::shared_ptr<LayerCap> cap)
{
	cap_ = cap;
	cap->setLayer(shared_from_this());
}

void Layer::setActive(bool active)
{
	if(cap_) {
		cap_->setActive(active);
	}
	is_active_ = active;
}

void Layer::setParent(std::shared_ptr<Layer> layer)
{
	parent_ = layer;
	if(layer) {
		getNode().setParent(&layer->getNode());
	}
}

void Layer::update()
{
	if(auto parent = parent_.lock()) {
		parent->update();
	}
	if(transform_.isDirty()) {
		transform_.refreshMatrix();
	}
	if(cap_) {
		cap_->update();
	}
}

void Layer::setParamByComment(const string &com)
{
	comment::extractParam(param_, com);
}
const string& Layer::getParam(const string& key)
{
	return comment::getParam(param_, key);
}


void Layer::setPropertyFrame(int frame)
{
	frame -= frame_offset_;
	if(frame_in_ <= frame && frame < frame_out_) {
		PropertyGroup::setFrame(frame);
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
