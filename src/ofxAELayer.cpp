#include "ofxAELayer.h"
#include "ofxAEProperty.h"
#include "ofxAELayerCap.h"

OFX_AE_NAMESPACE_BEGIN

Layer::~Layer()
{
	for(vector<Marker*>::iterator it = marker_.begin(); it != marker_.end(); ++it) {
		delete *it;
	}
}
	
Layer::Layer()
{
	active_ = true;
	opacity_ = 1;
	parent_ = NULL;
	addProperty(&active_);
	addProperty(&opacity_);
	addProperty(&transform_);
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

void Layer::addProperty(ofxAE::PropertyBase *property)
{
	properties_.push_back(property);
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
	for(vector<PropertyBase*>::iterator prop = properties_.begin(); prop != properties_.end(); ++prop) {
		if((*prop)->isDirty()) {
			(*prop)->update();
		}
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
		for(vector<LayerCap*>::iterator c = cap_.begin(); c != cap_.end(); ++c) {
			(*c)->setPropertyFrame(frame);
		}
	}
	else {
		setActive(false);
	}
}

OFX_AE_NAMESPACE_END
/* EOF */