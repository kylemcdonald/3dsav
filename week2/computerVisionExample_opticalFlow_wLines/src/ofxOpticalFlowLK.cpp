/*
 *  ofxOpticalFlow.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 31/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxOpticalFlowLK.h"

ofxOpticalFlowLK :: ofxOpticalFlowLK()
{
	opticalFlowSize	= 5;
	opticalFlowBlur = 7;
	opticalFlowMin	= 2;
	opticalFlowMax	= 10;
}

ofxOpticalFlowLK :: ~ofxOpticalFlowLK()
{
	destroy();
}

///////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////

void ofxOpticalFlowLK :: setup ( const ofRectangle& size )
{
	setup( size.width, size.height );
}

void ofxOpticalFlowLK :: setup ( int width, int height )
{
	sizeSml.width	= width;
	sizeSml.height	= height;
	
	sizeLrg.width	= width;		// assume input size is the same as optical flow size.
	sizeLrg.height	= height;
	
	if( bInitialised )
		destroy();
	
	colrImgLrg.allocate( sizeLrg.width, sizeLrg.height );
	colrImgSml.allocate( sizeSml.width, sizeSml.height );
	greyImgLrg.allocate( sizeLrg.width, sizeLrg.height );
	greyImgSml.allocate( sizeSml.width, sizeSml.height );
	greyImgPrv.allocate( sizeSml.width, sizeSml.height );
	greyImgDif.allocate( sizeSml.width, sizeSml.height );
	
	opFlowVelX = cvCreateImage( cvSize( sizeSml.width, sizeSml.height ), IPL_DEPTH_32F, 1 );
	opFlowVelY = cvCreateImage( cvSize( sizeSml.width, sizeSml.height ), IPL_DEPTH_32F, 1 );
	
	reset();
	
	bInitialised = true;
}

void ofxOpticalFlowLK :: reset ()
{
	colrImgLrg.set( 0 );
	colrImgSml.set( 0 );
	greyImgLrg.set( 0 );
	greyImgSml.set( 0 );
	greyImgPrv.set( 0 );
	greyImgDif.set( 0 );

    cvSetZero( opFlowVelX );
    cvSetZero( opFlowVelY );
}

void ofxOpticalFlowLK :: destroy ()
{
	colrImgLrg.clear();
	colrImgSml.clear();
	greyImgLrg.clear();
	greyImgSml.clear();
	greyImgPrv.clear();
	greyImgDif.clear();
	
	cvReleaseImage( &opFlowVelX );
	cvReleaseImage( &opFlowVelY );
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void ofxOpticalFlowLK :: update	( ofImage& source )
{
	update( source.getPixels(), source.width, source.height, source.type );
}

void ofxOpticalFlowLK :: update	( ofxCvColorImage& source )
{
	update( source.getPixels(), source.width, source.height, OF_IMAGE_COLOR );
}

void ofxOpticalFlowLK :: update ( ofxCvGrayscaleImage& source )
{
	update( source.getPixels(), source.width, source.height, OF_IMAGE_GRAYSCALE );
}

void ofxOpticalFlowLK :: update ( ofVideoPlayer& source )
{
	update( source.getPixels(), source.width, source.height, OF_IMAGE_COLOR );	// assume colour image type.
}

void ofxOpticalFlowLK :: update ( ofVideoGrabber& source )
{
	update( source.getPixels(), source.width, source.height, OF_IMAGE_COLOR );	// assume colour image type.
}

void ofxOpticalFlowLK :: update ( unsigned char* pixels, int width, int height, int imageType )
{
	bool rightSize = ( sizeSml.width == width && sizeSml.height == height );
	
	//-- making the input the right size for optical flow to work with.
	
	if( rightSize )
	{
		if( imageType == OF_IMAGE_COLOR )
		{
			colrImgSml.setFromPixels( pixels, sizeSml.width, sizeSml.height );
			greyImgSml.setFromColorImage( colrImgSml );
		}
		else if( imageType == OF_IMAGE_GRAYSCALE )
		{
			greyImgSml.setFromPixels( pixels, sizeSml.width, sizeSml.height );
		}
		else
		{
			return;		// wrong image type.
		}
	}
	else
	{
		bool sizeLrgChanged = ( sizeLrg.width != width || sizeLrg.height != height );
		
		if( sizeLrgChanged )		// size of input has changed since last update.
		{
			sizeLrg.width	= width;
			sizeLrg.height	= height;
			
			colrImgLrg.clear();
			greyImgLrg.clear();
			
			colrImgLrg.allocate( sizeLrg.width, sizeLrg.height );
			greyImgLrg.allocate( sizeLrg.width, sizeLrg.height );
			
			colrImgLrg.set( 0 );
			greyImgLrg.set( 0 );
		}
		
		if( imageType == OF_IMAGE_COLOR )
		{
			colrImgLrg.setFromPixels( pixels, sizeLrg.width, sizeLrg.height );
			colrImgSml.scaleIntoMe( colrImgLrg );
			greyImgSml.setFromColorImage( colrImgSml );
		}
		else if( imageType == OF_IMAGE_GRAYSCALE )
		{
			greyImgLrg.setFromPixels( pixels, sizeLrg.width, sizeLrg.height );
			greyImgSml.scaleIntoMe( greyImgLrg );
		}
		else
		{
			return;		// wrong image type.
		}
	}
	
	if( bMirrorH || bMirrorV )
		greyImgSml.mirror( bMirrorV, bMirrorH );
	
	int opFlowSize = opticalFlowSize;	// value must be 1, 3, 5, 7... etc.
	opFlowSize *= 2;
	opFlowSize += 1;
	
	cvCalcOpticalFlowLK( greyImgPrv.getCvImage(), greyImgSml.getCvImage(),	cvSize( opFlowSize, opFlowSize), opFlowVelX, opFlowVelY );
	
	int opFlowBlur = opticalFlowBlur;	// value must be 1, 3, 5, 7... etc.
	opFlowBlur *= 2;
	opFlowBlur += 1;
	
	cvSmooth( opFlowVelX, opFlowVelX, CV_BLUR, opFlowBlur );
	cvSmooth( opFlowVelY, opFlowVelY, CV_BLUR, opFlowBlur );
	
	greyImgPrv = greyImgSml;
}

///////////////////////////////////////////
//	OP.FLOW VELOCITY GETTERS.
///////////////////////////////////////////

ofPoint ofxOpticalFlowLK :: getVelAtNorm ( float x, float y )
{
	x = ofClamp( x, 0.0, 1.0 );
	y = ofClamp( y, 0.0, 1.0 );
	
	int xi = x * ( sizeSml.width  - 1 );
	int yi = y * ( sizeSml.height - 1 );
	
	ofPoint p;
	p.x = cvGetReal2D( opFlowVelX, yi, xi );
	p.y = cvGetReal2D( opFlowVelY, yi, xi );
	
	limitMin( p, opticalFlowMin );		// should look at making this optional, to save on cpu.
	limitMax( p, opticalFlowMax );		// should look at making this optional, to save on cpu.
	
	return p;
}

ofPoint ofxOpticalFlowLK :: getVelAtPixel ( int x, int y )
{
	x = ofClamp( x, 0, sizeSml.width  - 1 );
	y = ofClamp( y, 0, sizeSml.height - 1 );
	
	ofPoint p;
	p.x = cvGetReal2D( opFlowVelX, y, x );
	p.y = cvGetReal2D( opFlowVelY, y, x );

	limitMin( p, opticalFlowMin );		// should look at making this optional, to save on cpu.
	limitMax( p, opticalFlowMax );		// should look at making this optional, to save on cpu.
	
	return p;
}

///////////////////////////////////////////
//	UTILS.
///////////////////////////////////////////

void ofxOpticalFlowLK :: limitMin ( ofPoint& p, float min )
{
	float length = (float)sqrt( p.x * p.x + p.y * p.y );
	
	if( length < min )
	{
		p.x = 0;
		p.y = 0;
	}
}

void ofxOpticalFlowLK :: limitMax ( ofPoint& p, float max )
{
	float lengthSquared = ( p.x * p.x + p.y * p.y );
	
	if( lengthSquared > max * max && lengthSquared > 0 ) 
	{
		float ratio = max / (float)sqrt( lengthSquared );
		
		p.x *= ratio;
		p.y *= ratio;
	}
	
	float length = (float)sqrt( p.x * p.x + p.y * p.y );
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void ofxOpticalFlowLK :: draw ( int width, int height,  float lineScale, int res )
{
	bool rightSize = ( sizeSml.width == width && sizeSml.height == height );
	
	ofPoint vel;
	
	for( int x=0; x<width; x+=res )
	{
		for( int y=0; y<height; y+=res )
		{
			if( rightSize )
				vel = getVelAtPixel( x, y );
			else
				vel = getVelAtNorm( x / (float)width, y / (float)height );
			
			if( vel.x == 0 && vel.y == 0 )
				continue;
			
			ofLine( x, y, x + vel.x * lineScale, y + vel.y * lineScale );
		}
	}
}

///////////////////////////////////////////
//	CONFIG.
///////////////////////////////////////////

void ofxOpticalFlowLK :: setMirror ( bool mirrorHorizontally, bool mirrorVertically )
{
	bMirrorH = mirrorHorizontally;
	bMirrorV = mirrorVertically;
}
