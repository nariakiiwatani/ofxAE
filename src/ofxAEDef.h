#pragma once

#define OFX_AE_NAMESPACE_BEGIN namespace ofxAE {
#define OFX_AE_NAMESPACE_END }

#include "ofConstants.h"
#include <map>

OFX_AE_NAMESPACE_BEGIN

namespace comment {
static const std::string NEW_LINE = "\\r\\n";
static const std::string EMPTY = "";

static void extractParam(std::map<std::string,std::string>& dst, const std::string& src, std::size_t pos=0)
{
	while(pos != std::string::npos) {
		std::size_t delim = src.find_first_of('=', pos);
		if(delim != std::string::npos) {
			std::string key = src.substr(pos, delim-pos);
			delim += 1;
			pos = src.find(NEW_LINE, delim);
			if(pos == std::string::npos) {
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
static const std::string& getParam(std::map<std::string,std::string>& param, const std::string& key)
{
	if(param.find(key) != end(param)) {
		return param[key];
	}
	return EMPTY;
}
}
OFX_AE_NAMESPACE_END

/* EOF */
