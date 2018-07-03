#pragma once

#include "ofxAEDef.h"
#include "ofxAEImageCap.h"
#ifdef TARGET_WIN32
#include <regex>
#else
#include "regex.h"
#endif

OFX_AE_NAMESPACE_BEGIN

class SequenceCap : public ImageCap
{
public:
	SequenceCap();
	~SequenceCap();
	void setSequenceString(const std::string& str);
	void update();
private:
#ifdef TARGET_WIN32
	regex regex_;
#else
	regex_t regex_;
#endif
	int digit_;
	int start_, end_;
	std::string before_, after_;
	int prev_frame_=-1;
};

OFX_AE_NAMESPACE_END
/* EOF */
