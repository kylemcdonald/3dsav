#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void handEvent(ofxOpenNIHandEvent & event);
	
	ofxOpenNI openni;
};
