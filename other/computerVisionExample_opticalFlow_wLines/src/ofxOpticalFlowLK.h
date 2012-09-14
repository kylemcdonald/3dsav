/*
 *  ofxOpticalFlow.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 31/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OFX_OPTICAL_FLOW_LK_H
#define OFX_OPTICAL_FLOW_LK_H

#define OPTICAL_FLOW_DEFAULT_WIDTH		320
#define OPTICAL_FLOW_DEFAULT_HEIGHT		240

#include "ofMain.h"
#include "ofxOpenCv.h"

class ofxOpticalFlowLK
{
	
public :
	
	 ofxOpticalFlowLK();
	~ofxOpticalFlowLK();
	
	void setup		( const ofRectangle& size );
	void setup		( int width = OPTICAL_FLOW_DEFAULT_WIDTH, int height = OPTICAL_FLOW_DEFAULT_HEIGHT );
	
	void reset		();
	void destroy	();
	
	void update		( ofImage& source );
	void update		( ofxCvColorImage& source );
	void update		( ofxCvGrayscaleImage& source );
	void update		( ofVideoPlayer& source );
	void update		( ofVideoGrabber& source );
	void update		( unsigned char* pixels, int width, int height, int imageType );
	
	ofPoint getVelAtNorm	( float x, float y );
	ofPoint getVelAtPixel	( int x, int y );
	
	void limitMin	( ofPoint& p, float min );
	void limitMax	( ofPoint& p, float max );
	
	void draw		( int width = OPTICAL_FLOW_DEFAULT_WIDTH, int height = OPTICAL_FLOW_DEFAULT_HEIGHT, float lineScale = 10, int res = 6 );
	
	void setMirror	( bool mirrorHorizontally = false, bool mirrorVertically = false );
	
	bool			bInitialised;
	ofRectangle		sizeSml;
	ofRectangle		sizeLrg;
	
	ofxCvColorImage			colrImgLrg;		// full scale color image.
	ofxCvColorImage			colrImgSml;		// full scale color image.
	ofxCvGrayscaleImage		greyImgLrg;		// full scale grey image.
	ofxCvGrayscaleImage		greyImgSml;		// scaled down grey image.
	ofxCvGrayscaleImage		greyImgPrv;		// scaled down grey image - copy of previous frame.
	ofxCvGrayscaleImage		greyImgDif;		// scaled down grey difference image with threshold applied.
	IplImage*				opFlowVelX;		// optical flow in the x direction.
	IplImage*				opFlowVelY;		// optical flow in the y direction.
	
	bool			bMirrorH;
	bool			bMirrorV;
	
	int				opticalFlowSize;
	int				opticalFlowBlur;
	float			opticalFlowMin;
	float			opticalFlowMax;
};

#endif