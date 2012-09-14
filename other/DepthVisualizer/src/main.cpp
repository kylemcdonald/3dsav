#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024, 480 * 2, OF_WINDOW);
	ofRunApp(new testApp());
}
