#pragma once

#include "ofMain.h"

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
	
private:
	vector<ofImage*> images;
	vector<ofImage*> alpha;
	
	float framerate;
	int lastPosition;
	float frame;
	float lastTime;
	float playSpeed;
};