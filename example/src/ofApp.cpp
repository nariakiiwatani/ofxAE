#include "ofApp.h"
#include "ofxAEComposition.h"
#include "ofxAEAVLayer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofEnableAlphaBlending();
	composition_ = loader_.loadComposition("3d_nest.json");
	composition_->setLoopState(FrameCounter::LOOP_ONEWAY);
}

//--------------------------------------------------------------
void ofApp::update(){
	composition_->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	composition_->draw();
	ofxAE::AVLayer *layer = composition_->getAVLayer(0);
	float x = ofGetMouseX();
	float y = ofGetMouseY();
	if(layer && layer->isHit(x,y)) {
		ofPushStyle();
		ofSetColor(ofColor::red);
		ofFill();
		ofDrawEllipse(x,y,10,10);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if('1' <= key && key <= '9') {
		composition_->setActiveMarker(key-'1');
	}
	else if(key == '0') {
		composition_->clearActiveMarker(true);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
