#include "ofxAESequenceCap.h"

OFX_AE_NAMESPACE_BEGIN

#ifdef TARGET_WIN32
SequenceCap::SequenceCap(AVLayer *layer)
:ImageCap(layer)
,regex_("(.*)\\[([0-9]+)-([0-9]+)\\](.+)")
,prev_frame_(-1)
{
}
SequenceCap::~SequenceCap()
{
}
void SequenceCap::setSequenceString(const string& str)
{
    cmatch matches;
	
    if(!regex_match(str.c_str(), matches, regex_))
    {
		ofLog(OF_LOG_WARNING, "invalid string");
        return;
    }
	int reg_size = matches.size();
	if(reg_size != 5) {
		ofLog(OF_LOG_WARNING, "invalid string");
        return;
    }
	before_ = matches[1].str();
	start_ = ofToInt(matches[2].str());
	digit_ = matches[2].length();
	end_ = ofToInt(matches[3].str());
	after_ = matches[4].str();
}
#else
SequenceCap::SequenceCap(AVLayer *layer)
:ImageCap(layer)
,prev_frame_(-1)
{
	regcomp( &regex_, "(.*)\\[([0-9]+)-([0-9]+)](.+)", REG_EXTENDED );
}
SequenceCap::~SequenceCap()
{
    regfree(&regex_);
}
void SequenceCap::setSequenceString(const string& str)
{
	const int reg_size = 5;
    regmatch_t matches[reg_size];
	
    if( regexec( &regex_, str.c_str(), reg_size, matches, 0 ) != 0 )
    {
		ofLog(OF_LOG_WARNING, "invalid string");
        return;
    }
    for( int i = 0; i < reg_size; ++i )
    {
        if( matches[i].rm_so == -1 || matches[i].rm_eo == -1 )
        {
			ofLog(OF_LOG_WARNING, "invalid string");
            return;
        }
    }
	regmatch_t& match = matches[1];
	before_ = str.substr(match.rm_so, match.rm_eo - match.rm_so);
	match = matches[2];
	start_ = ofToInt(str.substr(match.rm_so, match.rm_eo - match.rm_so));
	digit_ = match.rm_eo - match.rm_so;
	match = matches[3];
	end_ = ofToInt(str.substr(match.rm_so, match.rm_eo - match.rm_so));
	match = matches[4];
	after_ = str.substr(match.rm_so, match.rm_eo - match.rm_so);
}
#endif
void SequenceCap::setPropertyFrame(int frame)
{
	int image_frame = frame - start_frame_;
	if(prev_frame_ != image_frame && 0 <= image_frame && image_frame <= end_-start_) {
		loadImage(before_+ofToString(min(start_+image_frame, end_), digit_, '0')+after_);
		prev_frame_ = image_frame;
	}
}

OFX_AE_NAMESPACE_END
/* EOF */