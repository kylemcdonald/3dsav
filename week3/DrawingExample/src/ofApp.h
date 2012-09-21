#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
		
	int lastX, lastY;
	ofMesh mesh;
};