#include "testApp.h"

const int useKinect = false;

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(60);
	
	camWidth = 640;
	camHeight = 480;
	
	if(useKinect) {
		//kinect.init(true);  //shows infrared image
		kinect.init();
		kinect.setVerbose(true);
		kinect.open();
	} else {
		animation.load("janus-1283433262");
		cout << "Animation length is " << animation.size() << " frames." << endl;
	}
	
	depthImage.allocate(camWidth, camHeight);
}

//--------------------------------------------------------------
void testApp::update() {
	if(useKinect) {
		kinect.update();
		if(kinect.isFrameNew()) {
			kinect.calibration.setClippingInCentimeters(20, 200);
			depthImage.setFromPixels(kinect.getDepthPixels(), camWidth, camHeight);
			depthImage.flagImageChanged();
		}
	} else {
		int frame = ofGetFrameNum() % animation.size();
		ofImage& cur = animation.getAlpha(frame);
		depthImage.setFromPixels(cur.getPixels(), cur.getWidth(), cur.getHeight());
		depthImage.flagImageChanged();
	}
	
	// do processing here on depthImage
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(0, 0, 0);
	
	ofSetColor(255, 255, 255);
	depthImage.draw(0, 0);
	
	ofPushMatrix();
	ofTranslate(0, camHeight);
	ofTranslate(camWidth / 2, camHeight / 2);
	ofRotateY(ofMap(mouseX, 0, ofGetWidth(), -180, 180));
	ofTranslate(-camWidth / 2, -camHeight / 2);
	drawPointCloud();
	ofPopMatrix();
	
	ofSetColor(255, 0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}

//--------------------------------------------------------------
void testApp::drawPointCloud() {
	if(useKinect) {
		ofScale(1, 1, 6);
		ofTranslate(0, 0, -200);
	} else {
		ofScale(1, 1, 2.5);
		ofTranslate(0, 0, -128);
	}
	
	unsigned char* depthPixels = depthImage.getPixels();
	glBegin(GL_POINTS);
	int step = 2;
	for(int y = 0; y < camHeight; y += step) {
		for(int x = 0; x < camWidth; x += step) {
			int i = y * camWidth + x;
			// this is an orthographic projection,
			// which isn't really 'correct'
			glVertex3f(x, y, depthPixels[i]);
		}
	}
	glEnd();
}

//--------------------------------------------------------------
void testApp::exit() {
	if(useKinect) {
		kinect.close();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

