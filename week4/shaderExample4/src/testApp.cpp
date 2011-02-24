#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	
	shader.setup("gradient", "gradient"); // loads gradient.frag, gradient.vert

	shader.printActiveUniforms();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	
	shader.begin();
	shader.setUniform("time", ofGetElapsedTimef());
	
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	//for (int i = 0; i < 10; i++) {
//		ofRect(i * 50,50, 40,400);
//	}
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

