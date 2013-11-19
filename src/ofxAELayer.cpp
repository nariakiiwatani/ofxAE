#include "ofxAELayer.h"
#include "ofGraphics.h"
#include "ofxAEMask.h"
#include "ofxAEProperty.h"
#include "ofxAEMarker.h"

namespace ofxAE {
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
	properties_.push_back(&active_);
	properties_.push_back(&opacity_);
	properties_.push_back(&transform_);
}
	
void Layer::setParent(Layer *layer)
{
	getNode().setParent(&layer->getNode());
}

void Layer::update()
{
	for(vector<PropertyBase*>::iterator prop = properties_.begin(); prop != properties_.end(); ++prop) {
		if((*prop)->isDirty()) {
			(*prop)->update();
		}
	}
	prepare();
}
	
void Layer::setPropertyFrame(int frame)
{
	for(vector<PropertyBase*>::iterator prop = properties_.begin(); prop != properties_.end(); ++prop) {
		(*prop)->setFrame(frame);
	}
}
}
/* EOF */