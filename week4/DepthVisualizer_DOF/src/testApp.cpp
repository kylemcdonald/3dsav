#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(60);
	
	input.setup(panel);  // < -- could pass in useKinnect here?
	
	panel.setup("Control Panel", 1024-310, 5, 300, 600);
	
	panel.addPanel("Threshold and Scale");
	panel.addSlider("near threshold", "nearThreshold", 255, 0, 255, true);
	panel.addSlider("far threshold", "farThreshold", 0, 0, 255, true);
	
	panel.addPanel("Control");
	panel.addSlider("scale", "scale", 7, 1, 20);	
	panel.addSlider("rotate y axis", "rotateY", 0, -360, 360, false);	
	panel.addToggle("auto rotate", "autoRotate", false);
	panel.addToggle("draw debug", "drawDebug", false);
	
	if (input.usingKinect() == false){
		panel.addSlider("playback speed", "playSpeed", 0.5, -1, 1, false);
	} else {
		panel.addToggle("draw scene bounding frustrum", "drawSceneBox", false);
	}
	
	panel.addPanel("DOF");
	panel.addSlider("focusDistance", "focusDistance", 1000, 0, 1600);
	panel.addSlider("aperture", "aperture", 0.06, 0.001, 0.2);
	panel.addSlider("pointBrightness", "pointBrightness", .8, 0, 1);
	panel.addSlider("rgbBrightness", "rgbBrightness", 3, 0, 10);
	panel.addSlider("maxPointSize", "maxPointSize", 20, 5, 40);
	
	//panel.addToggle("record", "doRecording", false);
	
	dofShader.setup("shaders/DOFCloud");
	
}

//--------------------------------------------------------------
void testApp::update() {
	input.update();
}

//--------------------------------------------------------------
void testApp::draw() {
	
	ofBackground(0, 0, 0);
	
	// draw debug or non debug
	
	if (panel.getValueB("drawDebug")){
		input.drawDebug();
		
	} else {
		ofPushMatrix();
		
			// center everything
			ofTranslate(ofGetWidth()/2, ofGetWidth()/2, -500);
			ofSetColor(255, 255, 255);
			ofRotateY(panel.getValueF("rotateY"));
			if (panel.getValueB("autoRotate")){
				ofRotateY(ofGetElapsedTimef()*5);
			}
			
			float scale = panel.getValueF("scale");
			ofScale(scale, scale, scale);
			
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0);

			// super helpful: http://pmviewer.sourceforge.net/method.php
			glEnable(GL_POINT_SMOOTH);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);

			dofShader.begin();
			
			dofShader.setUniform1f("focusDistance", panel.getValueF("focusDistance"));
			dofShader.setUniform1f("aperture", panel.getValueF("aperture"));
			dofShader.setUniform1f("pointBrightness", panel.getValueF("pointBrightness"));
			dofShader.setUniform1f("rgbBrightness", panel.getValueF("rgbBrightness"));
			dofShader.setUniform1f("maxPointSize", panel.getValueF("maxPointSize"));

			input.drawPerspective();
			dofShader.end();
			
		ofPopMatrix();
		
	}
	
}

//--------------------------------------------------------------
void testApp::exit() {
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	if(key == 'f') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
}

