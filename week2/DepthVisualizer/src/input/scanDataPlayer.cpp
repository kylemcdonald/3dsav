#include "scanDataPlayer.h"

#include "ofxDirList.h"
#include "ofxOpenCv.h"

void scanDataPlayer::load(string scanDirectory) {
	ofxDirList dir;
	int n = dir.listDir(scanDirectory);
	for(int i = 0; i < n; i++) {
		ofImage* cur = new ofImage();
		
		cur->loadImage(dir.getPath(i));
		images.push_back(cur);
		
		ofImage* curAlpha = new ofImage();
		curAlpha->allocate(cur->getWidth(), cur->getHeight(), OF_IMAGE_GRAYSCALE);
		
		// use opencv to extract the alpha channel from cur and put it in curAlpha
		int rows = cur->getHeight();
		int cols = cur->getWidth();
		CvMat curMat = cvMat(rows, cols, CV_8UC4, cur->getPixels());
		CvMat curAlphaMat = cvMat(rows, cols, CV_8UC1, curAlpha->getPixels());
		cvSplit(&curMat, NULL, NULL, NULL, &curAlphaMat);
		
		curAlpha->update();		
		alpha.push_back(curAlpha);
	}
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
	return *(images[frameNumber]);
}

ofImage& scanDataPlayer::getAlpha(int frameNumber) {
	return *(alpha[frameNumber]);
}