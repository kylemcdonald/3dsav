#include "testApp.h"

void testApp::setup() {
	isLive = true;
	isRecording = false;

	setupRecording();
	
	ofBackground(0, 0, 0);
	
	panel.setup("Control Panel", ofGetWidth() - 315, 5, 300, 600);	
	panel.addPanel("Skeleton Tracking");	
	panel.addToggle("track skeletons", "isTracking", true);
	panel.addToggle("mask background", "isMasking", true);	
	panel.addToggle("mask cloud background", "cloudMasking", false);		
	panel.addSlider("cloud z", "zpos", 0, -5000, 5000, false);
	panel.addSlider("cloud rotation", "cloud_rotation", 0, 360, 0, false);	
	panel.addSlider("cloud point size", "cloud_point_size", 1, 1, 50, true);		
	panel.addSlider("z pixels per meter", "ppm", 20, 0, 100, false);
	panel.addToggle("show scene box", "show_scene_box", true);		
	
	if (isLive) {
		recordUser.setPointCloudRotation(0);
	}
	else {
		playUser.setPointCloudRotation(0);
	}	
	
}


void testApp::setupRecording(string _filename) {
	if (!recordContext.isInitialized()) {		
		recordContext.setup();
		recordContext.setupUsingXMLFile();
		recordDepth.setup(&recordContext);
		recordImage.setup(&recordContext);
		recordUser.setup(&recordContext, &recordDepth, &recordImage);
		recordDepth.toggleRegisterViewport(&recordImage);
		recordContext.toggleMirror();
		oniRecorder.setup(&recordContext, &recordDepth, &recordImage);	
	}
	else {
		currentFileName = _filename;
	}
}

void testApp::setupPlayback(string _filename) {
	playContext.clear();
	playContext.setupUsingRecording(ofToDataPath(_filename));
	playDepth.setup(&playContext);
	playImage.setup(&playContext);
	playUser.setup(&playContext, &playDepth, &playImage);
	playDepth.toggleRegisterViewport(&playImage);
	playContext.toggleMirror();
}

void testApp::update(){
	if (isLive) {
		recordContext.update();
		if (panel.getValueB("isTracking")) recordUser.update();
	}
	else {
		playContext.update();
		if (panel.getValueB("isTracking")) playUser.update();
	}
}


void testApp::draw(){	
	ofSetColor(255, 255, 255);
	
	// draw the debug screens
	ofPushMatrix();
	ofScale(0.5, 0.5);
							
	if (isLive) {
		recordDepth.draw(0, 0, 640, 480);
		recordImage.draw(640, 0, 640, 480);
		if (panel.getValueB("isTracking")) {
			recordUser.draw();
			
			if(panel.getValueB("isMasking")) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_DST_COLOR, GL_ZERO);
				recordUser.drawUserMasks(640, 0);
				glDisable(GL_BLEND);
			}
		}
	}
	else {
		playDepth.draw(0, 0, 640, 480);
		playImage.draw(640, 0, 640, 480);
		if (panel.getValueB("isTracking")) {
			
			playUser.draw();
			
			if(panel.getValueB("isMasking")) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_DST_COLOR, GL_ZERO);
				playUser.drawUserMasks(640, 0);
				glDisable(GL_BLEND);
			}
		}
	}
	
	// draw rectangles to ground the debug screens
	ofNoFill();
	ofRect(0, 0, 640, 480);
	ofRect(640, 0, 640, 480);							
	ofFill();	
	
	ofPopMatrix();
	
	// draw the point cloud
	//glMatrixMode(GL_MODELVIEW);	
	
	
//	glPushMatrix();
//	glTranslatef(320, 240, 0);
//	glRotatef(panel.getValueF("cloud_rotation"), 0, 1, 0);
//	glTranslatef(-320, -240, 0);	
	
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, -300);	
	ofRotateY(panel.getValueF("cloud_rotation"));
	ofTranslate(-320, -240, 0);

//	if (panel->getValueB("autoRotate")){
//		ofRotateY(ofGetElapsedTimef()*5);
//	}

	
	
	//glCallList(SCENE); /* draw the scene */
	
	if (isLive) {
		//recordUser.drawPointCloud(!panel.getValueB("cloudMasking"), panel.getValueI("cloud_point_size"));		
		recordUser.drawOrthogonalPointCloud(panel.getValueF("ppm"), false, panel.getValueB("show_scene_box"), panel.getValueI("cloud_point_size"));
		recordUser.drawOrthogonal(panel.getValueF("ppm"));
	}
	else {
		//playUser.drawPointCloud(!panel.getValueB("cloudMasking"), panel.getValueI("cloud_point_size"));
	}


	ofPopMatrix();		
	
	
	ofSetColor(255, 255, 0);
	
	string msg1, msg2, msg3;	//drawBitmapString is limited to some numebr of characters -> is this a bug in 007 or always the case?
	
	msg1 += "Press 's' to start/stop recording\n";
	msg1 += "Press 'p' to toggle playback/live streams\n";
	msg3 += (string)(isLive ? "LIVE STREAM\n" : "PLAY STREAM\n");
	msg3 += (string)(!isRecording ? "READY\n" : "RECORDING\n");

	
	ofDrawBitmapString(msg1, 20, 500);
	ofDrawBitmapString(msg3, 20, 600);
	ofDrawBitmapString(currentFileName, 20, 700);
	
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch (key) {
		case 's':
			if (isRecording) {
				oniRecorder.stopRecord();
				isRecording = false;
				break;
			} else {
				setupRecording(generateFileName());
				oniRecorder.startRecord(currentFileName);
				isRecording = true;
				break;
			}
			break;
		case 'p':
			if (currentFileName != "" && !isRecording && isLive) {
				setupPlayback(currentFileName);
				isLive = false;
			} else {
				isLive = true;
			}
			break;
		default:
			break;
	}	
}

string testApp::generateFileName() {
	
	string _root = "kinectRecord";
	
	string _timestamp = ofToString(ofGetDay()) + 
	ofToString(ofGetMonth()) +
	ofToString(ofGetYear()) +
	ofToString(ofGetHours()) +
	ofToString(ofGetMinutes()) +
	ofToString(ofGetSeconds());
	
	string _filename = (_root + _timestamp + ".oni");
	
	return _filename;
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	if (isLive) {
		//recordUser.setPointCloudRotation(x);
	}
	else {
		//playUser.setPointCloudRotation(x);
	}	

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}