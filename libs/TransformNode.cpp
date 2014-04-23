/*!
*/

#include "TransformNode.h"

TransformNode::TransformNode()
:translation_()
,rotation_(0,0,0)
,rotation_order_(ROTATION_ORDER_ZYX)
,orientation_()
,scale_(1,1,1)
,world_matrix_()
,local_matrix_()
,world_matrix_ptr_(&world_matrix_)
,world_matrix_inversed_()
,world_matrix_inversed_ptr_(&world_matrix_inversed_)
,local_matrix_inversed_()
,local_matrix_inversed_ptr_(&local_matrix_inversed_)
,is_local_matrix_identity_(true)
,is_world_matrix_identity_(true)
{
}

void TransformNode::refreshMatrix()
{
	if(isDirty(LOCAL)) {
		calcLocalMatrix();
		clsDirtyFlag(LOCAL);
		if(isLocalMatrixIdentity()) {
			local_matrix_inversed_ptr_ = &local_matrix_;
		}
		else {
			local_matrix_inversed_.makeInvertOf(local_matrix_);
			local_matrix_inversed_ptr_ = &local_matrix_inversed_;
		}
	}
	if(TransformNode* parent = static_cast<TransformNode*>(getParent())) {
		if(isDirty(PARENT)) {
			parent->refreshMatrix();
			clsDirtyFlag(PARENT);
		}
		if(parent->isWorldMatrixIdentity()) {
			world_matrix_ptr_ = &local_matrix_;
			is_world_matrix_identity_ = is_local_matrix_identity_;
		}
		else {
			if(isLocalMatrixIdentity()) {
				world_matrix_ptr_ = parent->getWorldMatrix();
			}
			else {
				world_matrix_.makeFromMultiplicationOf(local_matrix_, *parent->getWorldMatrix());
				world_matrix_ptr_ = &world_matrix_;
			}
			is_world_matrix_identity_ = false;
		}
	}
	else {
		world_matrix_ptr_ = &local_matrix_;
		is_world_matrix_identity_ = is_local_matrix_identity_;
	}
	if(isWorldMatrixIdentity()) {
		world_matrix_inversed_ptr_ = world_matrix_ptr_;
	}
	else {
		world_matrix_inversed_.makeInvertOf(*world_matrix_ptr_);
		world_matrix_inversed_ptr_ = &world_matrix_inversed_;
	}
}

void TransformNode::calcLocalMatrix()
{
	local_matrix_.makeTranslationMatrix(-anchor_point_);
	local_matrix_.scale(scale_);
	switch(rotation_order_) {
		case ROTATION_ORDER_XYZ:
			local_matrix_.rotate(rotation_.x, 1,0,0);
			local_matrix_.rotate(rotation_.y, 0,1,0);
			local_matrix_.rotate(rotation_.z, 0,0,1);
			break;
		case ROTATION_ORDER_YZX:
			local_matrix_.rotate(rotation_.y, 0,1,0);
			local_matrix_.rotate(rotation_.z, 0,0,1);
			local_matrix_.rotate(rotation_.x, 1,0,0);
			break;
		case ROTATION_ORDER_ZXY:
			local_matrix_.rotate(rotation_.z, 0,0,1);
			local_matrix_.rotate(rotation_.x, 1,0,0);
			local_matrix_.rotate(rotation_.y, 0,1,0);
			break;
		case ROTATION_ORDER_XZY:
			local_matrix_.rotate(rotation_.x, 1,0,0);
			local_matrix_.rotate(rotation_.z, 0,0,1);
			local_matrix_.rotate(rotation_.y, 0,1,0);
			break;
		case ROTATION_ORDER_YXZ:
			local_matrix_.rotate(rotation_.y, 0,1,0);
			local_matrix_.rotate(rotation_.x, 1,0,0);
			local_matrix_.rotate(rotation_.z, 0,0,1);
			break;
		case ROTATION_ORDER_ZYX:
			local_matrix_.rotate(rotation_.z, 0,0,1);
			local_matrix_.rotate(rotation_.y, 0,1,0);
			local_matrix_.rotate(rotation_.x, 1,0,0);
			break;
	}
	local_matrix_.rotate(orientation_);
	local_matrix_.translate(translation_);
	is_local_matrix_identity_ = local_matrix_.isIdentity();
}


void TransformNode::pushMatrix()
{
	glPushMatrix();
	glMultMatrixf(world_matrix_ptr_->getPtr());
}

void TransformNode::popMatrix()
{
	glPopMatrix();
}

const ofMatrix4x4* TransformNode::getWorldMatrix() const
{
	return world_matrix_ptr_;
}

const ofMatrix4x4* TransformNode::getLocalMatrix() const
{
	return &local_matrix_;
}

const ofMatrix4x4* TransformNode::getWorldMatrixInversed() const
{
	return world_matrix_inversed_ptr_;
}

const ofMatrix4x4* TransformNode::getLocalMatrixInversed() const
{
	return local_matrix_inversed_ptr_;
}

void TransformNode::setTranslation(const ofVec3f& trans)
{
	translation_.set(trans);
	dirty(LOCAL);
}

void TransformNode::setTranslation(float x, float y, float z)
{
	translation_.set(x,y,z);
	dirty(LOCAL);
}

void TransformNode::setTranslationX(float x)
{
	translation_.x = x;
	dirty(LOCAL);
}
void TransformNode::setTranslationY(float y)
{
	translation_.y = y;
	dirty(LOCAL);
}
void TransformNode::setTranslationZ(float z)
{
	translation_.z = z;
	dirty(LOCAL);
}

void TransformNode::setOrientation(const ofVec3f& from, const ofVec3f& to)
{
    orientation_.makeRotate(from,to);
	dirty(LOCAL);
}
void TransformNode::setOrientation(float angle, const ofVec3f& axis)
{
    orientation_.makeRotate(angle, axis);
	dirty(LOCAL);
}

void TransformNode::setOrientation(float angle, float x, float y, float z)
{
    orientation_.makeRotate(angle, x, y, z);
	dirty(LOCAL);
}

void TransformNode::setOrientation(const ofQuaternion& quat)
{
	orientation_.set(quat);
	dirty(LOCAL);
}

void TransformNode::setOrientation(float angle1, const ofVec3f& axis1,
								   float angle2, const ofVec3f& axis2,
								   float angle3, const ofVec3f& axis3)
{
	orientation_.makeRotate(angle1, axis1, angle2, axis2, angle3, axis3);
	dirty(LOCAL);
}

void TransformNode::setOrientation(const ofVec3f& orientation)
{
	switch(rotation_order_) {
		case ROTATION_ORDER_XYZ:
			setOrientation(	orientation.x, ofVec3f(1,0,0),
							orientation.y, ofVec3f(0,1,0),
							orientation.z, ofVec3f(0,0,1));
			break;
		case ROTATION_ORDER_YZX:
			setOrientation(	orientation.y, ofVec3f(0,1,0),
							orientation.z, ofVec3f(0,0,1),
							orientation.x, ofVec3f(1,0,0));
			break;
		case ROTATION_ORDER_ZXY:
			setOrientation(	orientation.z, ofVec3f(0,0,1),
							orientation.x, ofVec3f(1,0,0),
							orientation.y, ofVec3f(0,1,0));
			break;
		case ROTATION_ORDER_XZY:
			setOrientation(	orientation.x, ofVec3f(1,0,0),
							orientation.z, ofVec3f(0,0,1),
							orientation.y, ofVec3f(0,1,0));
			break;
		case ROTATION_ORDER_YXZ:
			setOrientation(	orientation.y, ofVec3f(0,1,0),
							orientation.x, ofVec3f(1,0,0),
							orientation.z, ofVec3f(0,0,1));
			break;
		case ROTATION_ORDER_ZYX:
			setOrientation(	orientation.z, ofVec3f(0,0,1),
							orientation.y, ofVec3f(0,1,0),
							orientation.x, ofVec3f(1,0,0));
			break;
	}
}

void TransformNode::setRotation(const ofVec3f& rotation)
{
	rotation_.set(rotation);
	dirty(LOCAL);
}

void TransformNode::setRotation(float x, float y, float z)
{
	rotation_.set(x,y,z);
	dirty(LOCAL);
}

void TransformNode::setRotationX(float x)
{
	rotation_.x = x;
	dirty(LOCAL);
}

void TransformNode::setRotationY(float y)
{
	rotation_.y = y;
	dirty(LOCAL);
}

void TransformNode::setRotationZ(float z)
{
	rotation_.z = z;
	dirty(LOCAL);
}

void TransformNode::setScale(const ofVec3f& scale)
{
	scale_.set(scale);
	dirty(LOCAL);
}

void TransformNode::setScale(float x, float y, float z)
{
	scale_.set(x,y,z);
	dirty(LOCAL);
}

void TransformNode::setScaleX(float x)
{
	scale_.x = x;
	dirty(LOCAL);
}
void TransformNode::setScaleY(float y)
{
	scale_.y = y;
	dirty(LOCAL);
}
void TransformNode::setScaleZ(float z)
{
	scale_.z = z;
	dirty(LOCAL);
}

void TransformNode::setAnchorPoint(const ofVec3f& anchor)
{
	anchor_point_.set(anchor);
	dirty(LOCAL);
}

void TransformNode::setAnchorPoint(float x, float y, float z)
{
	anchor_point_.set(x,y,z);
	dirty(LOCAL);
}

void TransformNode::setAnchorPointX(float x)
{
	anchor_point_.x = x;
	dirty(LOCAL);
}

void TransformNode::setAnchorPointY(float y)
{
	anchor_point_.y = y;
	dirty(LOCAL);
}

void TransformNode::setAnchorPointZ(float z)
{
	anchor_point_.z = z;
	dirty(LOCAL);
}

void TransformNode::addTranslation(const ofVec3f& trans)
{
	translation_ += trans;
	dirty(LOCAL);
}

void TransformNode::addTranslation(float x, float y, float z)
{
	translation_ += ofVec3f(x,y,z);
	dirty(LOCAL);
}

void TransformNode::addOrientation(const ofVec3f& from, const ofVec3f& to)
{
	ofQuaternion quat;
	quat.makeRotate(from,to);
    orientation_ *= quat;
	dirty(LOCAL);
}
void TransformNode::addOrientation(float angle, const ofVec3f& axis)
{
	ofQuaternion quat;
	quat.makeRotate(angle, axis);
    orientation_ *= quat;
	dirty(LOCAL);
}

void TransformNode::addOrientation(float angle, float x, float y, float z)
{
	ofQuaternion quat;
	quat.makeRotate(angle, x, y, z);
    orientation_ *= quat;
	dirty(LOCAL);
}

void TransformNode::addOrientation(const ofQuaternion& quat)
{
    orientation_ *= quat;
	dirty(LOCAL);
}

void TransformNode::addOrientation(float angle1, const ofVec3f& axis1,
								   float angle2, const ofVec3f& axis2,
								   float angle3, const ofVec3f& axis3)
{
	ofQuaternion quat;
	quat.makeRotate(angle1, axis1, angle2, axis2, angle3, axis3);
    orientation_ *= quat;
	dirty(LOCAL);
}

void TransformNode::addScale(const ofVec3f& scale)
{
	scale_ += scale;
	dirty(LOCAL);
}

void TransformNode::addScale(float x, float y, float z)
{
	scale_ += ofVec3f(x,y,z);
	dirty(LOCAL);
}
void TransformNode::mulScale(const ofVec3f& scale)
{
	scale_ *= scale;
	dirty(LOCAL);
}

void TransformNode::mulScale(float x, float y, float z)
{
	scale_ *= ofVec3f(x,y,z);
	dirty(LOCAL);
}

void TransformNode::addAnchorPoint(const ofVec3f& anchor)
{
	anchor_point_ += anchor;
	dirty(LOCAL);
}

void TransformNode::addAnchorPoint(float x, float y, float z)
{
	anchor_point_ += ofVec3f(x,y,z);
	dirty(LOCAL);
}

void TransformNode::addAnchorPointX(float x)
{
	anchor_point_.x += x;
	dirty(LOCAL);
}

void TransformNode::addAnchorPointY(float y)
{
	anchor_point_.y += y;
	dirty(LOCAL);
}

void TransformNode::addAnchorPointZ(float z)
{
	anchor_point_.z += z;
	dirty(LOCAL);
}



/* EOF */