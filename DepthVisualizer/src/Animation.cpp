#include "Animation.h"

#include "ofxDirList.h"
#include "ofxOpenCv.h"

void Animation::load(string animationDirectory) {
	ofxDirList dir;
	int n = dir.listDir(animationDirectory);
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

void Animation::unload() {
	for(int i = 0; i < size(); i++) {
		delete images[i];
		delete alpha[i];
	}
	images.clear();
}

Animation::~Animation() {
	unload();
}

int Animation::size() const {
	return images.size();
}

ofImage& Animation::get(int frameNumber) {
	return *(images[frameNumber]);
}

ofImage& Animation::getAlpha(int frameNumber) {
	return *(alpha[frameNumber]);
}