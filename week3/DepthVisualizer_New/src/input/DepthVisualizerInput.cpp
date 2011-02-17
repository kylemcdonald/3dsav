/*
 *  DepthVisualizerInput.cpp
 *  DepthVisualizer
 *
 *  Created by zachary lieberman on 2/16/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "DepthVisualizerInput.h"
#include "testApp.h"



const int useKinect = true;


void DepthVisualizerInput::setup(){
	
	camWidth = 640;
	camHeight = 480;
	
	if(useKinect) {
		//kinect.init(true);  //shows infrared image
		kinect.init();
		kinect.setVerbose(true);
		kinect.open();
		
		// this is setting the raw data conversion range
		// we assume that 100 to 300 CM (1 - 3 meters) is good for tracking a person in space
		kinect.getCalibration().setClippingInCentimeters(100, 300);
		
	} else {
		animation.load("janus-1283433262");
		cout << "Animation length is " << animation.size() << " frames." << endl;
	}
	
	depthImage.allocate(camWidth, camHeight);

}

void DepthVisualizerInput::update(){
	if(useKinect) {
		kinect.update();
		if(kinect.isFrameNew()) {
			depthImage.setFromPixels(kinect.getDepthPixels(), camWidth, camHeight);
			depthImage.flagImageChanged();
		}
	} else {
		int frame = ofGetFrameNum() % animation.size();
		ofImage& cur = animation.getAlpha(frame);
		depthImage.setFromPixels(cur.getPixels(), cur.getWidth(), cur.getHeight());
		depthImage.flagImageChanged();
	}
	
	// do processing here on depthImage
	// getting rid of (setting depth = 0) on pixels we don't care about
	// for example, background, etc. 
	//
	unsigned char* depthPixels = depthImage.getPixels();
	int nearThresh =  ((testApp * ) ofGetAppPtr())->panel.getValueF("nearThreshold");
	int farThresh =  ((testApp * ) ofGetAppPtr())->panel.getValueF("farThreshold");
	
	for(int y = 0; y < camHeight; y++) {
		for(int x = 0; x < camWidth; x++) {
			int i = y * camWidth + x;
			if (depthPixels[i] <  farThresh  || depthPixels[i] >= nearThresh){
				depthPixels[i] = 0;
			}
		}
	}
}


void DepthVisualizerInput::drawDebug(){
	ofPushMatrix();
	// center everything
	ofTranslate((ofGetWidth() - camWidth) / 2, 0, 0);
	ofSetColor(255, 255, 255);
	depthImage.draw(0, 0);
	ofPushMatrix();
	ofTranslate(0, camHeight);
	ofTranslate(camWidth / 2, camHeight / 2);
	ofRotateY(((testApp * ) ofGetAppPtr())->panel.getValueF("rotateY"));
	if (((testApp * ) ofGetAppPtr())->panel.getValueB("autoRotate")){
		ofRotateY(ofGetElapsedTimef()*5);
	}
	ofTranslate(-camWidth / 2, -camHeight / 2);
	
	// need something here....
	//drawPointCloud();
	
	ofPopMatrix();
	ofPopMatrix();

}


//--------------------------------------------------------------
void DepthVisualizerInput::exit() {
	if(useKinect) {
		kinect.close();
	}
}