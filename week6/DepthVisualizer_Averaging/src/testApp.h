#pragma once

#include "ofMain.h"

#include "ofxAutoControlPanel.h"
#include "DepthVisualizerInput.h"

class testApp : public ofBaseApp {
	
	public:

		void setup();
		void update();
		void draw();
		void exit();
		
		void resetAverage();
	
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		ofxAutoControlPanel panel;
	
		DepthVisualizerInput input;
		ofxVec3f summedGrid[480][640];
		unsigned int summedCount[480][640];
		ofxVec3f averagedGrid[480][640];
};
