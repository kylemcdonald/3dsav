#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	int w = 640 , h = 480;

	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(w, h);

	colorImg.allocate(w, h);
	grayImage.allocate(w, h);
	prevGrayImage.allocate(w, h);
	
	flow.setup(w, h);

}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);

	bool bNewFrame = false;


	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();


	if (bNewFrame){
		colorImg.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight());
		grayImage = colorImg;
		grayImage.mirror(false, true);
		flow.update(grayImage);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	grayImage.draw(0,0, ofGetWidth(), ofGetHeight());
	flow.draw(ofGetWidth(), ofGetHeight(), 3);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
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

