#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	kinect.init(false, false);  // disable infrared/rgb video iamge (faster fps)
	kinect.open();
	
	panel.setup(280, 400);
	panel.addPanel("Settings");
	panel.addSlider("nearThreshold", 500, 0, 2000);
	panel.addSlider("farThreshold", 2000, 0, 2000);
	panel.addToggle("nearWhite", true);
	panel.addToggle("threshold", false);
	
	result.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
}

void testApp::update() {
	kinect.update();
	if(kinect.isFrameNew()) {
		float* distancePixels = kinect.getDistancePixels(); // distance in millimeters
		unsigned char* resultPixels = result.getPixels();
		int n = kinect.getWidth() * kinect.getHeight();
		float nearThreshold = panel.getValueF("nearThreshold");
		float farThreshold = panel.getValueF("farThreshold");
		int nearTarget, farTarget;
		bool threshold = panel.getValueB("threshold");
		int thresholdLevel = (nearTarget + farTarget) / 2;
		bool nearWhite = panel.getValueB("nearWhite");
		if(nearWhite) {
			nearTarget = 255;
			farTarget = 0;
		} else {
			nearTarget = 0;
			farTarget = 255;
		}
		if(nearThreshold != farThreshold) {
			for(int i = 0; i < n; i++) {
				float cur = distancePixels[i];
				if(cur != 0) { // only remap nonzeros
				
					// this code will threshold data to white or black
					if(threshold) {
						if(cur > thresholdLevel) {
							if(nearWhite) {
								cur = 0;
							} else {
								cur = 255;
							}
						} else {
							if(nearWhite) {
								cur = 255;
							} else {
								cur = 0;
							}
						}
					}
					
					// alternatively, just remap between 0 and 255
					else {
						cur = ofMap(cur, nearThreshold, farThreshold, nearTarget, farTarget, true);
					}
				}
				resultPixels[i] = cur;
			}
		}
		result.update();
	}
}

void testApp::draw() {
	ofSetColor(255);
	result.draw(0, 0);
}

void testApp::exit() {
	kinect.close();
}
