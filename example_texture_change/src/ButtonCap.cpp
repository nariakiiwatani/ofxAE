#include "ButtonCap.h"
#include "ofxAEComposition.h"
#include "ofxAEAVLayer.h"
#include "ofEvents.h"

ButtonCap::ButtonCap(ofxAE::AVLayer *layer, ofxAE::Composition *comp)
:ofxAE::AVCap(layer)
,comp_(comp)
,state_(State(-1))
,pressed_before_(false)
,is_clicked_frame_(false)
{
	setState(STATE_NORMAL);
}
void ButtonCap::update()
{
	bool pressed = ofGetMousePressed();
	hitCheck(ofGetMouseX(), ofGetMouseY(), !pressed_before_&&pressed);
	pressed_before_ = pressed;
	switch(state_) {
		case STATE_OFF:
		case STATE_DO:
			if(comp_->isEnd()) {
				setState(STATE_NORMAL);
			}
			break;
		case STATE_NORMAL:
		case STATE_ON:
			break;
	}
}
bool ButtonCap::hitCheck(float x, float y, bool push)
{
	is_clicked_frame_ = false;
	bool hit = static_cast<ofxAE::AVLayer*>(layer_)->isHit(x,y);
	if(hit) {
		if(push) {
			switch(state_) {
				case STATE_NORMAL:
				case STATE_ON:
				case STATE_DO:
				case STATE_OFF:
					setState(STATE_DO);
					break;
			}
			is_clicked_frame_ = true;
		}
		else {
			switch(state_) {
				case STATE_NORMAL:
				case STATE_OFF:
					setState(STATE_ON);
					break;
				case STATE_ON:
				case STATE_DO:
					break;
			}
		}
	}
	else {
		switch(state_) {
			case STATE_ON:
				setState(STATE_OFF);
				break;
			case STATE_NORMAL:
			case STATE_DO:
			case STATE_OFF:
				break;
		}
	}
	return hit;
}

void ButtonCap::setState(State state)
{
	if(state == STATE_DO) {
		comp_->setActiveMarker(state);
	}
	else if(state != state_) {
		comp_->setActiveMarker(state);
	}
	state_ = state;
}

/* EOF */