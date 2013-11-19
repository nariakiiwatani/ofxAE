#include "FrameCounter.h"
#include "ofMath.h"

FrameCounter::FrameCounter()
:frame_(0)
,speed_(1)
,loop_(LOOP_NONE)
,first_(true)
{}
int FrameCounter::update() {
	if(first_) {
		first_ = false;
	}
	else {
		frame_ += abs(speed_);
	}
	frame_internal_ = calcInternalFrame(frame_);
	return getCurrent();
}

int FrameCounter::getCurrent()
{
	int ret = frame_internal_;
	if(isForward() || isStable()) {
		ret += from_;
	}
	else /*if(isBackward())*/ {
		ret = from_+length_-1 - ret;
	}
	return ret;
}

int FrameCounter::calcInternalFrame(int input)
{
	int frame = input;
	switch(loop_) {
		case LOOP_NONE:
			frame = ofClamp(frame, 0, length_-1);
			break;
		case LOOP_ONEWAY:
			frame %= length_;
			break;
		case LOOP_PINGPONG:
			frame %= (length_-1)*2;
			if(frame > length_-1) {
				frame = 2*(length_-1)-frame;
			}
			break;
	}
	return frame;
}

bool FrameCounter::isEnd()
{
	return loop_==LOOP_NONE && (frame_internal_ == length_-1);
}

/* EOF */