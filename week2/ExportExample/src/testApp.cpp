#include "testApp.h"


void testApp::setup() {
	kinect.init();
	kinect.setRegistration(true);
	kinect.setDepthClipping(500, 1500);
	kinect.open();
}

void testApp::update() {
	kinect.update();
	if(kinect.isFrameNew()) {
		if(ofGetKeyPressed(' ')) {
			ofPixels& depthPixels = kinect.getDepthPixelsRef();
			ofPixels& colorPixels = kinect.getPixelsRef();
			ofSaveImage(depthPixels, "depth.png");
			ofSaveImage(colorPixels, "color.png");
			img.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
			for(int y = 0; y < 480; y++) {
				for(int x = 0; x < 640; x++) {
					ofColor color = colorPixels.getColor(x, y);
					ofColor depth = depthPixels.getColor(x, y);
					ofColor combinedColor(color, depth.getBrightness());
					img.setColor(x, y, combinedColor);
				}
			}
			img.saveImage("out.png");
		}
	}
}

void testApp::draw() {
	ofBackground(0);
	ofSetColor(255);
	kinect.drawDepth(0, 0, 640, 480);
	kinect.draw(0, 480, 640, 480);
}

void testApp::exit() {
	kinect.close();
}

