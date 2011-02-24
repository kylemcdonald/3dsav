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
	
	panel.addToggle("draw scene bounding frustrum", "drawSceneBox", false);
		
	if (input.usingKinect() == false){
		panel.addSlider("playback speed", "playSpeed", 0.5, -1, 1, false);
		panel.addToggle("playbackPause", "playbackPause", false);
	}
	
	panel.addPanel("DOF");
	panel.addToggle("usePoints", "usePoints", true);	
	panel.addSlider("focusDistance", "focusDistance", 1300, 0, 1600);
	panel.addSlider("aperture", "aperture", 0.06, 0.001, 0.2);
	panel.addSlider("pointBrightness", "pointBrightness", .8, 0, 1);
	panel.addSlider("rgbBrightness", "rgbBrightness", 3, 0, 10);
	panel.addSlider("maxPointSize", "maxPointSize", 20, 5, 40);
	panel.addToggle("spiralLines", "spiralLines", false);
	panel.addSlider("maxLineLength", "maxLineLength", 50, 0, 500);
	panel.addSlider("maxLines", "maxLines", 20000, 0, 100000);
	
	//panel.addToggle("record", "doRecording", false);
	
	dofPoints.setup("shaders/DOFCloud");
	dofLines.setup("shaders/DOFLine");
	
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

			if(panel.getValueB("usePoints")) {
				dofPoints.begin();
				
				dofPoints.setUniform1f("focusDistance", panel.getValueF("focusDistance"));
				dofPoints.setUniform1f("aperture", panel.getValueF("aperture"));
				dofPoints.setUniform1f("pointBrightness", panel.getValueF("pointBrightness"));
				dofPoints.setUniform1f("rgbBrightness", panel.getValueF("rgbBrightness"));
				dofPoints.setUniform1f("maxPointSize", panel.getValueF("maxPointSize"));

				input.drawPerspective();
				
				dofPoints.end();
			} else {
				dofLines.begin();
							
				dofLines.setUniform1f("aspectRatio", ofGetWidth() / ofGetHeight());
				dofLines.setUniform1f("lineWidth", 1);
				dofLines.setUniform1f("focusDistance", panel.getValueF("focusDistance"));
				dofLines.setUniform1f("aperture", panel.getValueF("aperture"));
				dofPoints.setUniform1f("pointBrightness", panel.getValueF("pointBrightness"));
				dofPoints.setUniform1f("rgbBrightness", panel.getValueF("rgbBrightness"));
				
				GLint sideLocation = dofLines.getAttributeLocation("side");
				GLint nextLocation = dofLines.getAttributeLocation("next");
				
				vector<ofxVec3f>& points = input.pointCloud;
				
				float maxLineLength = panel.getValueF("maxLineLength");
				float maxLines = panel.getValueF("maxLines");
				
				ofSeedRandom(0); // same random numbers every frame
				
				int n = points.size();
				
				if(panel.getValueB("spiralLines")) {
					glBegin(GL_TRIANGLE_STRIP);
										
					float w = input.camWidth;
					float h = input.camHeight;
					float steps = 100.;
					for(int i = 0; i < maxLines; i++) {
					
						float theta = i / steps;
						float r = (input.camHeight / 2) * ((float) i / maxLines);
						int x = (w / 2) + r * cos(theta);
						int y = (h / 2) + r * sin(theta);
						x = ofClamp(x, 0, w);
						y = ofClamp(y, 0, h);
						
						float ntheta = (i + 1) / steps;
						float nr = (input.camHeight / 2) * ((float) (i + 1) / maxLines);
						int nx = (w / 2) + nr * cos(ntheta);
						int ny = (h / 2) + nr * sin(ntheta);
						nx = ofClamp(nx, 0, w);
						ny = ofClamp(ny, 0, h);
						
						ofxVec3f& cur = input.pointGrid[y][x];
						ofxVec3f& next = input.pointGrid[ny][nx];
						
						// simple check to make sure we're not at 0,0,0
						if(next.z != 0 && cur.z != 0) {
							// place two vertices to represent the top and bottom
							// at this vertex
							dofLines.setAttribute1f(sideLocation, -.5);
							dofLines.setAttribute3f(nextLocation, next.x, next.y, next.z);
							glVertex3f(cur.x, cur.y, cur.z); // top vertex
							dofLines.setAttribute1f(sideLocation, +.5);
							dofLines.setAttribute3f(nextLocation, next.x, next.y, next.z);
							glVertex3f(cur.x, cur.y, cur.z); // bottom vertex
						}
					}
					glEnd();
				} else {
					for(int i = 0; i < maxLines; i++) {
						ofxVec3f& cur = points[(int) ofRandom(0, n)];
						ofxVec3f& next = points[(int) ofRandom(0, n)];
						
						float lineLength = cur.distance(next);
						
						glBegin(GL_TRIANGLE_STRIP);
						if(lineLength < maxLineLength) {
							// place two vertices to represent the top and bottom
							// of the beginning of the line
							dofLines.setAttribute1f(sideLocation, -.5);
							dofLines.setAttribute3f(nextLocation, next.x, next.y, next.z);
							glVertex3f(cur.x, cur.y, cur.z); // top vertex
							dofLines.setAttribute1f(sideLocation, +.5);
							dofLines.setAttribute3f(nextLocation, next.x, next.y, next.z);
							glVertex3f(cur.x, cur.y, cur.z); // bottom vertex
							
							// extrapolate where the line would go next
							ofxVec3f after = next + (next - cur);
							
							// place two vertices to represent the top and bottom
							// of the end of the line
							dofLines.setAttribute1f(sideLocation, -.5);
							dofLines.setAttribute3f(nextLocation, after.x, after.y, after.z);
							glVertex3f(next.x, next.y, next.z); // top vertex
							dofLines.setAttribute1f(sideLocation, +.5);
							dofLines.setAttribute3f(nextLocation, after.x, after.y, after.z);
							glVertex3f(next.x, next.y, next.z); // bottom vertex
						}
						glEnd();
					}
				}
				
				dofLines.end();
			}
			
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

