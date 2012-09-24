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
}

void testApp::update() {
	kinect.update();
}

void testApp::draw() {
	ofBackground(0);
		
	easyCam.begin();
	int width = kinect.getWidth();
	int height = kinect.getHeight();
	ofScale(1, -1, -1); // orient the point cloud properly
	ofTranslate(0, 0, -1500); // rotate about z = 1500 mm

	float* distancePixels = kinect.getDistancePixels(); // distance in millimeters
	ofMesh cloud;
	cloud.setMode(OF_PRIMITIVE_POINTS);
	float minz = numeric_limits<float>::max();
	float minx = 0, miny = 0;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int i = y * width + x;
			float z = distancePixels[i];
			if(z != 0) { // ignore empty depth pixels
				ofVec3f cur = ConvertProjectiveToRealWorld(x, y, z);
				cloud.addVertex(cur);
				if(z < minz) {
					minz = z;
					minx = x;
					miny = y;
				}
			}
		}
	}
	cloud.draw();
	easyCam.end();
	
	ofSetColor(255);
	kinect.drawDepth(0, 0);
	ofSetColor(255, 0, 0);
	ofNoFill();
	int searchRadius = 128;
	ofRect(minx - searchRadius / 2, miny - searchRadius / 2, searchRadius, searchRadius);
	ofCircle(minx, miny, 10);
	
	ofVec3f sum;
	int count = 0;
	int maxSearchDistance = 100;
	for(int y = -searchRadius / 2; y < +searchRadius / 2; y++) {
		for(int x = -searchRadius / 2; x < +searchRadius / 2; x++) {
			int curx = x + minx;
			int cury = y + miny;
			if(curx > 0 && curx < width &&
				cury > 0 && cury < height) {
				int i = cury * width + curx;
				float curz = distancePixels[i];
				if(curz != 0 && abs(curz - minz) < maxSearchDistance) {
					sum.x += curx;
					sum.y += cury;
					sum.z += curz;
					count++;
				}
			}
		}
	}
	ofVec3f avg = sum / count;
	ofSetColor(0, 0, 255);
	ofCircle(avg.x, avg.y, 10);
	
	if(smoothedForepoint == ofVec3f()) {
		smoothedForepoint = avg;
	} else {
		smoothedForepoint.interpolate(avg, .1);
	}
	ofSetColor(255, 0, 255);
	ofCircle(smoothedForepoint.x, smoothedForepoint.y, 10);
	
	ofSetColor(255);
	ofDrawBitmapString(ofToString(minz) + "->" + ofToString(avg.z), 10, 20);
}

void testApp::exit() {
	kinect.close();
}