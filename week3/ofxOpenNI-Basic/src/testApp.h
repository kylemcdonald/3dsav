#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	ofxOpenNIContext openni;
	ofxDepthGenerator depth;
	ofxImageGenerator image;
	ofxUserGenerator user;
};
