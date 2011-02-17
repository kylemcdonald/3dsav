
// Note this is code from 0.07 openframeworks -- included here to help make drawing in 3d easier. 
// we'll add functions that we need for now in here. 

#pragma once

#include "ofTypes.h"

void ofLine(float x1,float y1,float z1,float x2,float y2,float z2);
void ofLine(const ofPoint & p1, const ofPoint & p2);

void ofSphere(float x, float y, float z, float radius);
void ofSphere(float x, float y, float radius);
void ofSphere(const ofPoint& position, float radius);
void ofSphere(float radius);

void ofBox(float x, float y, float z, float size);
void ofBox(float x, float y, float size);
void ofBox(const ofPoint& position, float size);
void ofBox(float size);
