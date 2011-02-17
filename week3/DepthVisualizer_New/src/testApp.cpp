#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(60);
	
	input.setup();  // < -- could pass in useKinnect here?
	
	panel.setup("Control Panel", 5, 5, 300, 600);
	panel.addPanel("Threshold and Scale");
	panel.addPanel("Control");
	
	panel.setWhichPanel("Threshold and Scale");
	panel.addSlider("near threshold", "nearThreshold", 255, 0, 255, true);
	panel.addSlider("far threshold", "farThreshold", 0, 0, 255, true);
	panel.addSlider("depth scale", "depthScale", 5, 1, 20);
	panel.addSlider("depth offset", "depthOffset", 128, 0, 255);
	panel.addSlider("step size", "stepSize", 2, 1, 4, true);
	panel.addSlider("point size", "pointSize", 1, 1, 10, true);
	panel.addToggle("draw zeros", "drawZeros", false);
	
	panel.setWhichPanel("Control");
	panel.addSlider("rotate y axis", "rotateY", 0, -360, 360, false);	
	panel.addToggle("auto rotate", "autoRotate", false);
	panel.addToggle("draw scene bounding box", "drawSceneBox", false);
	
	panel.addToggle("draw debug", "drawDebug", false);
	
}

//--------------------------------------------------------------
void testApp::update() {
	
	
	input.update();
		
	
	centroid.set(0,0,0);
	int nPixels = 0;
	
	int width = input.depthImage.getWidth();
    int height = input.depthImage.getHeight();
    unsigned char * depthPixels = input.depthImage.getPixels();
    bool needsSetting = true;
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int i = y * width + x;
            unsigned char curDepthValue = depthPixels[i];
            if(curDepthValue != 0) {
				
				centroid.x += x;
				centroid.y += y;
				centroid.z += curDepthValue;
				nPixels++;
				
				
                float curPosition[] = {x, y, curDepthValue};
                if(needsSetting) {
                    for(int j = 0; j < 3; j++) {
                        maxBound[j] = curPosition[j];
                        minBound[j] = curPosition[j];
                    }
                    needsSetting = false;
                } else {
                    for(int j = 0; j < 3; j++) {
                        if(curPosition[j] > maxBound[j]) {
                            maxBound[j] = curPosition[j];
                        }
                        if(curPosition[j] < minBound[j]) {
                            minBound[j] = curPosition[j];
                        }
                    }
                }
            }
        }
    }
	
	if (nPixels > 0){
		centroid.x /= (float)nPixels;
		centroid.y /= (float)nPixels;
		centroid.z /= (float)nPixels;
	}
	
	
	
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
		ofTranslate(ofGetWidth()/2, ofGetWidth()/2, 0);
		ofSetColor(255, 255, 255);
		ofRotateY(panel.getValueF("rotateY"));
		if (panel.getValueB("autoRotate")){
			ofRotateY(ofGetElapsedTimef()*5);
		}
		ofTranslate(-input.camWidth / 2, -input.camHeight / 2);
		
		//drawPointCloud();
		// draw anything else: 
		
		
		ofLine(minBound[0],minBound[1], minBound[2], maxBound[0],minBound[1], minBound[2]);
		ofLine(minBound[0],minBound[1], minBound[2], minBound[0],maxBound[1], minBound[2]);
		ofLine(minBound[0],minBound[1], minBound[2], minBound[0],minBound[1], maxBound[2]);
		
		ofBox(centroid.x, centroid.y, centroid.z, 10);
		
		
		ofPopMatrix();
		
	
	}

}

//--------------------------------------------------------------
void testApp::drawPointCloud() {
	
}

//--------------------------------------------------------------
void testApp::exit() {
	
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

