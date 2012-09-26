#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxOsc.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	ofVec3f ConvertProjectiveToRealWorld(const XnPoint3D& p);
	
	ofxOpenNIContext openni;
	ofxDepthGenerator depth;
	ofxImageGenerator image;
	ofxUserGenerator user;
	float distance;
	
	ofxOscSender osc;
};
