#include "DepthVisualizerInput.h"
#include "testApp.h"

const int useKinect = true;

void DepthVisualizerInput::setup(ofxControlPanel& panel){
	this->panel = &panel;
	
	camWidth = 640;
	camHeight = 480;
	
	rawDepth.resize(camWidth * camHeight);
	depthImage.allocate(camWidth, camHeight);
	
	if(useKinect) {
		//kinect.init(true);  //shows infrared image
		kinect.init();
		kinect.setVerbose(true);
		kinect.open();
		
		// this is setting the raw data conversion range
		// we assume that 100 to 300 CM (1 - 3 meters) is good for tracking a person in space
		rawNearThreshold = 100;
		rawFarThreshold = 300;
		kinect.getCalibration().setClippingInCentimeters(rawNearThreshold, rawFarThreshold);
		
	} else {
		animation.load("janus-1283433262");
		cout << "Animation length is " << animation.size() << " frames." << endl;
	}

}

void DepthVisualizerInput::update(){
	if(useKinect) {
		kinect.update();
		if(kinect.isFrameNew()) {
			depthImage.setFromPixels(kinect.getDepthPixels(), camWidth, camHeight);
			depthImage.flagImageChanged();
		}
	} else {
		int frame = ofGetFrameNum() % animation.size();
		ofImage& cur = animation.getAlpha(frame);
		depthImage.setFromPixels(cur.getPixels(), cur.getWidth(), cur.getHeight());
		depthImage.flagImageChanged();
	}
	
	// do processing here on depthImage
	// getting rid of (setting depth = 0) on pixels we don't care about
	// for example, background, etc. 
	//
	unsigned char* depthPixels = depthImage.getPixels();
	int nearThresh = panel->getValueF("nearThreshold");
	int farThresh = panel->getValueF("farThreshold");
	
	for(int y = 0; y < camHeight; y++) {
		for(int x = 0; x < camWidth; x++) {
			int i = y * camWidth + x;
			if (depthPixels[i] <  farThresh  || depthPixels[i] >= nearThresh){
				depthPixels[i] = 0;
			}
		}
	}
	
	// put distance values at a similar scale to the pixel units
	float scale, trans;
	if(useKinect) {
		scale =  panel->getValueF("depthScale");
		trans = -panel->getValueF("depthOffset");
	} else {
		scale = 2.5;
		trans = -128;
	}
	for(int y = 0; y < camHeight; y++) {
		for(int x = 0; x < camWidth; x++) {
			int i = y * camWidth + x;
			rawDepth[i] = scale * (depthPixels[i] + trans);
		}
	}
	
	// only generate the point cloud for the kinect,
	// since i don't know the parameters for the janus data	
	if(useKinect) {
		pointCloud.clear();
		// this offset will center the data on the center of the scene
		float offset = -(rawNearThreshold + rawFarThreshold) / 2;
		for(int y = 0; y < camHeight; y++) {
			for(int x = 0; x < camWidth; x++) {
				int i = y * camWidth + x;
				if(depthPixels[i] != 0) {
					ofxVec3f cur = kinect.getWorldCoordinateFor(x, y);
					cur *= 100; // convert from meters to cm
					cur.z += offset;
					pointCloud.push_back(ofPoint(cur));
				}
			}
		}
	}
}

void DepthVisualizerInput::drawOrthographic() {
	glEnable(GL_POINT_SMOOTH);
	glPointSize(panel->getValueF("pointSize"));
	
	int step = panel->getValueI("stepSize");
	bool drawZeroes = panel->getValueB("drawZeros");
	
	glBegin(GL_POINTS);
	unsigned char* depthPixels = depthImage.getPixels();
	for(int y = 0; y < camHeight; y += step) {
		for(int x = 0; x < camWidth; x += step) {
			int i = y * camWidth + x;
			if(!(depthPixels[i] == 0 && !drawZeroes)) {
				glVertex3f(x, y, rawDepth[i]);
			}
		}
	}
	glEnd();
}

void DepthVisualizerInput::drawPerspective() {
	glEnable(GL_POINT_SMOOTH);
	glPointSize(panel->getValueF("pointSize"));
	
	glBegin(GL_POINTS);
	for(int i = 0; i < pointCloud.size(); i++) {
		ofPoint cur = pointCloud[i];
		glVertex3f(cur.x, cur.y, cur.z);
	}
	glEnd();
	
	// i would draw the frustum here using
	// kinect.getCalibration().getWorldCoordinateFor(0, 0, rawNearThreshold),
	// ...
	// kinect.getCalibration().getWorldCoordinateFor(camWidth, camHeight, rawFarThreshold)
	// but it would be about 20 lines of code and really annoying to read
	// you would need to do a post-processing that accounts for the fact
	// that we centered the data above
}

void DepthVisualizerInput::drawDebug(){
	ofPushMatrix();
	// center everything
	ofTranslate((ofGetWidth() - camWidth) / 2, 0, 0);
	ofSetColor(255, 255, 255);
	depthImage.draw(0, 0);
	ofPushMatrix();
	ofTranslate(0, camHeight);
	ofTranslate(camWidth / 2, camHeight / 2);
	ofRotateY(((testApp * ) ofGetAppPtr())->panel.getValueF("rotateY"));
	if (((testApp * ) ofGetAppPtr())->panel.getValueB("autoRotate")){
		ofRotateY(ofGetElapsedTimef()*5);
	}
	ofTranslate(-camWidth / 2, -camHeight / 2);
	
	// need something here....
	//drawPointCloud();
	
	ofPopMatrix();
	ofPopMatrix();

}


//--------------------------------------------------------------
void DepthVisualizerInput::exit() {
	if(useKinect) {
		kinect.close();
	}
}