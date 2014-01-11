#pragma once

class FrameCounter {
public:
	enum LoopState {
		LOOP_NONE,
		LOOP_ONEWAY,
		LOOP_PINGPONG,
	};
	FrameCounter();
	int update();
	int getCurrent();
	void setLoopState(LoopState state) { loop_ = state; }
	void setRange(int from, int length) { from_ = from; length_ = length; }
	void setSpeed(float speed) { speed_ = speed; }
	void setBackward(bool backward);
	void setFrame(int frame);
	void resetFrame(int frame) { frame_ = frame; first_ = true; }
	
	bool isEnd();
	bool isForward() { return !is_backward_; }
	bool isBackward() { return is_backward_; }
	bool isStable() { return speed_ == 0; }
	int getLength() { return length_; }
	int getFrom() { return from_; }

private:
	float frame_;
	int frame_internal_;
	int from_, length_;
	LoopState loop_;
	bool is_backward_;
	float speed_;
	bool first_;

private:
	int calcInternalFrame(int input);
};

/* EOF */