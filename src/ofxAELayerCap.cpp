#include "ofxAELayerCap.h"
#include "ofxAELayer.h"
#include "ofImage.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

LayerCap::LayerCap(Layer *layer)
:layer_(layer)
{
	layer_->setCap(this);
}

LayerCap::~LayerCap()
{
	if(this == layer_->getCap()) {
		layer_->setCap(NULL);
	}
}

OFX_AE_NAMESPACE_END
/* EOF */