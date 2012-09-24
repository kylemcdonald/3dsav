#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	openni.setup();
	openni.addImageGenerator();
	openni.addDepthGenerator();
	openni.addHandsGenerator();
	openni.addAllHandFocusGestures();
	openni.setMaxNumHands(4);
	openni.setRegister(true);
	openni.setMirror(true);
	openni.start();
}

void testApp::update(){
	openni.update();
}

void testApp::draw(){
	ofBackground(0);
	ofSetColor(255);
	ofScale(.75, .75, .75);
	openni.drawDepth(0, 0, 640, 480);
	openni.drawImage(640, 0, 640, 480);
	
	ofSetColor(255, 0, 0);
	int n = openni.getNumTrackedHands();
	for(int i = 0; i < n; i++) {
		ofxOpenNIHand& hand = openni.getTrackedHand(i);
		ofPoint& handPosition = hand.getPosition();
		ofCircle(handPosition.x, handPosition.y, 10);
	}
}

void testApp::exit() {
	openni.stop();
}

void testApp::handEvent(ofxOpenNIHandEvent& event) {
	ofLog() << 
		getHandStatusAsString(event.handStatus) << 
		"for hand" << event.id << 
		"from device" << event.deviceID;
}