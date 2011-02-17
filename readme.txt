


This folder should be placed at the ROOT level of openframeworks, next to apps, libs, etc.  This is because it's organized by weeks, and it just proved easier to organize it this way. 


+ + + + + + + 


first, download the zip file on this page by clicking the 'download' button at the top right.

then download the zip file on https://github.com/ofTheo/ofxKinect.

unzip ofxKinect and rename the unzipped folder to ofxKinect.

move ofxKinect to your addons/ folder.

ofxControlPanel is distributed with the DepthVisualizer, so you don't need to download it separately. if you need it in the future, you can find it at https://github.com/ofTheo/ofxControlPanel/

if you would like to run with test data instead of using a kinect, there is a short 8 MB download available at http://kylemcdonald.net/3dsav/janus-1283433262.zip be sure to place the unzipped directory in your data directory:

DepthVisualizer/bin/data/janus-1283433262

if you change the variable 'useKinect' at the top of testApp.cpp to 'false', the testApp will try and load the janus-1283433262 animation.

the animation is stored as a sequence of RGBA PNGs, where the alpha encodes depth information.

recording from the kinect into this format is left as an 'exercise for the reader' at the moment ;)

+ + + + 

control panel needs ofxXmlSettings and ofxDirList addons in the project. 

+ + + + 

the optical flow example w features, and good features are modified code from simongeilfus: 
http://www.openframeworks.cc/forum/viewtopic.php?p=16428

the optical flow add-on ofxOpticalFlowLK is from http://code.google.com/p/julapy/

+ + + + 
