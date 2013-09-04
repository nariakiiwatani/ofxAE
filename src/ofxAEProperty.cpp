#include "ofxAEProperty.h"
#include "ofxAELayer.h"
#include "ofxAeMask.h"
#include "ofxAECameraLayer.h"

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

void LayerActiveProp::changedCallback(const bool& val)
{
	target_->setActive(val);
}
void LayerOpacityProp::changedCallback(const float& val)
{
	target_->setOpacity(val);
}
void LayerPositionProp::changedCallback(const ofVec3f& val)
{
	target_->setPosition(val);
}
void LayerScaleProp::changedCallback(const ofVec3f& val)
{
	target_->setScale(val);
}
void LayerRotationXProp::changedCallback(const float& val)
{
	target_->setRotationX(val);
}
void LayerRotationYProp::changedCallback(const float& val)
{
	target_->setRotationY(val);
}
void LayerRotationZProp::changedCallback(const float& val)
{
	target_->setRotationZ(val);
}
void LayerAnchorPointProp::changedCallback(const ofVec3f& val)
{
	target_->setAnchorPoint(val);
}
void LayerOrientationProp::changedCallback(const ofVec3f& val)
{
	target_->setOrientation(val);
}

template class Property_<Layer, bool>;
template class Property_<Layer, float>;
template class Property_<Layer, ofVec3f>;

void MaskVertexProp::changedCallback(const MaskShapeVertexArg& val)
{
	target_->setVertex(val.index, val.val);
}
void MaskInTangentProp::changedCallback(const MaskShapeInTangentArg& val)
{
	target_->setInTangent(val.index, val.val);
}
void MaskOutTangentProp::changedCallback(const MaskShapeOutTangentArg& val)
{
	target_->setOutTangent(val.index, val.val);
}
template class Property_<Mask, MaskShapeVertexArg>;
template class Property_<Mask, MaskShapeInTangentArg>;
template class Property_<Mask, MaskShapeOutTangentArg>;


void CameraLayerFovProp::changedCallback(const float& val)
{
	target_->setFov(val);
}

template class Property_<CameraLayer, float>;

}
/* EOF */