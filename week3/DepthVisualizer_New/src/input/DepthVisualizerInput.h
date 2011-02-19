#pragma once

#include "ofMain.h"
#include "ofxAutoControlPanel.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "scanDataPlayer.h"
#include "ofx3dGraphics.h"

class DepthVisualizerInput {

public: 
	
	// you can get two things from this class
	
	// 1) grayscale depth image
	ofxCvGrayscaleImage depthImage;
	
	// 2) point cloud of projected points
	vector<ofPoint> pointCloud;
	
	int camWidth, camHeight;
	float rawFarThreshold, rawNearThreshold;
	
	ofxKinect kinect;
	scanDataPlayer animation;
	
	ofxControlPanel* panel;
	void setup(ofxControlPanel& panel);
	void update();
	void drawDebug();
	void drawOrthographic();
	void drawPerspective();
	
	bool usingKinect();
	void exit();

};