#pragma once

#include "ofxAEDef.h"
#include "ofxAEAVLayer.h"
#include "regex.h"

OFX_AE_NAMESPACE_BEGIN

class SequenceLayer : public AVLayer
{
	friend class Loader;
public:
	SequenceLayer();
	~SequenceLayer();
	void setSequenceString(const string& str);
private:
	void setPropertyFrame(int frame);
	void render();
private:
	regex_t regex_;
	int digit_;
	int start_, end_;
	string before_, after_;
	ofTexture texture_;
};

OFX_AE_NAMESPACE_END
/* EOF */