#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(30);
	ofEnableAlphaBlending();
	ofxAE::Loader loader;
	loader.loadComposition(composition_, "marker_loop.json");
	composition_.setLoopState(FrameCounter::LOOP_ONEWAY);
}

//--------------------------------------------------------------
void testApp::update(){
	composition_.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	composition_.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if('1' <= key && key <= '9') {
		composition_.setActiveMarker(key-'1');
	}
	else if(key == '0') {
		composition_.clearActiveMarker();
	}
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
