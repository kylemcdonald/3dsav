#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(100,100,100);

	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
	#else
        vidPlayer.loadMovie("singlePerson-whiteBackground.mov");
        vidPlayer.play();
	#endif

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);
	grayDiffTemp.allocate(320,240);

	bLearnBakground = true;
	
	panel.setup("control", 770, 0, 300, 400);
	panel.addPanel("image processing", 1, false);
	
	panel.setWhichPanel("image processing");
	
	panel.addSlider("theshold", "THRESHOLD", 80,0,255, true);
	panel.addSlider("nDilates", "NUM_DILATES", 0,0,20, true);
	panel.addSlider("nErosion", "NUM_EROSION", 0,0,20, true);
	panel.addSlider("blur", "BLUR_KERNEL", 0,0,100, true);
	panel.addSlider("median", "NUM_MEDIAN", 0,0,40, true);
	
	panel.loadSettings("settings.xml");

	
}

//--------------------------------------------------------------
void testApp::update(){
	
	panel.update();
	
	

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
				
				
		grayDiff.threshold(panel.getValueI("THRESHOLD"));
	
		for (int i = 0; i < panel.getValueI("NUM_DILATES"); i++){
			grayDiff.dilate_3x3();
		}
		
		for (int i = 0; i < panel.getValueI("NUM_EROSION"); i++){
			grayDiff.erode_3x3();
		}
		
		// not really useful, but just for fun
		if (panel.getValueI("BLUR_KERNEL") > 0){
			int nBlur = panel.getValueI("BLUR_KERNEL") * 2 + 1;
			grayDiff.blur(nBlur);
		}
		
		if (panel.getValueI("NUM_MEDIAN")){
			// since median isn't implemented in ofxCvGrayscaleImage, we do into another
			// image then copy back. 
			int nBlur = panel.getValueI("NUM_MEDIAN") * 2 + 1;
			cvSmooth(grayDiff.getCvImage(), grayDiffTemp.getCvImage(), CV_MEDIAN, nBlur);
			grayDiff = grayDiffTemp;
		}
		
		
		// calculate the centroid and bounding box
		// 
		
		centroid.x = 0;
		centroid.y = 0;
		int nWhitePixels = 0;
		
		int minx = 0;
		int maxx = 0;
		int miny = 0;
		int maxy = 0;
		bool bFirstTime = true;
		boundingBox.x = 0;
		boundingBox.y = 0;
		boundingBox.width = 0;
		boundingBox.height = 0;
		
		unsigned char * pixels = grayDiff.getPixels();
		
		for (int i = 0; i < grayDiff.width; i++){
			for (int j = 0; j < grayDiff.height; j++){
				if (pixels[ j * grayDiff.width + i ] > 0){
					centroid.x += i;
					centroid.y += j;
					nWhitePixels++;
					
					if (bFirstTime == true){
						minx = maxx = i;
						miny = maxy = j;
						bFirstTime = false;
					} else {
						
						if (i < minx){
							minx = i;	
						}
						if (i > maxx){
							maxx = i;	
						}
						if (j < miny){
							miny = j;	
						}
						if (j > maxy){
							maxy = j;	
						}
						
						
					}
					
					
				}
			}
		}
		
		if (nWhitePixels > 0){
			centroid.x /= (float)nWhitePixels;
			centroid.y /= (float)nWhitePixels;
			boundingBox.x = minx;
			boundingBox.y = miny;
			boundingBox.width = maxx - minx;
			boundingBox.height = maxy - miny;
			
		}
		
		

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
	}

}

//--------------------------------------------------------------
void testApp::draw(){

	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetColor(0xffffff);
	colorImg.draw(20,20);
	
	// draw centroid 
	ofFill();
	ofSetColor(255,0,0);
	ofCircle(centroid.x + 20, centroid.y + 20, 10);
	
	ofNoFill();
	ofRect(boundingBox.x + 20, boundingBox.y + 20, boundingBox.width, boundingBox.height);
	
	ofSetColor(255,255,255);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);

	// then draw the contours:

	ofFill();
	ofSetColor(0x333333);
	ofRect(360,540,320,240);
	ofSetColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(360,540);

	// or, instead we can draw each blob individually,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
    }

	// finally, a report:

	ofSetColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\n\nnum blobs found %i, fps: %f", contourFinder.nBlobs, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);

	
	panel.draw();
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	panel.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	panel.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	panel.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

