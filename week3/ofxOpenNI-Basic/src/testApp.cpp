#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	openni.setup();
	image.setup(&openni);
	depth.setup(&openni);
	user.setup(&openni);
	openni.registerViewport();
	openni.setMirror(true);
}

void testApp::update(){
	openni.update();
	image.update();
	depth.update();
	user.update();
}

void testApp::draw(){
	ofBackground(0);
	ofSetColor(255);
	ofScale(.75, .75, .75);
	depth.draw(0, 0, 640, 480);
	image.draw(640, 0, 640, 480);
	user.draw();
}