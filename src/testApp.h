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
	int getPerformer(string s);
	
	void standardDraw(e_performer t_perf);
	void splitDraw(e_performer t_perf);
	void loadSectionSettings();
	void orderImages(int t_perf);
	void nextSection();
	
	vector<vector<vector<ofImage * > > >images;
	
	int numSections;
	
	vector<string> sectionTitles;
	vector<string> sectionSequence;
	vector<vector<string > >sectionDrawTypes;
	
	vector<vector<float> >sectionAttackTimes;
	vector<vector<float> >sectionHoldTimes;
	vector<vector<float> >sectionFadeTimes;
	
	vector<vector<string> > sectionPositions;
	vector<int> triggerPerformer;
	
	bool isMouseDown[3];
	int currentImage[3];
	int deviceNum, currentSection;
	e_performer c_perf;
	
	float perfAlphas[3];
	float preAlphas[3];
	
	string c_drawMode[3];
	ofVec2f splitPosition[3]; // could be fixed for performer or variable
	
	int screenMode;
	int assigns[3];

	
};

