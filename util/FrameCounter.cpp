#include "FrameCounter.h"

FrameCounter::FrameCounter()
:frame_(0)
,speed_(1)
,loop_(ONEWAY)
,first_(true)
,is_end_(false)
{}
int FrameCounter::update() {
	if(first_) {
		first_ = false;
		return frame_;
	}
	frame_ += speed_;
	if(speed_ < 0 && frame_ < from_) {
		switch(loop_) {
			case NONE:
				is_end_ = true;
				break;
			case ONEWAY:
				frame_ += length_;
				break;
			case PINGPONG:
				frame_ = 2*from_-frame_;
				speed_ = -speed_;
				break;
		}
	}
	if(speed_ > 0 && frame_ >= from_+length_) {
		switch(loop_) {
			case NONE:
				is_end_ = true;
				break;
			case ONEWAY:
				frame_ -= length_;
				break;
			case PINGPONG:
				frame_ = 2*(from_+length_-1)-frame_;
				speed_ = -speed_;
				break;
		}
	}
	return frame_;
}

/* EOF */