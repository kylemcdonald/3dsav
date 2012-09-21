#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	openni.setup();
	openni.addImageGenerator();
	openni.addDepthGenerator();
	openni.addUserGenerator();
	openni.setRegister(true);
	openni.setMirror(true);
	openni.start();
	
	osc.setup("localhost", 8000);
	
	distance = 0;
}

void testApp::update(){
	openni.update();
	if(openni.getNumTrackedUsers() > 0) {
		ofxOpenNIUser& user = openni.getTrackedUser(0);
		ofPoint& leftHand = user.getLimb(LIMB_LEFT_LOWER_ARM).getEndJoint().getWorldPosition();
		ofPoint& rightHand = user.getLimb(LIMB_RIGHT_LOWER_ARM).getEndJoint().getWorldPosition();
		
		distance = (leftHand - rightHand).length();
		
		ofxOscMessage msg;
		msg.setAddress("/distance");
		msg.addFloatArg(distance);
		osc.sendMessage(msg);
	}
}

void testApp::draw(){
	ofBackground(0);
	ofSetColor(255);
	ofScale(.75, .75, .75);
	openni.drawDepth(0, 0, 640, 480);
	openni.drawImage(640, 0, 640, 480);
	openni.drawSkeletons(0, 0, 640, 480);
	ofRect(0, 10, distance, 10);
}

void testApp::exit() {
	openni.stop();
}