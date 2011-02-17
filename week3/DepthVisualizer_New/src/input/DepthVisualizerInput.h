/*
 *  DepthVisualizerInput.h
 *  DepthVisualizer
 *
 *  Created by zachary lieberman on 2/16/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxAutoControlPanel.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "scanDataPlayer.h"
#include "ofx3dGraphics.h"


class DepthVisualizerInput {

public: 
	
	int camWidth, camHeight;
	
	ofxKinect kinect;
	scanDataPlayer animation;
	ofxCvGrayscaleImage depthImage;
	
	void setup();
	void update();
	void drawDebug();
	
	void exit();

};