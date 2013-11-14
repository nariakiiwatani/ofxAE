#pragma once

class FrameCounter {
public:
	FrameCounter();
	int update();
	enum LoopState {
		NONE,
		ONEWAY,
		PINGPONG,
	};
	void setLoopState(LoopState state) { loop_ = state; }
	void setRange(int from, int length) { from_ = from; length_ = length; }
	void setSpeed(float speed) { speed_ = speed; }
	void setFrame(int frame) { frame_ = frame; }
	void resetFrame(int frame) { frame_ = frame; first_ = true; }
	
	bool isEnd();
	int getLength() { return length_; }
private:
	float frame_;
	int from_, length_;
	LoopState loop_;
	float speed_;
	bool first_;
	bool is_end_;
};

/* EOF */