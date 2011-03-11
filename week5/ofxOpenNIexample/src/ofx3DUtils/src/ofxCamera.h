#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"

enum cameraOrigin {
    OF_ORIGIN,
    OF_ORIGIN_ZERO,
    };

class ofxCamera{
public:
	ofxCamera();

	void position(float x, float y, float z);
	void position(ofxVec3f _pos);
	void position(); //reset the position to initial values
	void lerpPosition(float _x, float _y, float _z, float _step); //step should be a value between 0 and 1
	void lerpPosition(ofxVec3f _pos, float _step); //step should be a value between 0 and 1
	void lerpEye(float _x, float _y, float _z, float _step); //step should be a value between 0 and 1
	void lerpEye(ofxVec3f _target, float _step); //step should be a value between 0 and 1

	void eye(float _x, float _y, float _z);
	void eye(ofxVec3f _eye);
	void eye(); //reset eye psition to the initial values
	void up(float _x, float _y, float _z);
	void up(ofxVec3f _up);
	void up(); //reset up vector to initial values

	void perspective(float _fov, float _aspect, float _zNear, float _zFar);
	void perspective();//reset perspective to initial values

    void setOrigin(cameraOrigin org); // This sets the camera origin to either OF coordinates or zero (for OF_ORIGIN_ZERO)
	virtual void place(); //this must go in the draw function!!!!
	void remove(); //Removes the camera, so it returns as if there was no camera

	void moveLocal(float _x, float _y, float _z); //Moves the camera along it's own coordinatesystem
	void moveLocal(ofxVec3f move);
	void moveGlobal(float _x, float _y, float _z); //Moves the camera along the global coordinatesystem
	void moveGlobal(ofxVec3f move);

	void orbitAround(ofxVec3f target, ofxVec3f axis, float value);
	void rotate(ofxVec3f axis, float value);

	ofxVec3f getDir();
	ofxVec3f getPosition();
	ofxVec3f getEye();
	ofxVec3f getUp();

protected:
	ofxVec3f posCoord;
	ofxVec3f eyeCoord;
	ofxVec3f upVec;

	//relative to defining the persperctive:
	cameraOrigin    origin;
	float	fieldOfView;
	int	w;
	int	h;
	float	aspectRatio;
	float zNear, zFar;
};



