#pragma once

#include "ofTexture.h"
#include "ofColor.h"
#include "ofCamera.h"

namespace ofxAE {
	class AVLayer;
	class CameraLayer;
	class Composition;
}
namespace ofxAE {

class LayerHelper_ {
public:
	virtual void update(){}
	virtual void render(){}
	virtual void setPropertyFrame(int frame){}
	virtual void resetPropertyFrame(){setPropertyFrame(0);}
};

template<class LayerClass>
class LayerHelper : public LayerHelper_ {
public:
	void setTarget(LayerClass *layer) { layer_ = layer; }
protected:
	LayerClass *layer_;
};
	
class SolidLayerHelper : public LayerHelper<AVLayer> {
	friend class Loader;
public:
	void render();
private:
	ofFloatColor color_;
};
class StillLayerHelper : public LayerHelper<AVLayer> {
	friend class Loader;
public:
	void loadImage(const string& filepath);
	void render();
private:
	ofTexture texture_;
};
class CompositionLayerHelper : public LayerHelper<AVLayer> {
	friend class Loader;
public:
	void update();
	void render();
	void setPropertyFrame(int frame);
private:
	Composition *composition_;
};
}

/* EOF */