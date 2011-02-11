#pragma once

#include "ofMain.h"

#include "ofxControlPanel.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "Animation.h"

class testApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		void exit();
	
		void drawPointCloud();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		int camWidth, camHeight;

		ofxKinect kinect;
		Animation animation;
		
		ofxCvGrayscaleImage depthImage;
};
