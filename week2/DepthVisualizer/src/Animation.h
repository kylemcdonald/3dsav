#pragma once

#include "ofMain.h"

class Animation {
public:
	void load(string animationDirectory);
	void unload();
	
	~Animation();
	
	int size() const;
	ofImage& get(int frameNumber);
	ofImage& getAlpha(int frameNumber);
	
private:
	vector<ofImage*> images;
	vector<ofImage*> alpha;
};