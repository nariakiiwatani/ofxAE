#include "ofxAEMarker.h"

namespace ofxAE {
static string NEW_LINE = "\\r\\n";
static string getParam(const string& src, const string& key, size_t pos)
{
	size_t len = key.length();
	size_t found = src.find(key, pos);
	if(found != string::npos) {
		pos = found+len;
		size_t end_pos = src.find(NEW_LINE, pos);
		if(end_pos == string::npos) {
			return src.substr(pos);
		}
		else {
			return src.substr(pos, end_pos-pos);
		}
	}
	return "";

}
void Marker::setupByComment(const string& comment)
{
	loop_ = FrameCounter::NONE;
	size_t pos = comment.find(NEW_LINE);
	if(pos == string::npos) {
		name_ = comment;
		return;
	}
	name_ = comment.substr(0, pos);
	pos += NEW_LINE.length();
	string loop = getParam(comment, "loop=", pos);
	if(loop != "") {
		if(loop == "oneway") {
			loop_ = FrameCounter::ONEWAY;
		}
		else if(loop == "pingpong") {
			loop_ = FrameCounter::PINGPONG;
		}
	}
}
}

/* EOF */