#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	
	ofxKinect kinect;
	ofEasyCam easyCam;
	ofMesh mesh; // for even more speed, use ofVboMesh
	ofLight light;
	bool drawWireframe;
};
