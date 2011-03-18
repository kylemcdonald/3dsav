#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	panel.setup("Control Panel", 1024-310, 5, 300, 600);
	panel.addPanel("nvector");
	panel.addSlider("xLength", "xLength", 0, -100, 100);
	panel.addSlider("yLength", "yLength", 0, -100, 100);
	panel.addSlider("zLength", "zLength", 0, -100, 100);
	
}

//--------------------------------------------------------------
void testApp::update(){

}

void rotateToNormal(ofxVec3f normalVec) {
	normalVec.normalize();
	
	float rotationAmount;
	ofxVec3f rotationAngle;
	ofxQuaternion rotation;
	
	ofxVec3f xaxis(0, 0, 1);
	rotation.makeRotate(xaxis, normalVec);
	rotation.getRotate(rotationAmount, rotationAngle);
	ofRotate(ofRadToDeg(rotationAmount), rotationAngle.x, rotationAngle.y, rotationAngle.z);
}

//--------------------------------------------------------------
void testApp::draw(){
	ofxVec3f start(0, 0, 0);
	ofxVec3f end(
		panel.getValueF("xLength"),
		panel.getValueF("yLength"),
		panel.getValueF("zLength"));
		
	ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);
	
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	
	glBegin(GL_LINES);
	glVertex3f(start.x, start.y, start.z);
	glVertex3f(end.x, end.y, end.z);
	glEnd();
	
	rotateToNormal(end - start);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	
	glutSolidCone(40, 200, 20, 20);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

