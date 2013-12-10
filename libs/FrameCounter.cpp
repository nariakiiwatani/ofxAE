#include "FrameCounter.h"
#include "ofMath.h"

FrameCounter::FrameCounter()
:frame_(0)
,speed_(1)
,loop_(LOOP_NONE)
,is_backward_(false)
,first_(true)
{}

void FrameCounter::setFrame(int frame)
{
	frame_ = frame;
	frame_internal_ = calcInternalFrame(frame);
	first_ = false;
}
int FrameCounter::update() {
	if(first_) {
		first_ = false;
	}
	else {
		frame_ += speed_;
	}
	frame_internal_ = calcInternalFrame(frame_);
	return getCurrent();
}

int FrameCounter::getCurrent()
{
	int ret = frame_internal_;
	if(isForward()) {
		ret += from_;
	}
	else /*if(isBackward())*/ {
		ret = from_+length_-1 - ret;
	}
	return ret;
}

static int calcLoopedFrame(int frame, int length)
{
	if(frame < 0) {
		frame += (1-frame/length)*(length);
	}
	else {
		frame %= length;
	}
	return frame;
}
int FrameCounter::calcInternalFrame(int input)
{
	int frame = input;
	switch(loop_) {
		case LOOP_NONE:
			frame = ofClamp(frame, 0, length_-1);
			break;
		case LOOP_ONEWAY:
			frame = calcLoopedFrame(frame, length_);
			break;
		case LOOP_PINGPONG:
			frame = calcLoopedFrame(frame, (length_-1)*2);
			if(frame > length_-1) {
				frame = 2*(length_-1)-frame;
			}
			break;
	}
	return frame;
}

void FrameCounter::setBackward(bool backward)
{
	if(is_backward_ != backward) {
		switch(loop_) {
			case LOOP_NONE:
				frame_ = length_-frame_;
				break;
			case LOOP_ONEWAY: {
				int looped = (frame_/length_) * length_;
				frame_ = length_-(frame_-looped)+looped;
			}	break;
			case LOOP_PINGPONG:
				int looped = (frame_/(length_-1)*2) * (length_-1)*2;
				frame_ = length_-(frame_-looped)+looped;
				break;
		}
	}
	is_backward_ = backward;
}

bool FrameCounter::isEnd()
{
	return loop_==LOOP_NONE && (frame_internal_ == length_-1);
}

/* EOF */