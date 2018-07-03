#include "ofxAELayer.h"
#include "ofxAELayerCap.h"
#include "ofxAEMarker.h"

using namespace std;

OFX_AE_NAMESPACE_BEGIN

Layer::Layer()
{
	addProperty<float>("opacity")->setTarget(&opacity_);
	addProperty<bool>("active")->setCallback(this, &Layer::setActiveCallback);
	
	auto transform = add<TransformProperty>("transform");
	transform->getTranslation()->setCallback(&transform_, &TransformNode::setTranslation);
	transform->getRotation()->setCallback(&transform_, &TransformNode::setRotation);
	transform->getScale()->setCallback(&transform_, &TransformNode::setScale);
	transform->getOrientation()->setCallback(&transform_, &TransformNode::setOrientation);
	transform->getAnchorPoint()->setCallback(&transform_, &TransformNode::setAnchorPoint);
}

void Layer::setCap(shared_ptr<LayerCap> cap)
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

void Layer::setParent(shared_ptr<Layer> layer)
{
	parent_ = layer;
	if(layer) {
		getNode().setParent(&layer->getNode());
	}
}

shared_ptr<Marker> Layer::addNewMarker()
{
	auto marker = make_shared<Marker>();
	addMarker(marker);
	return marker;
}

void Layer::addMarker(shared_ptr<Marker> marker)
{
	marker_.push_back(marker);
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

OFX_AE_NAMESPACE_END
/* EOF */
