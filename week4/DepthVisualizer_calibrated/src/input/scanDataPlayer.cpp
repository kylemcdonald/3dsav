#include "scanDataPlayer.h"

#include "ofxOpenCv.h"

void scanDataPlayer::load(string scanDirectory) {
	setPlaySpeed(1);
	lastPosition = 0;
	framerate = 30;
	frame = 0;
	lastTime = ofGetElapsedTimef();
	
	int n = dir.listDir(scanDirectory);
	alpha.resize(n);
	images.resize(n);
	for(int i = 0; i < n; i++) {
		images[i] = new ofImage();
		images[i]->setUseTexture(false);
		alpha[i] = new ofImage();
		alpha[i]->setUseTexture(false);
	}
}

void scanDataPlayer::checkLoaded(int frameNumber) {
		ofImage* cur = images[frameNumber];
		if(cur->getWidth() == 0) { // if not already loaded
			cur->loadImage(dir.getPath(frameNumber));
			
			ofImage* curAlpha = alpha[frameNumber];
			curAlpha->allocate(cur->getWidth(), cur->getHeight(), OF_IMAGE_GRAYSCALE);
			
			// use opencv to extract the alpha channel from cur and put it in curAlpha
			int rows = cur->getHeight();
			int cols = cur->getWidth();
			CvMat curMat = cvMat(rows, cols, CV_8UC4, cur->getPixels());
			CvMat curAlphaMat = cvMat(rows, cols, CV_8UC1, curAlpha->getPixels());
			cvSplit(&curMat, NULL, NULL, NULL, &curAlphaMat);
		}
}

void scanDataPlayer::setPlaySpeed(float playSpeed) {
	this->playSpeed = playSpeed;
}

bool scanDataPlayer::isFrameNew() {
	float curTime = ofGetElapsedTimef();
	float timeDiff = curTime - lastTime;
	frame += timeDiff * playSpeed * framerate;
	lastTime = curTime;
	
	// this can be done without a while loop ;)
	while(frame < 0) {
		frame += size();
	}
	frame = fmodf(frame, size());
	int curPosition = (int) frame;
	bool newFrame = (curPosition != lastPosition);
	lastPosition = curPosition;
	
	return newFrame;
}

int scanDataPlayer::getFrameNumber() {
	return lastPosition;
}

ofImage& scanDataPlayer::get() {
	return get(lastPosition);
}

ofImage& scanDataPlayer::getAlpha() {
	return getAlpha(lastPosition);
}

void scanDataPlayer::unload() {
	for(int i = 0; i < size(); i++) {
		delete images[i];
		delete alpha[i];
	}
	images.clear();
}

scanDataPlayer::~scanDataPlayer() {
	unload();
}

int scanDataPlayer::size() const {
	return images.size();
}

ofImage& scanDataPlayer::get(int frameNumber) {
	checkLoaded(frameNumber);
	return *(images[frameNumber]);
}

ofImage& scanDataPlayer::getAlpha(int frameNumber) {
	checkLoaded(frameNumber);
	return *(alpha[frameNumber]);
}