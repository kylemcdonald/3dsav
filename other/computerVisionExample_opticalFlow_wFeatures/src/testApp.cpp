#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	int w = 640 , h = 480;

	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(w, h);

	colorImg.allocate(w, h);
	grayImage.allocate(w, h);
	prevGrayImage.allocate(w, h);

	points[0] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
	points[1] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));

	pyramid = cvCreateImage( cvGetSize(grayImage.getCvImage()), 8, 1 );
	prev_pyramid = cvCreateImage( cvGetSize(grayImage.getCvImage()), 8, 1 );

	status = (char*)cvAlloc(MAX_COUNT);

	quality = 0.01;
	min_distance = 10;
	count = 350;
	flags = 0;
	win_size = 10;

	opticalFlow = false;
	selecting   = false;
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);

	bool bNewFrame = false;


	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();


	if (bNewFrame){

		colorImg.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight());


		prevGrayImage = grayImage;
		grayImage = colorImg;
		grayImage.mirror(false, true);
		
		if(opticalFlow)
		{
			cvCalcOpticalFlowPyrLK( prevGrayImage.getCvImage(), grayImage.getCvImage(), prev_pyramid, pyramid, points[1], points[0], count, cvSize(win_size, win_size), 3, status, 0, cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), flags );
			flags |= CV_LKFLOW_PYR_A_READY;
			
			CvPoint2D32f *swap_points;
			CV_SWAP( points[0], points[1], swap_points );
			CV_SWAP( prev_pyramid, pyramid, swap_temp );
			
		}
		
		//save the last grayImage
		prevGrayImage = grayImage;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);

	grayImage.draw(0,0, ofGetWidth(), ofGetHeight());

	ofSetLineWidth(3);
	ofSetColor(0xff00ff);
	for(int i=0; i<count; i++){
		float x = points[0][i].x / 640.0 * ofGetWidth();
		float y = points[0][i].y / 480.0 * ofGetHeight();
		ofLine(x-5, y, x+5, y);
		ofLine(x, y-5, x, y+5);
	}

	ofSetLineWidth(1);
	ofSetColor(0xffff00);
	for(int i=0; i<count; i++){
		float x = points[1][i].x / 640.0 * ofGetWidth();
		float y = points[1][i].y / 480.0 * ofGetHeight();
		ofLine(x-5, y, x+5, y);
		ofLine(x, y-5, x, y+5);
	}
	
	if(selecting){
		ofNoFill();
		ofRect(selection.x, selection.y, selection.width, selection.height);	
	}

	ofSetColor(0xffffff);
	ofDrawBitmapString("capture  : "+ofToString((int) ofGetFrameRate())+" fps", 20, 20);
	ofDrawBitmapString(status, 20, 35);
	ofDrawBitmapString(ofToString(flags), 20, 50);


}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	if(key==32){
		IplImage* eig = cvCreateImage( cvGetSize(grayImage.getCvImage()), 32, 1 );
		IplImage* temp = cvCreateImage( cvGetSize(grayImage.getCvImage()), 32, 1 );

		count = 350;

		cvGoodFeaturesToTrack( grayImage.getCvImage(), eig, temp, points[1], &count, quality, min_distance, 0, 3, 0, 0.04 );
		cvFindCornerSubPix( grayImage.getCvImage(), points[1], count, cvSize(win_size,win_size), cvSize(-1,-1), cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));

		cvReleaseImage( &eig );
		cvReleaseImage( &temp );

		opticalFlow = true;
	}
	else{
		count = 0;
		opticalFlow = false;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	//x = (float) x / (float) ofGetWidth() * 320.0f;
	//y = (float) y / (float) ofGetHeight() * 240.0f;

	selection.x = MIN(x,origin.x);
	selection.y = MIN(y,origin.y);
	selection.width = selection.x + CV_IABS(x - origin.x);
	selection.height = selection.y + CV_IABS(y - origin.y);

	selection.x = MAX( selection.x, 0 );
	selection.y = MAX( selection.y, 0 );
	//selection.width = MIN( selection.width, ofGetWidth() );
	//selection.height = MIN( selection.height, ofGetHeight() );
	selection.width -= selection.x;
	selection.height -= selection.y;


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	//x = (float) x / (float) ofGetWidth() * 320.0f;
	//y = (float) y / (float) ofGetHeight() * 240.0f;

	origin = cvPoint(x,y);
	selection = cvRect(x,y,0,0);
	selecting = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if( selection.width > 0 && selection.height > 0 ){
		IplImage* eig = cvCreateImage( cvGetSize(grayImage.getCvImage()), 32, 1 );
		IplImage* temp = cvCreateImage( cvGetSize(grayImage.getCvImage()), 32, 1 );
		
		selection.x = (float) selection.x / (float) ofGetWidth() * 640.0;
		selection.y = (float) selection.y / (float) ofGetHeight() * 480.0;

		selection.width = (float) selection.width / (float) ofGetWidth() * 640.0;
		selection.height = (float) selection.height / (float) ofGetHeight() * 480.0;

		cvSetImageROI( grayImage.getCvImage(), selection );
		cvSetImageROI( eig, selection );
		cvSetImageROI( temp, selection );

		count = 400;

		cvGoodFeaturesToTrack( grayImage.getCvImage(), eig, temp, points[1], &count, quality, min_distance, 0, 3, 0, 0.04 );
		//cvFindCornerSubPix( grayImage.getCvImage(), points[1], count, cvSize(win_size,win_size), cvSize(-1,-1), cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
		
		for(int i=0; i<count; i++){
			points[1][i].x += selection.x;
			points[1][i].y += selection.y;
		}

		cvResetImageROI( grayImage.getCvImage() );
		cvReleaseImage( &eig );
		cvReleaseImage( &temp );
		opticalFlow = true;
	}
	else{
		if(count<MAX_COUNT){
			x = (float) x / (float) ofGetWidth() * 640.0;
			y = (float) y / (float) ofGetHeight() * 480.0;

			points[0][count].x = x;
			points[0][count].y = y;
			points[1][count].x = x;
			points[1][count].y = y;
			count++;
			opticalFlow = true;
		}
	}
	selecting = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

