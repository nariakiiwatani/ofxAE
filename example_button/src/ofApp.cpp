#include "ofApp.h"
#include "ofxAEComposition.h"
#include "ButtonCap.h"

//--------------------------------------------------------------
void ofApp::setup(){
	comp_ = loader_.loadComposition("button.json");
	button_ = new ButtonCap(comp_->getAVLayer("hit"), comp_);
}

//--------------------------------------------------------------
void ofApp::update(){
	comp_->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	comp_->draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
	delete button_;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
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
