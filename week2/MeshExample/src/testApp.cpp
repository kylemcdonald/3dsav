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

ofVec3f getNormal(ofVec3f& a, ofVec3f& b, ofVec3f& c) {
	ofVec3f side1 = a - b;
	ofVec3f side2 = c - b;
	ofVec3f normal = side1.cross(side2);
	normal.normalize();
	return normal;
}

void testApp::setup() {
	ofSetVerticalSync(true);
	kinect.init(false, false);  // disable infrared/rgb video iamge (faster fps)
	kinect.open();
	light.setPosition(0, 0, -200);
	light.setDiffuseColor(ofColor(255));
	light.setAmbientColor(ofColor(0));
	light.enable();
	drawWireframe = false;
}

void testApp::update() {
	kinect.update();
	if(kinect.isFrameNew()) { // only recompute the mesh if necessary
		int width = kinect.getWidth();
		int height = kinect.getHeight();
		float* distancePixels = kinect.getDistancePixels(); // distance in millimeters
		mesh.clear();
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);
		for(int y = 0; y < height - 1; y++) { // don't go to the end
			for(int x = 0; x < width - 1; x++) { // don't go to the end
			
				// get indices for each corner
				int nwi = (y + 0) * width + (x + 0);
				int nei = (y + 0) * width + (x + 1);
				int sei = (y + 1) * width + (x + 1);
				int swi = (y + 1) * width + (x + 0);
				
				// get z values for each corner
				float nwz = distancePixels[nwi];
				float nez = distancePixels[nei];
				float sez = distancePixels[sei];
				float swz = distancePixels[swi];
				
				if(nwz > 0 && nez > 0 && sez > 0 && swz > 0) { // ignore empty depth pixels
					// get real world locations for each corner
					ofVec3f nwv = ConvertProjectiveToRealWorld(x + 0, y + 0, nwz);
					ofVec3f nev = ConvertProjectiveToRealWorld(x + 1, y + 0, nez);
					ofVec3f sev = ConvertProjectiveToRealWorld(x + 1, y + 1, sez);
					ofVec3f swv = ConvertProjectiveToRealWorld(x + 0, y + 1, swz);
					
					// compute normal for the upper left
					ofVec3f normal = getNormal(nwv, nev, swv);
				
					// add the upper left triangle
					mesh.addNormal(normal);
					mesh.addVertex(nwv);
					mesh.addNormal(normal);
					mesh.addVertex(nev);
					mesh.addNormal(normal);
					mesh.addVertex(swv);
					
					// add the bottom right triangle
					mesh.addNormal(normal);
					mesh.addVertex(nev);
					mesh.addNormal(normal);
					mesh.addVertex(sev);
					mesh.addNormal(normal);
					mesh.addVertex(swv);
				}
			}
		}
	}
}

void testApp::draw() {
	ofBackground(0);
	
	easyCam.begin();
	ofScale(1, -1, -1); // orient the point cloud properly
	ofTranslate(0, 0, -1500); // rotate about z = 1500 mm
	glEnable(GL_DEPTH_TEST);
	ofSetColor(255);
	if(drawWireframe) {
		ofDisableLighting();
		mesh.drawWireframe();
	} else {
		ofEnableLighting();
		mesh.drawFaces();
	}
	easyCam.end();
	
	ofDisableLighting();
	ofDrawBitmapString(drawWireframe ? "rendering wireframe" : "rendering mesh", 10, 20);
}

void testApp::exit() {
	kinect.close();
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		drawWireframe = !drawWireframe;
	}
}