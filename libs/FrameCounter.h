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
	void setLoopState(LoopState state) { loop_ = state; }
	void setRange(int from, int length) { from_ = from; length_ = length; }
	void setSpeed(float speed) { speed_ = speed; }
	void setFrame(int frame) { frame_ = frame; }
	void resetFrame(int frame) { frame_ = frame; first_ = true; }
	
	bool isEnd();
	bool isForward() { return speed_ > 0; }
	bool isBackward() { return speed_ < 0; }
	bool isStable() { return speed_ == 0; }
	int getLength() { return length_; }

private:
	float frame_;
	int frame_internal_;
	int from_, length_;
	LoopState loop_;
	float speed_;
	bool first_;

private:
	int calcInternalFrame(int input);
};

/* EOF */