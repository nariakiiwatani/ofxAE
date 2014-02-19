#pragma once

#include "ofxAEDef.h"
#include "ofxAEAVLayer.h"
#ifdef TARGET_WIN32
#include <regex>
#else
#include "regex.h"
#endif

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
	void render(float alpha=1);
private:
#ifdef TARGET_WIN32
	regex regex_;
#else
	regex_t regex_;
#endif
	int digit_;
	int start_, end_;
	string before_, after_;
	ofTexture texture_;
	int prev_frame_;
};

OFX_AE_NAMESPACE_END
/* EOF */