#pragma once

#include "ofMain.h"
#include "ofxDirList.h"

class scanDataPlayer {
public:
	void load(string animationDirectory);
	void unload();
	
	~scanDataPlayer();
	
	int size() const;
	
	void setPlaySpeed(float playSpeed);
	bool isFrameNew();
	int getFrameNumber();
	
	// use playback settings to get the current frame
	ofImage& get();
	ofImage& getAlpha();
		
	// get specific frames
	ofImage& get(int frameNumber);
	ofImage& getAlpha(int frameNumber);
	
protected:
	void checkLoaded(int frameNumber);

	vector<ofImage*> images;
	vector<ofImage*> alpha;
	
	ofxDirList dir;
	
	float framerate;
	int lastPosition;
	float frame;
	float lastTime;
	float playSpeed;
};