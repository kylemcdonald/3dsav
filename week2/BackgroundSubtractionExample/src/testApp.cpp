#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	kinect.init(false, false);  // disable infrared/rgb video iamge (faster fps)
	kinect.open();
	
	panel.setup(280, 200);
	panel.addPanel("Settings");
	panel.addToggle("learnBackground");
	panel.addToggle("clearBackground");
	panel.addSlider("threshold", 4, 0, 64, true); // true means it's an int slider, not float
	
	result.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	background.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
}

void testApp::update() {
	kinect.update();
	if(kinect.isFrameNew()) {
		unsigned char* kinectPixels = kinect.getDepthPixels();
		unsigned char* resultPixels = result.getPixels();
		unsigned char* backgroundPixels = background.getPixels();
		int n = kinect.getWidth() * kinect.getHeight();
		
		if(panel.getValueB("learnBackground")) {
			// learn the background image
			for(int i = 0; i < n; i++) {
				// only learn pixels with data
				if(kinectPixels[i] != 0) {
					// if the background has no data here, save the new data
					if(backgroundPixels[i] == 0) {
						backgroundPixels[i] = kinectPixels[i];
					}
					// if the background already has data, save the average
					else {
						backgroundPixels[i] = (backgroundPixels[i] + kinectPixels[i]) / 2;
					}
				}
			}
			background.update();
		}
		
		if(panel.getValueB("clearBackground")) {
			for(int i = 0; i < n; i++) {
				backgroundPixels[i] = 0;
			}
			background.update();
			panel.setValueB("clearBackground", false);
		}
		
		int threshold = panel.getValueI("threshold");
		
		for(int i = 0; i < n; i++) {
			// we're going to do signed math, so we store these pixels as signed values
			int backgroundPixel = backgroundPixels[i];
			int kinectPixel = kinectPixels[i];
			
			// this is the standard difference, not the absolute difference
			// because we only want things that are in front of the background
			// if we find something near or behind the background, we just set it to 0
			// advanced note: the threshold should get bigger as we get farther away,
			// because there is more noise when you're further away.
			if(kinectPixel - backgroundPixel > threshold) {
				resultPixels[i] = kinectPixel;
			} else {
				resultPixels[i] = 0;
			}
		}
		
		result.update();
	}
}

void testApp::draw() {
	ofSetColor(255);
	kinect.drawDepth(400, 0, 400, 300);
	background.draw(0, 300, 400, 300);
	result.draw(400, 300, 400, 300);
}

void testApp::exit() {
	kinect.close();
}
