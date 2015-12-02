#include "ofxAEMovieCap.h"
#include "ofGraphics.h"
#include "ofxAELayer.h"
#include "ofAppRunner.h"
#include "ofxAEComposition.h"

OFX_AE_NAMESPACE_BEGIN

MovieCap::MovieCap(AVLayer *layer)
:AVCap(layer)
,frame_rate_(ofGetTargetFrameRate())
,comp_(NULL)
,use_audio_(true)
{
	const string &lazy = layer_->getParam("lazy");
	if(lazy == "") {
		lazy_seconds_ = 1;
	}
	else {
		lazy_seconds_ = ofToFloat(lazy);
	}
}

void MovieCap::loadMovie(const string& filepath)
{
	movie_.load(filepath);
	movie_.setLoopState(OF_LOOP_NONE);
	frame_rate_ = movie_.getTotalNumFrames()/movie_.getDuration();
}

void MovieCap::setActive(bool active)
{
	active = use_audio_&&active;
	if(active != movie_.isPlaying()) {
		active?movie_.play():movie_.stop();
	}
}

void MovieCap::setUseAudio(bool use_audio)
{
	use_audio_ = use_audio;
	setActive(layer_->isActive());
}

void MovieCap::update()
{
	if(comp_) {
		float movie_position, app_position;
		if(comp_->isBackward()) {
			movie_.setSpeed(-comp_->getSpeed());
			movie_position = movie_.getPosition()*movie_.getDuration();
			app_position = movie_.getDuration()-layer_->getFrame()/comp_->getFrameRate();
		}
		else {
			movie_.setSpeed(comp_->getSpeed());
			movie_position = movie_.getPosition()*movie_.getDuration();
			app_position = layer_->getFrame()/comp_->getFrameRate();
		}
		if(!use_audio_ || abs(app_position-movie_position) > abs(comp_->getSpeed()*lazy_seconds_)) {
			movie_.setPosition(app_position/movie_.getDuration());
		}
	}
	movie_.update();
}

void MovieCap::draw(float alpha)
{
	ofPushStyle();
	ofEnableBlendMode(blend_mode_);
	ofSetColor(ofColor::white, opacity_*alpha*255);
	movie_.draw(rect_);
	ofPopStyle();
}

OFX_AE_NAMESPACE_END

/* EOF */