#pragma once

#include "ofMain.h"
#include "ofxAELoader.h"
#include "ofxAEImageCap.h"

class ButtonCap;

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
private:
	ofxAE::Loader loader_;
	ofxAE::Composition *comp_;
	ButtonCap *button_;
	ofxAE::AVLayer *image_layer_;
	ofxAE::ImageCap *image_a_, *image_b_;
};
