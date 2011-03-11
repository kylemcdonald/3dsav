/*
 In your test class, add a member variable:
  ofxEasyCam camera;

 Finally, in your draw(), say:
  camera.place();
 To render from the camera's perspective. To draw a HUD, add:
  camera.remove();
 When you're done drawing from the camera's perspective.
*/

/*
 A better implementation would use quaternions,
 code for which is described at the end of this presentation:

 http://www.cs.sunysb.edu/~mueller/teaching/cse564/trackball.ppt

 The code has the advantage of not requiring an implementation of
 a quaternion class.
*/

#pragma once

#include "ofx3DUtils.h"

class ofxEasyCam : public ofxCamera {
private:
	float startMomentum, stopMomentum;
	bool mouseHasMoved, mouseClicked;
	float dmouseX, dmouseY, pmouseX, pmouseY;
	int mouseButton;
public:
	float zoomSpeed, orbitSpeed, panSpeed;

	ofxEasyCam() :
		startMomentum(.5),
		stopMomentum(.5),
		mouseHasMoved(false),
		mouseClicked(false),
		dmouseX(0), dmouseY(0),
		pmouseX(0), pmouseY(0),
		zoomSpeed(1),
		orbitSpeed(.2),
		panSpeed(1)
		{

		ofAddListener(ofEvents.mousePressed, this, &ofxEasyCam::mousePressed);
		ofAddListener(ofEvents.mouseReleased, this, &ofxEasyCam::mouseReleased);
		ofAddListener(ofEvents.mouseDragged, this, &ofxEasyCam::mouseDragged);

		float theta = PI * fieldOfView / 360.0f;
		float opposite = ofGetWidth() / 2;
		posCoord.z = opposite / tanf(theta);
	}
	void place() {
		ofxVec3f relx = getDir().getCrossed(getUp()).normalize();
		ofxVec3f rely = getDir().getCrossed(relx).normalize();
		if(mouseButton == 0) {
			ofxVec3f rot = relx * dmouseY + rely * -dmouseX;
			orbitAround(getEye(), rot, rot.length() * orbitSpeed);
		} else if(mouseButton == 2) {
			ofxVec3f offset = getDir().normalize() * (zoomSpeed * dmouseY);
			eye(getEye() - offset);
			moveGlobal(offset);
		} else if(mouseButton == 1) {
			ofxVec3f offset =
				(relx * dmouseX * panSpeed) +
				(rely * dmouseY * panSpeed);
			moveGlobal(offset);
		}
		dmouseX *= stopMomentum;
		dmouseY *= stopMomentum;
		ofxCamera::place();
	}
	void mousePressed(ofMouseEventArgs& event) {
		if(mouseClicked == true)
			mouseButton = 3; // left + right
		else
			mouseButton = event.button;
		mouseClicked = true;
	}

	void mouseDragged(ofMouseEventArgs& event) {
		int x = event.x;
		int y = event.y;
		if(mouseHasMoved) {
			dmouseX = ofLerp(pmouseX - x, dmouseX, startMomentum);
			dmouseY = ofLerp(pmouseY - y, dmouseY, startMomentum);
		} else
			mouseHasMoved = true;
		pmouseX = x;
		pmouseY = y;
	}

	void mouseReleased(ofMouseEventArgs& event) {
		mouseHasMoved = false;
		mouseClicked = false;
	}
};
