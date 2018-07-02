#pragma once

#include "ofxAEDef.h"

OFX_AE_NAMESPACE_BEGIN

class Layer;

class LayerCap
{
public:
	virtual void setLayer(std::shared_ptr<Layer> layer);
	virtual void update(){}
	virtual void draw(float alpha=1){}
	virtual void setActive(bool active){}
protected:
	std::weak_ptr<Layer> layer_;
};


OFX_AE_NAMESPACE_END

/* EOF */
