#include "DepthVisualizerInput.h"
#include "testApp.h"

const int useKinect =false;

void DepthVisualizerInput::setup(ofxControlPanel& panel){
	this->panel = &panel;
	
	camWidth = 640;
	camHeight = 480;
	
	rawDepth = new float [camWidth * camHeight]; //.resize(camWidth * camHeight);
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
		
		rawNearThreshold = 100;
		rawFarThreshold = 300;
		
		animation.load("kinectScan");
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
		int frame = ((int)(ofGetFrameNum() * 0.5)) % animation.size();
		cout << frame << endl;
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
		scale =  panel->getValueF("depthScale");
		trans = -panel->getValueF("depthOffset");
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
	} else {
		
		
		
		double fx_d = 1.0 / 5.9421434211923247e+02;
		double fy_d = 1.0 / 5.9104053696870778e+02;
		float cx_d = 3.3930780975300314e+02;
		float cy_d = 2.4273913761751615e+02;
		
		
		pointCloud.clear();
		// this offset will center the data on the center of the scene
		float offset = -(rawNearThreshold + rawFarThreshold) / 2;
		for(int y = 0; y < camHeight; y++) {
			for(int x = 0; x < camWidth; x++) {
				int i = y * camWidth + x;
				if(depthPixels[i] != 0) {
					// ok where are we: 
					float pct = depthPixels[i] / 255.0;
					
					
					float zval = rawNearThreshold + (1-pct) * (rawFarThreshold - rawNearThreshold);
					
					float z = zval / 100.0f;
					ofPoint result;
					result.x = float((x - cx_d) * z * fx_d);
					result.y = float((y - cy_d) * z * fy_d);
					result.z = z;
					
					
					ofxVec3f cur;
					cur.set(result.x, result.y, result.z);
					//ofxVec3f cur = kinect.getWorldCoordinateFor((float)x, (float)y, zval/100.0f);
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
	
	
	ofSetColor(255,255,255,80);
	
	if (panel->getValueB("drawSceneBox")){
		
		float offset = -(rawNearThreshold + rawFarThreshold) / 2;
		
		ofxPoint3f pts[8];
		
		pts[0] =  kinect.getCalibration().getWorldCoordinateFor(0, 0, rawNearThreshold/100.0);
		pts[1] =  kinect.getCalibration().getWorldCoordinateFor(camWidth, 0, rawNearThreshold/100.0);
		pts[2] =  kinect.getCalibration().getWorldCoordinateFor(camWidth, camHeight, rawNearThreshold/100.0);
		pts[3] =  kinect.getCalibration().getWorldCoordinateFor(0, camHeight, rawNearThreshold/100.0);
		pts[4] =  kinect.getCalibration().getWorldCoordinateFor(0, 0, rawFarThreshold/100.0);
		pts[5] =  kinect.getCalibration().getWorldCoordinateFor(camWidth, 0, rawFarThreshold/100.0);
		pts[6] =  kinect.getCalibration().getWorldCoordinateFor(camWidth, camHeight, rawFarThreshold/100.0);
		pts[7] =  kinect.getCalibration().getWorldCoordinateFor(0, camHeight, rawFarThreshold/100.0);
		
		
		for (int i = 0; i < 8; i++){
			pts[i]*= 100;
			pts[i].z += offset;
		}
		
		ofLine(pts[0], pts[1]);
		ofLine(pts[1], pts[2]);
		ofLine(pts[2], pts[3]);
		ofLine(pts[3], pts[0]);
		
		ofLine(pts[4], pts[5]);
		ofLine(pts[5], pts[6]);
		ofLine(pts[6], pts[7]);
		ofLine(pts[7], pts[4]);
		
		ofLine(pts[0], pts[4]);
		ofLine(pts[1], pts[5]);
		ofLine(pts[2], pts[6]);
		ofLine(pts[3], pts[7]);
	}
	
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