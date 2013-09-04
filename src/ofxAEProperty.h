#pragma once

#include "ofEvents.h"
#include <map>

namespace ofxAE {
	class Layer;
	class Mask;
	class CameraLayer;
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

class LayerActiveProp : public Property_<Layer, bool> {
private:
	void changedCallback(const bool& val);
};
class LayerOpacityProp : public Property_<Layer, float> {
private:
	void changedCallback(const float& val);
};
class LayerPositionProp : public Property_<Layer, ofVec3f> {
private:
	void changedCallback(const ofVec3f& val);
};
class LayerScaleProp : public Property_<Layer, ofVec3f> {
private:
	void changedCallback(const ofVec3f& val);
};
class LayerRotationXProp : public Property_<Layer, float> {
private:
	void changedCallback(const float& val);
};
class LayerRotationYProp : public Property_<Layer, float> {
private:
	void changedCallback(const float& val);
};
class LayerRotationZProp : public Property_<Layer, float> {
private:
	void changedCallback(const float& val);
};
class LayerAnchorPointProp : public Property_<Layer, ofVec3f> {
private:
	void changedCallback(const ofVec3f& val);
};
class LayerOrientationProp : public Property_<Layer, ofVec3f> {
private:
	void changedCallback(const ofVec3f& val);
};

#define MaskShapeDefine(name) \
	struct name{int index;ofVec2f val;name(){};name(int i, const ofVec2f& v) {index=i;val=v;}\
		bool operator!=(const name& op) const {return !(index==op.index&&val==op.val);};};
	MaskShapeDefine(MaskShapeVertexArg)
	MaskShapeDefine(MaskShapeInTangentArg)
	MaskShapeDefine(MaskShapeOutTangentArg)
#undef MaskShapeDefine

class MaskVertexProp : public Property_<Mask, MaskShapeVertexArg> {
private:
	void changedCallback(const MaskShapeVertexArg& val);
};
class MaskInTangentProp : public Property_<Mask, MaskShapeInTangentArg> {
private:
	void changedCallback(const MaskShapeInTangentArg& val);
};
class MaskOutTangentProp : public Property_<Mask, MaskShapeOutTangentArg> {
private:
	void changedCallback(const MaskShapeOutTangentArg& val);
};

class CameraLayerFovProp : public Property_<CameraLayer, float> {
private:
	void changedCallback(const float& val);
};

}

/* EOF */