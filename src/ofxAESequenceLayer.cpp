#include "ofxAESequenceLayer.h"
#include "regex.h"
#include "ofImage.h"
#include "ofGraphics.h"

OFX_AE_NAMESPACE_BEGIN

SequenceLayer::SequenceLayer()
{
	regcomp( &regex_, "(.*)\\[([0-9]+)-([0-9]+)](.+)", REG_EXTENDED );
}
SequenceLayer::~SequenceLayer()
{
    regfree(&regex_);
}
void SequenceLayer::setSequenceString(const string& str)
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
void SequenceLayer::setPropertyFrame(int frame)
{
	int image_frame = frame - start_frame_;
	if(0 <= image_frame && image_frame <= end_-start_) {
		ofLoadImage(texture_, before_+ofToString(min(start_+image_frame, end_), digit_, '0')+after_);
	}
	AVLayer::setPropertyFrame(frame);
}

void SequenceLayer::render(float alpha)
{
	ofPushStyle();
	ofSetColor(ofColor::white, opacity_*alpha*255);
	texture_.draw(0, 0, size_.x, size_.y);
	ofPopStyle();
}

OFX_AE_NAMESPACE_END
/* EOF */