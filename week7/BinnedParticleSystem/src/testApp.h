#pragma once

#include "ParticleSystem.h"
#include "ofMain.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	float timeStep;
	int lineOpacity, pointOpacity;
	float particleNeighborhood, particleRepulsion;
	float centerAttraction;

	int kParticles;
	ParticleSystem particleSystem;
	bool isMousePressed, slowMotion;
};
