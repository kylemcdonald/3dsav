#pragma once

#include "ofMain.h"

class scanDataPlayer {
public:
	void load(string animationDirectory);
	void unload();
	
	~scanDataPlayer();
	
	int size() const;
	ofImage& get(int frameNumber);
	ofImage& getAlpha(int frameNumber);
	
private:
	vector<ofImage*> images;
	vector<ofImage*> alpha;
};