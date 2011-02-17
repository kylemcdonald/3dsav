#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(60);
	
	input.setup(panel);  // < -- could pass in useKinnect here?
	
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
	panel.addToggle("auto rotate", "autoRotate", false);
	panel.addToggle("draw scene bounding box", "drawSceneBox", false);
	
	panel.addToggle("draw debug", "drawDebug", false);
	
}

//--------------------------------------------------------------
void testApp::update() {
	
	input.update();
	
	centroid.set(0,0,0);
	int nPixels = 0;
	
	int width = input.depthImage.getWidth();
	int height = input.depthImage.getHeight();
	
	vector<ofPoint>& pointCloud = input.pointCloud;
	bool needsSetting = true;
	for(int i = 0; i < pointCloud.size(); i++) {
		ofPoint cur = pointCloud[i];
		
		if(needsSetting) {
			maxBound = cur;
			minBound = cur;
			needsSetting = false;
		} else {
			maxBound = max(maxBound, cur);
			minBound = min(minBound, cur);
		}
		
		centroid += cur;
		nPixels++;
	}
	
	centroid /= nPixels;	
	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	ofBackground(0, 0, 0);
	
	// draw debug or non debug
	
	if (panel.getValueB("drawDebug")){
		input.drawDebug();
		
	} else {
		ofPushMatrix();
		
			// center everything
			ofTranslate(ofGetWidth()/2, ofGetWidth()/2, 0);
			ofSetColor(255, 255, 255);
			ofRotateY(panel.getValueF("rotateY"));
			if (panel.getValueB("autoRotate")){
				ofRotateY(ofGetElapsedTimef()*5);
			}
			
			ofScale(3,3,3); // zoom in so 1 cm = 3 pixels
			
			input.drawPerspective();
			
			// draw anything else: 
			
			ofBox(centroid.x, centroid.y, centroid.z, 10);
			
			ofBox(minBound, maxBound);
			
		ofPopMatrix();
		
	}
	
}

//--------------------------------------------------------------
void testApp::exit() {
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
}

