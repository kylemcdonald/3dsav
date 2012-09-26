#include "testApp.h"

ofVec3f testApp::ConvertProjectiveToRealWorld(const XnPoint3D& p) {
	XnPoint3D world;
	depth.getXnDepthGenerator().ConvertProjectiveToRealWorld(1, &p, &world);
	return ofVec3f(world.X, world.Y, world.Z);
}

void testApp::setup() {
	ofSetVerticalSync(true);
	openni.setup();
	image.setup(&openni);
	depth.setup(&openni);
	user.setup(&openni);
	openni.registerViewport();
	openni.setMirror(true);

	osc.setup("localhost", 8000);
	
	distance = 0;
}

void testApp::update(){
	openni.update();
	image.update();
	depth.update();
	user.update();
	if(user.getNumberOfTrackedUsers() > 0) {
		ofxTrackedUser* cur = user.getTrackedUser(1); // old API starts users at 1, not 0
		ofVec3f leftHand = ConvertProjectiveToRealWorld(cur->left_lower_arm.position[1]);
		ofVec3f rightHand = ConvertProjectiveToRealWorld(cur->right_lower_arm.position[1]);
		
		distance = leftHand.distance(rightHand);
		
		ofxOscMessage msg;
		msg.setAddress("/distance");
		msg.addFloatArg(ofMap(distance, 10, 1000, 0, 1));
		osc.sendMessage(msg);
	}
}

void testApp::draw(){
	ofBackground(0);
	ofSetColor(255);
	ofScale(.75, .75, .75);
	depth.draw(0, 0, 640, 480);
	image.draw(640, 0, 640, 480);
	user.draw();
	ofRect(0, 10, distance, 10);
}