#include "ofxAEMovieCap.h"
#include "ofGraphics.h"
#include "ofxAEAVLayer.h"
#include "ofAppRunner.h"
#include "ofxAEComposition.h"

using namespace std;

OFX_AE_NAMESPACE_BEGIN

MovieCap::MovieCap()
:frame_rate_(ofGetTargetFrameRate())
{
}

void MovieCap::setLayer(shared_ptr<AVLayer> layer)
{
	AVCap::setLayer(layer);
	const string &lazy = layer->getParam("lazy");
	if(lazy != "") {
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
	if(auto layer = layer_.lock()) {
		setActive(layer->isActive());
	}
}

void MovieCap::update()
{
	if(comp_) {
		if(comp_->isBackward()) {
			movie_.setSpeed(-comp_->getSpeed());
		}
		else {
			movie_.setSpeed(comp_->getSpeed());
		}
		if(auto layer = layer_.lock()) {
			float movie_position = movie_.getPosition()*movie_.getDuration();
			float app_position = layer->getFrame()/comp_->getFrameRate();
			if(comp_->isBackward()) {
				app_position = movie_.getDuration() - app_position;
			}
			if(!use_audio_ || abs(app_position-movie_position) > abs(comp_->getSpeed()*lazy_seconds_)) {
				movie_.setPosition(app_position/movie_.getDuration());
			}
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
