#include "ofxAEMarker.h"

namespace ofxAE {
static const string NEW_LINE = "\\r\\n";
static const string EMPTY = "";
static void extractParam(map<string,string>& dst, const string& src, size_t pos)
{
	while(pos != string::npos) {
		size_t delim = src.find_first_of('=', pos);
		if(delim != string::npos) {
			string key = src.substr(pos, delim-pos);
			delim += 1;
			pos = src.find(NEW_LINE, delim);
			if(pos == string::npos) {
				dst.insert(pair<string,string>(key, src.substr(delim)));
			}
			else {
				dst.insert(pair<string,string>(key, src.substr(delim,pos-delim)));
				pos += NEW_LINE.length();
			}
		}
	}
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
	extractParam(param_, comment, pos+NEW_LINE.length());
}

const string& Marker::getParam(const string& key)
{
	if(param_.find(key) != param_.end()) {
		return param_[key];
	}
	return EMPTY;
}
}

/* EOF */