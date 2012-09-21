#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	ofHideCursor();
}

void ofApp::update() {
}

void ofApp::draw() {
	ofBackground(0);
	ofSetColor(255);
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofRotateY(ofGetFrameNum());
	ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2);
	if(ofGetKeyPressed()) {
		mesh.drawWireframe();
	} else {
		mesh.draw();
	}
}

void ofApp::keyPressed(int key) {
	if(key == 'f') {
		ofToggleFullscreen();
	}
}

void ofApp::mouseDragged(int x, int y, int button) {
	ofVec2f start(lastX, lastY);
	ofVec2f end(x, y);
	ofVec2f diff = end - start;
	ofVec3f perp = diff.rotate(90).normalize();
	perp *= 100;
	ofColor curColor = ofColor::fromHsb(ofRandom(255), 255, 255);
	
	perp.rotate(ofGetElapsedTimef() * 90, diff);
	ofVec3f top = ofVec3f(end) - perp/2;
	ofVec3f bottom = ofVec3f(end) + perp/2;
	
	mesh.addColor(curColor);
	mesh.addVertex(top);
	mesh.addColor(curColor);
	mesh.addVertex(bottom);
	lastX = x;
	lastY = y;
}
