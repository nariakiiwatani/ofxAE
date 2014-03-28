#include "ofxAEAVLayerCap.h"
#include "ofxAEAVLayer.h"

OFX_AE_NAMESPACE_BEGIN

AVCap::AVCap(AVLayer *layer)
:LayerCap(layer)
,rect_(0,0,layer->getWidth(),layer->getHeight())
,blend_mode_(layer->getBlendMode())
,opacity_(1)
{
}

OFX_AE_NAMESPACE_END
/* EOF */