#pragma once

#include "FrameCounter.h"
#include "ofTypes.h"

namespace ofxAE {
class Marker {
	friend class Loader;
public:
	void setupByComment(const string& comment);
	const string& getName() const { return name_; }
	int getFrom() const { return from_; }
	int getLength() const { return length_; }
	FrameCounter::LoopState getLoopState() const { return loop_; }
private:
	string name_;
	int from_;
	int length_;
	FrameCounter::LoopState loop_;
};
}

/* EOF */