#pragma once

#include "ofxAEDef.h"
#include "ofConstants.h"
#include <map>

OFX_AE_NAMESPACE_BEGIN

class Marker {
	friend class Loader;
public:
	void setupByComment(const string& comment);
	const string& getName() { return name_; }
	int getFrom() { return from_; }
	int getLength() { return length_; }
	const string& getParam(const string& key);
private:
	string name_;
	int from_;
	int length_;
	std::map<string, string> param_;
};

OFX_AE_NAMESPACE_END
/* EOF */