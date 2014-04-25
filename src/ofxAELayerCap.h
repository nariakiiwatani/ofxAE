#pragma once

#include "ofxAEDef.h"

OFX_AE_NAMESPACE_BEGIN

class Layer;

class LayerCap
{
public:
	LayerCap(Layer *layer);
	virtual ~LayerCap();
	virtual void update(){}
	virtual void draw(float alpha=1){}
	virtual void setActive(bool active){}
protected:
	Layer *layer_;
};


OFX_AE_NAMESPACE_END

/* EOF */