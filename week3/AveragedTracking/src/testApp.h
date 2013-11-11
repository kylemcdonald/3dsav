#pragma once

#include "ofMain.h"
#include "ofxKinect.h"

#include "DelayTimer.h"
class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	ofVec3f smoothedForepoint;
	ofxKinect kinect;
	ofEasyCam easyCam;
	DelayTimer timer;
};
