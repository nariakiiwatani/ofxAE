#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(30);
	ofEnableAlphaBlending();
	ofxAE::Loader loader;
	loader.loadComposition(composition_, "nest_gap_time.json");
}

//--------------------------------------------------------------
void testApp::update(){
	static int frame = 0;
	
	composition_.setFrame(frame);
	composition_.update();
	if(++frame>=30*10) {
		frame = 0;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	composition_.draw();
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
