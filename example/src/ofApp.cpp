#include "ofApp.h"
#include "ofxAELoader.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofEnableAlphaBlending();
	ofxAE::Loader loader;
	loader.loadComposition(composition_, "3d_basic.json");
	composition_.setLoopState(FrameCounter::LOOP_ONEWAY);
}

//--------------------------------------------------------------
void ofApp::update(){
	composition_.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	composition_.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if('1' <= key && key <= '9') {
		composition_.setActiveMarker(key-'1');
	}
	else if(key == '0') {
		composition_.clearActiveMarker(true);
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
