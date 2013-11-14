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
		return frame_;
	}
	frame_ += abs(speed_);
	return calcFrame(frame_);
}

int FrameCounter::calcFrame(int input)
{
	int frame = input;
	switch(loop_) {
		case LOOP_NONE:
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
	if(isForward() || isStable()) {
		frame += from_;
	}
	else /*if(isBackward())*/ {
		frame = from_+length_-1 - frame;
	}
	return frame;
}

bool FrameCounter::isEnd()
{
	return loop_==LOOP_NONE && ((speed_ < 0 && frame_ < from_) || (speed_ > 0 && frame_ >= from_+length_));
}

/* EOF */