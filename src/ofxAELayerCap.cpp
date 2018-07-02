#include "ofxAELayerCap.h"
#include "ofxAELayer.h"
#include "ofImage.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

void LayerCap::setLayer(std::shared_ptr<Layer> layer)
{
	layer_ = layer;
}

OFX_AE_NAMESPACE_END
/* EOF */
