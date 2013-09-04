# ofxAE
by nariakiiwatani

## Abstract
This is a project to make an addon for porting Adobe AfterEffects' composition to openFrameworks realtime animation.  
Currently so buggy, So I need community help. :)

## Depends on...
- [ofxMask](https://github.com/nariakiiwatani/ofxMask "ofxMask")
- [ofxJSON](https://github.com/jefftimesten/ofxJSON "ofxJSON")

## Usage...
### Export composition data from AfterEffects
1. Select composition(s) in project pane.
2. execute tool/compExport.jsx (File-->Script-->Execute) and save .json file in OF data folder.

### Load and play on OF
testApp.h  

	ofxAE::Loader loader_;
	ofxAE::Composition composition_;
	int frame_;
testApp.cpp  

	void testApp::setup(){
		ofSetFrameRate(30);
		ofEnableAlphaBlending();
		loader_.loadComposition(composition_, "foo.json");
	}

	//--------------------------------------------------------------
	void testApp::update(){
		composition_.setFrame(frame);
		composition_.update();
		// loop 30 frames
		if(++frame>=30) {
			frame = 0;
		}
	}

	//--------------------------------------------------------------
	void testApp::draw(){
		ofBackground(0);
		composition_.draw();
	}

## The goal is...
1. Drive all features in AE on OF.  
But of course it's impossible because AE is not for realtime animations.  
So I want the exporter to be able to select enable/disable with some features.  

2. Provide interfaces to operate animations interactively.  
This is why I decide to make this addon.  
change image sources, motion loop, add realtime effects,,,  

3. Write exporters for various animation softwares.  
Though this addon is named AE, AE is not only way to make animations.  
Other softwares will also work if it can export a json file in same format.  
So it's important to keep independence between AE and OF.  
