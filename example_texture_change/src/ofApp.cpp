#include "ofApp.h"
#include "ofxAEComposition.h"
#include "ButtonCap.h"
#include "ofxAEAVLayer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	comp_ = loader_.loadComposition("button.json");
	button_ = new ButtonCap(comp_->getAVLayer("hit"), comp_);
	image_layer_ = comp_->getAVLayer("button");
	image_a_ = static_cast<ofxAE::ImageCap*>(image_layer_->getCap(0));
	image_layer_->removeCap(image_a_);
	image_b_ = new ofxAE::ImageCap(image_layer_);
	image_b_->loadImage("button_b.png");
}

//--------------------------------------------------------------
void ofApp::update(){
	comp_->update();
	if(button_->isClicked()) {
		image_layer_->removeCap(image_layer_->getCap(0));
		if(ofRandom(1) < 0.5f) {
			image_layer_->addCap(image_a_);
		}
		else {
			image_layer_->addCap(image_b_);
		}
	}
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
