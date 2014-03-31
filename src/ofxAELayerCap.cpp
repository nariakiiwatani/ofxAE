#include "ofxAELayerCap.h"
#include "ofxAELayer.h"
#include "ofImage.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

LayerCap::LayerCap(Layer *layer)
:layer_(layer)
{
	layer_->addCap(this);
}

LayerCap::~LayerCap()
{
	layer_->removeCap(this);
}

OFX_AE_NAMESPACE_END
/* EOF */