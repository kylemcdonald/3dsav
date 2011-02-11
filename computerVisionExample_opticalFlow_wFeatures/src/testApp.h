#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"

class testApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();
	void drawIplImage(IplImage *image, int x, int y, GLfloat xZoom, GLfloat yZoom);

	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	ofVideoGrabber 		vidGrabber;


	ofxCvColorImage		colorImg;

	ofxCvGrayscaleImage 	grayImage,prevGrayImage;

	IplImage *pyramid, *prev_pyramid, *swap_temp;
	char* status;

	static const int MAX_COUNT = 500;
	CvPoint2D32f* points[2];

	double quality;
	double min_distance;
	int count;
	int flags;
	int win_size;

	bool opticalFlow;
	
	bool selecting;
	CvPoint origin;
	CvRect selection;
};

#endif
