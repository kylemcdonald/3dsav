#include "testApp.h"

const float FovX = 1.0144686707507438;
const float FovY = 0.78980943449644714;
const float XtoZ = tanf(FovX / 2) * 2;
const float YtoZ = tanf(FovY / 2) * 2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;

ofVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
	return ofVec3f((x / Xres - .5f) * z * XtoZ,
								 (y / Yres - .5f) * z * YtoZ,
								 z);
}

void testApp::setup() {
	ofSetVerticalSync(true);
	kinect.init(false, false);  // disable infrared/rgb video iamge (faster fps)
	kinect.open();
	useProjective = true;
}

void testApp::update() {
	kinect.update();
}

void testApp::draw() {
	ofBackground(0);
	
	ofSetColor(255);
	kinect.drawDepth(0, 0, 400, 300);
	
	easyCam.begin();
	int width = kinect.getWidth();
	int height = kinect.getHeight();
	ofScale(1, -1, -1); // orient the point cloud properly
	if(useProjective) {
		ofTranslate(-width / 2, -height / 2); // center the projective point cloud
	} else {
		ofTranslate(0, 0, -150); // rotate about z = 150 cm
	}
	float* distancePixels = kinect.getDistancePixels(); // distance in centimeters
	ofMesh cloud;
	cloud.setMode(OF_POINTS_MODE);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int i = y * width + x;
			float z = distancePixels[i];
			if(z != 0) { // ignore empty depth pixels
				if(useProjective) {
					cloud.addVertex(ofVec3f(x, y, z));
				} else {
					cloud.addVertex(ConvertProjectiveToRealWorld(x, y, z));
				}
			}
		}
	}
	cloud.drawVertices();
	easyCam.end();
	
	ofSetColor(255);
	ofDrawBitmapString(useProjective ? "projective point cloud" : "real world point cloud", 10, 20);
}

void testApp::exit() {
	kinect.close();
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		useProjective = !useProjective;
	}
}