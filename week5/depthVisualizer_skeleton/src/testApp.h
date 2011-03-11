#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxAutoControlPanel.h"

class testApp : public ofBaseApp {
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	void	setupRecording(string _filename = "");
	void	setupPlayback(string _filename);
	string	generateFileName();
	
	bool isLive, isRecording;
	
	string currentFileName;
	

	ofxOpenNIContext	recordContext, playContext;
	ofxDepthGenerator	recordDepth, playDepth;
	ofxImageGenerator	recordImage, playImage;
	ofxUserGenerator	recordUser, playUser;
	ofxOpenNIRecorder	oniRecorder;		

	ofxAutoControlPanel panel;			
};
