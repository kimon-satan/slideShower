#pragma once

#include "ofMain.h"
#include <algorithm>
#include "ofxManyMouse.h"
#include "ofxXmlSettings.h"

#include "slide.h"

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
	void loadSlideParameters(slide &s, ofxXmlSettings XML);
	void orderImages(int t_perf);
	void nextSection();
	
	vector<vector<vector<ofImage * > > >images;
	vector<slide> currentSlides;
	vector<vector<slide> > sectionSlides;
	
	int numSections;
	
	vector<string> sectionTitles;
	vector<string> sectionSequence;
	vector<int> triggerPerformer;
	
	vector<int> gridIndexList;
	
	int newSectionCount;
	int gridCount;
	int oneShotCount;
	int screenMode;
	int assigns[3];
	int sequenceIndex[3]; //rename to sequenceIndex
	int deviceNum, currentSection;
	e_performer c_perf;
	

	


	
};

