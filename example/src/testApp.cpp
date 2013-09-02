#include "testApp.h"
#include "ofxAEComposition.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(30);
	ofEnableAlphaBlending();
	composition_ = loader_.loadComposition("simple2d.json");
}

//--------------------------------------------------------------
void testApp::update(){
	static int frame = 0;
	
	composition_->update();
	composition_->setFrame(frame);
	if(++frame>=30) {
		frame = 0;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	composition_->draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
