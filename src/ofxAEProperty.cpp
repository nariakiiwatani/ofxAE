#include "ofxAEProperty.h"
#include "ofxAELayer.h"
#include "ofxAECameraLayer.h"
#include "ofxAEShapeLayer.h"

namespace ofxAE {
	
template<class Target, typename Type>
void Property_<Target,Type>::addKey(int frame, const Type& val)
{
	key_.insert(pair<int,Type>(frame,val));
}

template<class Target, typename Type>
void Property_<Target,Type>::setFrame(int frame)
{
	const Type& val = (--key_.upper_bound(frame))->second;
	if(val != prev_) {
		changedCallback(val);
		prev_ = val;
	}
}

template<class Target, typename Type>
void Property_<Target,Type>::resetFrame()
{
	const Type& val = key_.begin()->second;
	resetCallback(val);
	prev_ = val;
}

template class Property_<Layer, bool>;
template class Property_<Layer, float>;
template class Property_<Layer, ofVec3f>;
template class Property_<CameraLayer, float>;
template class Property_<ShapeContentStroke, ofVec3f>;
template class Property_<ShapeContentStroke, float>;
template class Property_<ShapeContentFill, ofVec3f>;
template class Property_<ShapeContentFill, float>;
template class Property_<ShapeContentEllipse, ofVec2f>;
template class Property_<ShapeContentRect, ofVec2f>;
template class Property_<ShapeContentRect, float>;
template class Property_<ShapeContentPoly, ofVec2f>;
template class Property_<ShapeContentPoly, float>;
template class Property_<ShapeContentGroup, ofVec2f>;
template class Property_<ShapeContentGroup, float>;

template class Property_<Path, PathShapeVertexArg>;
template class Property_<Path, PathShapeInTangentArg>;
template class Property_<Path, PathShapeOutTangentArg>;


}
/* EOF */