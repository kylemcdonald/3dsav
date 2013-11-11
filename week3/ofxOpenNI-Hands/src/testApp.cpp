#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	openni.setup();
	image.setup(&openni);
	depth.setup(&openni);
	hands.setup(&openni, 4);
	openni.registerViewport();
	openni.setMirror(true);
	img.set(0);
}

void testApp::update(){
	openni.update();
	image.update();
	depth.update();
}

void testApp::draw(){
	ofBackground(0);
	ofSetColor(255);
	ofScale(.75, .75, .75);
	depth.draw(0, 0, 640, 480);
	image.draw(640, 0, 640, 480);
	
	ofSetColor(255, 0, 0);
	int n = hands.getNumTrackedHands();
	for(int i = 0; i < n; i++) {
		ofxTrackedHand* hand = hands.getHand(i);
		ofPoint& handPosition = hand->projectPos;
		ofCircle(handPosition.x, handPosition.y, 10);
	}
}
