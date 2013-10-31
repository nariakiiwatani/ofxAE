#pragma once

#include "FrameCounter.h"
#include "ofConstants.h"
#include <map>

namespace ofxAE {
class Marker {
	friend class Loader;
public:
	void setupByComment(const string& comment);
	const string& getName() { return name_; }
	int getFrom() { return from_; }
	int getLength() { return length_; }
	FrameCounter::LoopState getLoopState() { return loop_; }
	const string& getParam(const string& key);
private:
	string name_;
	int from_;
	int length_;
	std::map<string, string> param_;
	FrameCounter::LoopState loop_;
};
}

/* EOF */