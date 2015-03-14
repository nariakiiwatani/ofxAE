#pragma once

#include "ofxAEDef.h"
#include "ofConstants.h"
#include "FrameCounter.h"
#include <map>

OFX_AE_NAMESPACE_BEGIN

class Marker {
	friend class Loader;
public:
	Marker();
	void setupByComment(const string& com);
	const string& getName() { return name_; }
	int getFrom() { return from_; }
	int getLength() { return length_; }
	const string& getParam(const string& key);
	FrameCounter::LoopState getLoopState() { return loop_; }
private:
	string name_;
	int from_;
	int length_;
	FrameCounter::LoopState loop_;
	std::map<string, string> param_;
};

OFX_AE_NAMESPACE_END
/* EOF */