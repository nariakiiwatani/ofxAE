#include "ofxAELayer.h"
#include "ofGraphics.h"
#include "ofxAEMask.h"

namespace ofxAE {
Layer::~Layer()
{
	for(vector<Marker*>::iterator it = marker_.begin(); it != marker_.end(); ++it) {
		delete *it;
	}
	for(vector<PropertyBase_*>::iterator it = property_.begin(); it != property_.end(); ++it) {
		delete *it;
	}
}

void Layer::update()
{
	if(isDirty()) {
		refreshMatrix();
	}
	prepare();
}

void Layer::setOrientation(const ofVec3f& orientation)
{
	switch(getRotationOrder()) {
		case ROTATION_ORDER_XYZ:
			TransformNode::setOrientation(orientation.x,ofVec3f(1,0,0),
										  orientation.y,ofVec3f(0,1,0),
										  orientation.z,ofVec3f(0,0,1));
			break;
		case ROTATION_ORDER_YZX:
			TransformNode::setOrientation(orientation.y,ofVec3f(0,1,0),
										  orientation.z,ofVec3f(0,0,1),
										  orientation.x,ofVec3f(1,0,0));
			break;
		case ROTATION_ORDER_ZXY:
			TransformNode::setOrientation(orientation.z,ofVec3f(0,0,1),
										  orientation.x,ofVec3f(1,0,0),
										  orientation.y,ofVec3f(0,1,0));
			break;
		case ROTATION_ORDER_XZY:
			TransformNode::setOrientation(orientation.x,ofVec3f(1,0,0),
										  orientation.z,ofVec3f(0,0,1),
										  orientation.y,ofVec3f(0,1,0));
			break;
		case ROTATION_ORDER_YXZ:
			TransformNode::setOrientation(orientation.y,ofVec3f(0,1,0),
										  orientation.x,ofVec3f(1,0,0),
										  orientation.z,ofVec3f(0,0,1));
			break;
		case ROTATION_ORDER_ZYX:
			TransformNode::setOrientation(orientation.z,ofVec3f(0,0,1),
										  orientation.y,ofVec3f(0,1,0),
										  orientation.x,ofVec3f(1,0,0));
			break;
	}
}

void Layer::resetPropertyFrame()
{
	for(vector<PropertyBase_*>::iterator prop = property_.begin(); prop != property_.end(); ++prop) {
		(*prop)->resetFrame();
	}
}
void Layer::setPropertyFrame(int frame)
{
	for(vector<PropertyBase_*>::iterator prop = property_.begin(); prop != property_.end(); ++prop) {
		(*prop)->setFrame(frame);
	}
}

}
/* EOF */