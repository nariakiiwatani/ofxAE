#include "ofxAEAVLayerCap.h"
#include "ofxAEAVLayer.h"

OFX_AE_NAMESPACE_BEGIN

void AVCap::setLayer(std::shared_ptr<Layer> layer)
{
	LayerCap::setLayer(layer);
	if(auto av = std::dynamic_pointer_cast<AVLayer>(layer)) {
		setLayer(av);
	}
}

void AVCap::setLayer(std::shared_ptr<AVLayer> layer)
{
	rect_.set(0,0,layer->getWidth(),layer->getHeight());
	blend_mode_ = layer->getBlendMode();
	is_collapse_ = layer->isCollapse();
}

OFX_AE_NAMESPACE_END
/* EOF */
