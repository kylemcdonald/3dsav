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
	openni.drawSkeletons(0, 0, 640, 480);
}

void testApp::exit() {
	openni.stop();
}