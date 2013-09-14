#pragma once

#include "ofEvents.h"
#include <map>
#include "ofxAELayer.h"
#include "ofxAECameraLayer.h"
#include "ofxAEShapeLayer.h"
#include "ofxAEMask.h"

namespace ofxAE {
	class Mask;
	class ShapeContentEllipse;
	class ShapeContentStroke;
}

namespace ofxAE {

class PropertyBase_ {
public:
	virtual ~PropertyBase_(){}
	virtual void resetFrame()=0;
	virtual void setFrame(int frame)=0;
};
	
template<class Target, typename Type>
class Property_ : public PropertyBase_ {
	friend class Loader;
public:
	void setTarget(Target *target) { target_=target; }
	void addKey(int frame, const Type& val);
	
	void setFrame(int frame);
	void resetFrame();
protected:
	Target *target_;
	virtual void changedCallback(const Type& val)=0;
	virtual void resetCallback(const Type& val){changedCallback(val);}
private:
	map<int,Type> key_;
	Type prev_;
};

#define PropDeclare(Name,Type,Target,func) \
class Name : public Property_<Target, Type> { \
private: void changedCallback(const Type& val){target_->func(val);}}
	PropDeclare(LayerActiveProp,bool,Layer,setActive);
	PropDeclare(LayerOpacityProp,float,Layer,setOpacity);
	PropDeclare(LayerPositionProp,ofVec3f,Layer,setPosition);
	PropDeclare(LayerScaleProp,ofVec3f,Layer,setScale);
	PropDeclare(LayerRotationXProp,float,Layer,setRotationX);
	PropDeclare(LayerRotationYProp,float,Layer,setRotationY);
	PropDeclare(LayerRotationZProp,float,Layer,setRotationZ);
	PropDeclare(LayerAnchorPointProp,ofVec3f,Layer,setAnchorPoint);
	PropDeclare(LayerOrientationProp,ofVec3f,Layer,setOrientation);
	PropDeclare(CameraLayerFovProp,float,CameraLayer,setFov);
	PropDeclare(ShapeEllipsePositionProp,ofVec2f,ShapeContentEllipse,setPosition);
	PropDeclare(ShapeEllipseSizeProp,ofVec2f,ShapeContentEllipse,setSize);
	PropDeclare(ShapeStrokeOpacityProp,float,ShapeContentStroke,setOpacity);
	PropDeclare(ShapeStrokeWidthProp,float,ShapeContentStroke,setWidth);
	PropDeclare(ShapeGroupPositionProp,ofVec2f,ShapeContentGroup,setPosition);
	PropDeclare(ShapeGroupAnchorPointProp,ofVec2f,ShapeContentGroup,setAnchorPoint);
	PropDeclare(ShapeGroupScaleProp,ofVec2f,ShapeContentGroup,setScale);
	PropDeclare(ShapeGroupRotationProp,float,ShapeContentGroup,setRotation);
	PropDeclare(ShapeGroupOpacityProp,float,ShapeContentGroup,setOpacity);
	PropDeclare(ShapeGroupSkewProp,float,ShapeContentGroup,setSkew);
	PropDeclare(ShapeGroupSkewAxisProp,float,ShapeContentGroup,setSkewAxis);
#undef PropDeclare
class ShapeStrokeColorProp : public Property_<ShapeContentStroke,ofVec3f>
{
private:
	void changedCallback(const ofVec3f& color) {
		target_->setColor(ofFloatColor(color.x,color.y,color.z));
	}
};

#define MaskShapeDefine(name) \
	struct name{int index;ofVec2f val;name(){};name(int i, const ofVec2f& v) {index=i;val=v;}\
		bool operator!=(const name& op) const {return !(index==op.index&&val==op.val);};}
	MaskShapeDefine(MaskShapeVertexArg);
	MaskShapeDefine(MaskShapeInTangentArg);
	MaskShapeDefine(MaskShapeOutTangentArg);
#undef MaskShapeDefine

class MaskVertexProp : public Property_<Mask, MaskShapeVertexArg> {
private:
	void changedCallback(const MaskShapeVertexArg& val) {
		target_->setVertex(val.index, val.val);
	}
};
class MaskInTangentProp : public Property_<Mask, MaskShapeInTangentArg> {
private:
	void changedCallback(const MaskShapeInTangentArg& val) {
		target_->setInTangent(val.index, val.val);
	}
};
class MaskOutTangentProp : public Property_<Mask, MaskShapeOutTangentArg> {
private:
	void changedCallback(const MaskShapeOutTangentArg& val) {
		target_->setOutTangent(val.index, val.val);
	}
};

}

/* EOF */