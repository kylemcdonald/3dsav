/*
 *  ofx3dGraphics.cpp
 *  DepthVisualizer
 *
 *  Created by zachary lieberman on 2/16/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofx3dGraphics.h"
#include "ofMain.h"


void ofLine(float x1,float y1,float z1,float x2,float y2,float z2){
	glBegin(GL_LINES);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
	glEnd();
}
void ofLine(const ofPoint & p1, const ofPoint & p2){
	glBegin(GL_LINES);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
	glEnd();
}



//----------------------------------------
void ofSphere(float x, float y, float z, float radius) {
	ofSphere(ofPoint(x, y, z), radius);
}

//----------------------------------------
void ofSphere(float x, float y, float radius) {
	ofSphere(x, y, 0, radius);
}

//----------------------------------------
void ofSphere(const ofPoint& position, float radius) {
	ofPushMatrix();
	ofTranslate(position.x, position.y, position.z);
	ofSphere(radius);
	ofPopMatrix();
}

//----------------------------------------
void ofSphere(float radius) {
	// TODO: add an implementation using ofMesh
#ifndef TARGET_OPENGLES
	// this needs to be swapped out with non-glut code
	// for good references see cinder's implementation from paul bourke:
	// https://github.com/cinder/Cinder/blob/master/src/cinder/gl/gl.cpp
	// void drawSphere( const Vec3f &center, float radius, int segments )
	// and processing's implementation of icospheres:
	// https://code.google.com/p/processing/source/browse/trunk/processing/core/src/processing/core/PGraphics.java?r=7543
	// public void sphere(float r)
	
	ofPushMatrix();
	ofRotateX(90);
	if(ofGetStyle().bFill) {
		glutSolidSphere(radius, 2 * 10, 10);
	} else {
		glutWireSphere(radius, 2 * 10, 10);
	}
	ofPopMatrix();
#endif
}

//----------------------------------------
void ofBox(float x, float y, float z, float size) {
	ofBox(ofPoint(x, y, z), size);
}

//----------------------------------------
void ofBox(float x, float y, float size) {
	ofBox(x, y, 0, size);
}

//----------------------------------------
void ofBox(const ofPoint& position, float size) {
	ofPushMatrix();
	ofTranslate(position.x, position.y, position.z);
	ofBox(size);
	ofPopMatrix();
}

//----------------------------------------
void ofBox(float size) {

	ofPushMatrix();
	ofScale(1, 1, -1);
	
	float h = size * .5;
	
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if(ofGetStyle().bFill) {
		GLfloat vertices[] = {
			+h,-h,+h, +h,-h,-h, +h,+h,-h, +h,+h,+h,
			+h,+h,+h, +h,+h,-h, -h,+h,-h, -h,+h,+h,
			+h,+h,+h, -h,+h,+h, -h,-h,+h, +h,-h,+h,
			-h,-h,+h, -h,+h,+h, -h,+h,-h, -h,-h,-h,
			-h,-h,+h, -h,-h,-h, +h,-h,-h, +h,-h,+h,
			-h,-h,-h, -h,+h,-h, +h,+h,-h, +h,-h,-h
		};
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		
		static GLfloat normals[] = {
			+1,0,0, +1,0,0, +1,0,0, +1,0,0,
			0,+1,0, 0,+1,0, 0,+1,0, 0,+1,0,
			0,0,+1, 0,0,+1, 0,0,+1, 0,0,+1,
			-1,0,0, -1,0,0, -1,0,0, -1,0,0,
			0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0,
			0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1
		};
		glNormalPointer(GL_FLOAT, 0, normals);
		
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//		static GLfloat tex[] = {
//			1,0, 0,0, 0,1, 1,1,
//			1,1, 1,0, 0,0, 0,1,
//			0,1, 1,1, 1,0, 0,0,
//			0,0, 0,1, 1,1, 1,0,
//			0,0, 0,1, 1,1, 1,0,
//			0,0, 0,1, 1,1, 1,0
//		};
//		glTexCoordPointer(2, GL_FLOAT, 0, tex);
		
		GLubyte indices[] = {
			0,1,2, // right top left
			0,2,3, // right bottom right
			4,5,6, // bottom top right
			4,6,7, // bottom bottom left
			8,9,10, // back bottom right
			8,10,11, // back top left
			12,13,14, // left bottom right
			12,14,15, // left top left
			16,17,18, // ... etc
			16,18,19,
			20,21,22,
			20,22,23
		};
		glDrawElements(GL_TRIANGLES, 3 * 6 * 2, GL_UNSIGNED_BYTE, indices);
		
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	} else {
		GLfloat vertices[] = {
			+h,+h,+h,
			+h,+h,-h,
			+h,-h,+h,
			+h,-h,-h,
			-h,+h,+h,
			-h,+h,-h,
			-h,-h,+h,
			-h,-h,-h
		};
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		
		static float n = sqrtf(3);
		static GLfloat normals[] = {
			+n,+n,+n,
			+n,+n,-n,
			+n,-n,+n,
			+n,-n,-n,
			-n,+n,+n,
			-n,+n,-n,
			-n,-n,+n,
			-n,-n,-n
		};
		glNormalPointer(GL_FLOAT, 0, normals);
		
		static GLubyte indices[] = {
			0,1, 1,3, 3,2, 2,0,
			4,5, 5,7, 7,6, 6,4,
			0,4, 5,1, 7,3, 6,2
		};
		glDrawElements(GL_LINES, 4 * 2 * 3, GL_UNSIGNED_BYTE, indices);
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	ofPopMatrix();
}

