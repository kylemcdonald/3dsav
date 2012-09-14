#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxAutoControlPanel.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	ofxKinect kinect;
	ofxAutoControlPanel panel;
	ofImage result;
};
