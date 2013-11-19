/*!
*/
#pragma once

#include "ofMatrix4x4.h"
#include "ofVec3f.h"
#include "Hierarchical.h"

class TransformNode : public Hierarchical
{
public:
	enum RotationOrder {
		ROTATION_ORDER_XYZ,
		ROTATION_ORDER_YZX,
		ROTATION_ORDER_ZXY,
		ROTATION_ORDER_XZY,
		ROTATION_ORDER_YXZ,
		ROTATION_ORDER_ZYX,
	};
public:
	TransformNode();

	void pushMatrix();
	void popMatrix();
	
	const ofMatrix4x4* getWorldMatrix() const;
	const ofMatrix4x4* getLocalMatrix() const;
	
	void setTranslation(const ofVec3f& trans);
	void setTranslation(float x, float y, float z);
	void setTranslationX(float x);
	void setTranslationY(float y);
	void setTranslationZ(float z);
	void setOrientation(const ofVec3f& from, const ofVec3f& to);
	void setOrientation(float angle, const ofVec3f& axis);
	void setOrientation(float angle, float x, float y, float z);
	void setOrientation(const ofQuaternion& quat);
	void setOrientation(float angle1, const ofVec3f& axis1,
						float angle2, const ofVec3f& axis2,
						float angle3, const ofVec3f& axis3);
	void setRotation(const ofVec3f& rotation);
	void setRotation(float x, float y, float z);
	void setRotationX(float x);
	void setRotationY(float y);
	void setRotationZ(float z);
	void setScale(const ofVec3f& scale);
	void setScale(float x, float y, float z);
	void setScaleX(float x);
	void setScaleY(float y);
	void setScaleZ(float z);
	void setAnchorPoint(const ofVec3f& anchor);
	void setAnchorPoint(float x, float y, float z);
	void setAnchorPointX(float x);
	void setAnchorPointY(float y);
	void setAnchorPointZ(float z);
	
	void addTranslation(const ofVec3f& trans);
	void addTranslation(float x, float y, float z);
	void addTranslationX(float x);
	void addTranslationY(float y);
	void addTranslationZ(float z);
	void addOrientation(const ofVec3f& from, const ofVec3f& to);
	void addOrientation(float angle, const ofVec3f& axis);
	void addOrientation(float angle, float x, float y, float z);
	void addOrientation(const ofQuaternion& quat);
	void addOrientation(float angle1, const ofVec3f& axis1,
						float angle2, const ofVec3f& axis2,
						float angle3, const ofVec3f& axis3);
	void addRotation(const ofVec3f& rotation);
	void addRotation(float x, float y, float z);
	void addRotationX(float x);
	void addRotationY(float y);
	void addRotationZ(float z);
	void addScale(const ofVec3f& scale);
	void addScale(float x, float y, float z);
	void mulScale(const ofVec3f& scale);
	void mulScale(float x, float y, float z);
	void addAnchorPoint(const ofVec3f& anchor);
	void addAnchorPoint(float x, float y, float z);
	void addAnchorPointX(float x);
	void addAnchorPointY(float y);
	void addAnchorPointZ(float z);
	
	const ofVec3f& getAnchorPoint() { return anchor_point_; }

	void refreshMatrix();
	
	RotationOrder getRotationOrder() { return rotation_order_; }
private:
	void calcLocalMatrix();
	bool isLocalMatrixIdentity() const{return is_local_matrix_identity_;}
	bool isWorldMatrixIdentity() const{return is_world_matrix_identity_;}

private:
	const ofMatrix4x4* world_matrix_ptr_;
	ofMatrix4x4 world_matrix_;
	ofMatrix4x4 local_matrix_;
	ofVec3f translation_;
	ofQuaternion orientation_;
	ofVec3f scale_;
	ofVec3f anchor_point_;
	ofVec3f rotation_;
	RotationOrder rotation_order_;

	bool is_local_matrix_identity_;
	bool is_world_matrix_identity_;
};


/* EOF */