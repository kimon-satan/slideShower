#pragma once

#include "ofMain.h"
#include <algorithm>
#include "ofxManyMouse.h"
#include "ofxXmlSettings.h"

class testApp : public ofBaseApp, public ofxManyMouse{
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	void mousePressed(int device, int button);
	void mouseReleased(int device, int button);

	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);		
	void exit();
	
private:
	
	enum e_performer{
		
		SIMON,
		LUKE,
		CIMEON,
		MISC,
		P_COUNT
		
	};
	
	string getPerformerString(e_performer t);
	
	void standardDraw(e_performer t_perf);
	void splitDraw(e_performer t_perf);
	void loadSectionSettings();
	
	vector<vector<vector<ofImage * > > >images;
	
	int numSections;
	
	vector<string> sectionTitles;
	vector<vector<string > >sectionDrawTypes;
	vector<vector<string> > sectionPositions;
	
	int currentImage[3];
	int deviceNum, currentSection;
	e_performer c_perf;
	
	int perfAlphas[3];
	
	string c_drawMode[3];
	ofVec2f splitPosition[3]; // could be fixed for performer or variable
	
	int screenMode;
	int assigns[3];

	
};

