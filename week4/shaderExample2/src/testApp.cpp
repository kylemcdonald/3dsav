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
	
	shader.setUniform("x", 100.0f);
	shader.setUniform("y", 100.0f);
	shader.setUniform("w", 200.0f);
	shader.setUniform("h", 200.0f);

	shader.setUniform("color", (float)(mouseX / (float)ofGetWidth()));

	
	ofRect(100,100,200,200);
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

