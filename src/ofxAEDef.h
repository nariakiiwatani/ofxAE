#pragma once

#define OFX_AE_NAMESPACE_BEGIN namespace ofxAE {
#define OFX_AE_NAMESPACE_END }

#include "ofConstants.h"
#include <map>

OFX_AE_NAMESPACE_BEGIN

namespace comment {
static const string NEW_LINE = "\\r\\n";
static const string EMPTY = "";

static void extractParam(map<string,string>& dst, const string& src, size_t pos=0)
{
	while(pos != string::npos) {
		size_t delim = src.find_first_of('=', pos);
		if(delim != string::npos) {
			string key = src.substr(pos, delim-pos);
			delim += 1;
			pos = src.find(NEW_LINE, delim);
			if(pos == string::npos) {
				dst[key] = src.substr(delim);
			}
			else {
				dst[key] = src.substr(delim,pos-delim);
				pos += NEW_LINE.length();
			}
		}
		else {
			return;
		}
	}
}
static const string& getParam(map<string,string>& param, const string& key)
{
	if(param.find(key) != param.end()) {
		return param[key];
	}
	return EMPTY;
}
}
OFX_AE_NAMESPACE_END

/* EOF */