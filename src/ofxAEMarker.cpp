#include "ofxAEMarker.h"

using namespace std;

OFX_AE_NAMESPACE_BEGIN

Marker::Marker()
:name_("")
,from_(0)
,length_(1)
,loop_(FrameCounter::LOOP_NONE)
{}

void Marker::setupByComment(const string& com)
{
	size_t pos = com.find(comment::NEW_LINE);
	if(pos == string::npos) {
		name_ = com;
		return;
	}
	name_ = com.substr(0, pos);
	comment::extractParam(param_, com, pos+comment::NEW_LINE.length());
	loop_ = FrameCounter::LOOP_NONE;
	const string& loop = getParam("loop");
	if(loop != "") {
		if(loop == "oneway") {
			loop_ = FrameCounter::LOOP_ONEWAY;
		}
		else if(loop == "pingpong") {
			loop_ = FrameCounter::LOOP_PINGPONG;
		}
		else if(loop == "random") {
			loop_ = FrameCounter::LOOP_RANDOM;
		}
	}
}

const string& Marker::getParam(const string& key)
{
	return comment::getParam(param_, key);
}

OFX_AE_NAMESPACE_END
/* EOF */
