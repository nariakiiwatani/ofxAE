#pragma once

#include "ofxAEAVLayerCap.h"

namespace ofxAE {
	class Composition;
}

class ButtonCap : public ofxAE::AVCap
{
public:
	enum State {
		STATE_NORMAL,
		STATE_ON,
		STATE_DO,
		STATE_OFF
	};
	ButtonCap(ofxAE::AVLayer *layer, ofxAE::Composition *comp);
	void update();
	bool hitCheck(float x, float y, bool push);
	void setState(State state);
	bool isClicked() { return is_clicked_frame_; }
private:
	ofxAE::Composition *comp_;
private:
	State state_;
	bool pressed_before_;
	bool is_clicked_frame_;
};

/* EOF */