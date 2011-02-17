#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"


class testApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	ofVideoGrabber		videoGrabber;

	ofxCvColorImage		colorImg;

	ofxCvGrayscaleImage 	grayImage;


	static const int MAX_COUNT = 500;
	CvPoint2D32f* points[2];
};

#endif
