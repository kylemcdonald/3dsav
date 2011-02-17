#include "testApp.h"

const int useKinect = true;

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

		// this is setting the raw data conversion range
		// we assume that 100 to 300 CM (1 - 3 meters) is good for tracking a person in space
		kinect.getCalibration().setClippingInCentimeters(100, 300);
		
	} else {
		animation.load("janus-1283433262");
		cout << "Animation length is " << animation.size() << " frames." << endl;
	}
	
	depthImage.allocate(camWidth, camHeight);
	
	panel.setup("Control Panel", 5, 5, 300, 600);
	panel.addPanel("Threshold and Scale");
	panel.addPanel("Control");
	
	panel.setWhichPanel("Threshold and Scale");
	panel.addSlider("near threshold", "nearThreshold", 255, 0, 255, true);
	panel.addSlider("far threshold", "farThreshold", 0, 0, 255, true);
	panel.addSlider("depth scale", "depthScale", 5, 1, 20);
	panel.addSlider("depth offset", "depthOffset", 128, 0, 255);
	panel.addSlider("step size", "stepSize", 2, 1, 4, true);
	panel.addSlider("point size", "pointSize", 1, 1, 10, true);
	panel.addToggle("draw zeros", "drawZeros", false);
	
	panel.setWhichPanel("Control");
	panel.addSlider("rotate y axis", "rotateY", 0, -360, 360, false);	
	panel.addToggle("draw debug", "drawDebug", false);
}

//--------------------------------------------------------------
void testApp::update() {
	if(useKinect) {
		kinect.update();
		if(kinect.isFrameNew()) {
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
	
	
	
	unsigned char* depthPixels = depthImage.getPixels();
	int nearThresh =  panel.getValueF("nearThreshold");
	int farThresh =  panel.getValueF("farThreshold");
	
	for(int y = 0; y < camHeight; y++) {
		for(int x = 0; x < camWidth; x++) {
			int i = y * camWidth + x;
			if (depthPixels[i] <  farThresh  || depthPixels[i] >= nearThresh){
				depthPixels[i] = 0;
			}
		}
	}
	
	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	
	ofBackground(0, 0, 0);
	
	// draw debug or non debug
	
	if (panel.getValueB("drawDebug")){
		ofPushMatrix();
		// center everything
		ofTranslate((ofGetWidth() - camWidth) / 2, 0, 0);
		ofSetColor(255, 255, 255);
		depthImage.draw(0, 0);
		ofPushMatrix();
		ofTranslate(0, camHeight);
		ofTranslate(camWidth / 2, camHeight / 2);
		ofRotateY(panel.getValueF("rotateY"));
		ofTranslate(-camWidth / 2, -camHeight / 2);
		drawPointCloud();
		ofPopMatrix();
		ofPopMatrix();
	} else {
		ofPushMatrix();
		// center everything
		ofTranslate(ofGetWidth()/2, ofGetWidth()/2, 0);
		ofSetColor(255, 255, 255);
		ofRotateY(panel.getValueF("rotateY"));
		ofTranslate(-camWidth / 2, -camHeight / 2);
		drawPointCloud();
		ofPopMatrix();
		
	}

}

//--------------------------------------------------------------
void testApp::drawPointCloud() {
	if(useKinect) {
		ofScale(1, 1, panel.getValueF("depthScale"));
		ofTranslate(0, 0, -panel.getValueF("depthOffset"));
	} else {
		ofScale(1, 1, 2.5);
		ofTranslate(0, 0, -128);
	}
	
	unsigned char* depthPixels = depthImage.getPixels();
	glEnable(GL_POINT_SMOOTH);
	glPointSize(panel.getValueF("pointSize"));
	glBegin(GL_POINTS);
	int step = panel.getValueI("stepSize");
	bool drawZeroes = panel.getValueB("drawZeros");
	for(int y = 0; y < camHeight; y += step) {
		for(int x = 0; x < camWidth; x += step) {
			int i = y * camWidth + x;
			// this is an orthographic projection,
			// which isn't really 'correct'
			if(!(depthPixels[i] == 0 && !drawZeroes)) {
				glVertex3f(x, y, depthPixels[i]);
			}
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

