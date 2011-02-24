#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	
	shader.setup("gradient", "gradient"); // loads gradient.frag, gradient.vert

	shader.printActiveUniforms();
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	
	shader.begin();
	
	ofSetColor(255,0,0);
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
	ofTranslate(0,0,sin(ofGetElapsedTimef())*300);
	ofCircle(sin(ofGetElapsedTimef()+2)*300, 0, 20);
	
	shader.end();
	
	
	
	
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

