#include "testApp.h"

bool isSlideDone(slide t){return(t.isFading && t.alpha <= 0);}
bool sortByAlpha(slide a, slide b){
	
	if(a.alpha >= 255 && b.alpha < 255)return true;
	if(a.alpha < 255 && b.alpha >= 255)return false;
	return false;

}

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetVerticalSync(true);
	ofHideCursor();
	ofSetFrameRate(30);
	
	screenMode = 0;
	loadSectionSettings();
	
	ofDirectory dir;
	assigns[0] = 0; assigns[1] = 1; assigns[2] = 2;
	
	string path = "images";
	
	dir.open(path);
	
	for(int i = 0; i < numSections; i ++){
		
		vector<vector<ofImage * > >t_section (4, vector<ofImage *>());
		
		ofDirectory s_dir;
		s_dir.open("images/" + sectionTitles[i]);
		
		int numPersons = (s_dir.isDirectory())? s_dir.listDir(): 0;
		
		for(int j = 0; j < numPersons; j++){
			
			vector<ofImage * > t_person;
			
			ofDirectory ss_dir;
			ss_dir.open(s_dir.getPath(j));
			
			int index = 99;
			if(s_dir.getName(j) == "simon")index = SIMON;
			if(s_dir.getName(j) == "luke")index = LUKE;
			if(s_dir.getName(j) == "cimeon")index = CIMEON;
			
			int numFiles = (ss_dir.isDirectory()) ?  ss_dir.listDir() : 0;
			
			if(index != 99 && numFiles > 0){
				
				for(int k = 0; k < numFiles; k ++){
					
					ofImage * temp = new ofImage(ss_dir.getPath(k));
					t_person.push_back(temp);
					
				}
				
				
				t_section[index] = t_person;
			}
			
		}
		
		images.push_back(t_section);
	}
	
	c_perf = SIMON;
	currentSection = -1;
	for(int i = 0; i < 21; i++)gridIndexList.push_back(i);
	
	nextSection();

	
	

}

void testApp::loadSectionSettings(){
	
	ofxXmlSettings XML;
	
	if( XML.loadFile("settings.xml") ){
		
		if(XML.pushTag("SET_FILLER",0)){
			
			numSections = XML.getNumTags("SECTION");
			
			for(int i = 0; i < numSections; i++){
				
				if(XML.pushTag("SECTION", i)){
					
					sectionTitles.push_back(XML.getValue("TITLE",""));
					sectionSequence.push_back(XML.getValue("SEQUENCE", "RAND"));
					triggerPerformer.push_back(getPerformer(XML.getValue("TRIGGER", "")));
		
					vector<slide> tempSlides;
					
					if(XML.pushTag("SIMON", 0)){
						
						slide s;
						loadSlideParameters(s, XML);
						tempSlides.push_back(s);
						XML.popTag();
					}
					
					if(XML.pushTag("LUKE", 0)){
						
						slide s;
						loadSlideParameters(s, XML);
						tempSlides.push_back(s);
						XML.popTag();
					}
					
					if(XML.pushTag("CIMEON", 0)){
						
						slide s;
						loadSlideParameters(s, XML);
						tempSlides.push_back(s);
						XML.popTag();
					}
					
					
					sectionSlides.push_back(tempSlides);
					
					XML.popTag();
				}
			}
			
			XML.popTag();
		}
		
	}
	
	
	
}

void testApp::loadSlideParameters(slide &s, ofxXmlSettings XML){
	
	s.drawType = XML.getValue("DRAW", "STANDARD");
	s.splitPosition = XML.getValue("POS", "CENTER");
	s.max_alpha = XML.getValue("MAX_ALPHA", 255.0f);
	s.attackTime = XML.getValue("ATT_SECS", 0.2f);
	s.releaseTime = XML.getValue("FADE_SECS", 0.0f);
	s.sustainTime = XML.getValue("HOLD_SECS", 0.195f); //defaults to a basic crossfade
	s.isSelfRelease = XML.getValue("AUTO_FADE", 0);
	s.clearType = XML.getValue("CLEAR_TYPE", "ALL");


}

//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(0);	
	
	for(int i = 0; i < currentSlides.size(); i ++)currentSlides[i].update();
	
	if(currentSlides.size() > 0){
		vector<slide>::iterator it = remove_if(currentSlides.begin(), currentSlides.end(), isSlideDone);
		currentSlides.erase(it, currentSlides.end());
	}
	
	if(newSectionCount > 0)newSectionCount -= 1;
}

//--------------------------------------------------------------
void testApp::draw(){	

	ofSetColor(255);
	
	if(screenMode == 0){
		
		ofEnableAlphaBlending();
		
		for(int i = 0; i < currentSlides.size(); i++){
			
			ofSetColor(255, 255, 255,currentSlides[i].alpha);
			
			if(currentSlides[i].drawType == "STANDARD"){
				
				if(currentSlides[i].image)currentSlides[i].image->draw(ofGetScreenWidth()/2,ofGetScreenHeight()/2,1200,800);
				
			}else if(currentSlides[i].drawType == "SPLIT"){
				
				ofVec2f pos(0,0);
				if(currentSlides[i].splitPosition == "LEFT")pos.set(ofGetScreenWidth()/6,ofGetScreenHeight()/2);
				if(currentSlides[i].splitPosition == "CENTER")pos.set(ofGetScreenWidth()/2,ofGetScreenHeight()/2);
				if(currentSlides[i].splitPosition == "RIGHT")pos.set(ofGetScreenWidth() * 5/6,ofGetScreenHeight()/2);
				if(currentSlides[i].image)currentSlides[i].image->draw(pos.x, pos.y, 400, ofGetScreenHeight());
				
			}else if(currentSlides[i].drawType == "GRID"){
				
				ofVec2f pos(ofGetScreenWidth()/8 + (ofGetScreenWidth()/8 * (currentSlides[i].gridPosition%7)) ,
						ofGetScreenHeight()/6 + (ofGetScreenHeight()/3 * floor(currentSlides[i].gridPosition/7)));
				
				
				if(currentSlides[i].image)currentSlides[i].image->draw(pos.x, pos.y, 125, 250);
			}
		}
		
		ofDisableAlphaBlending();
		
	}else{
		
		ofDrawBitmapString(getPerformerString(c_perf), ofVec2f(ofGetWidth()/2, ofGetHeight()/2));
		ofDrawBitmapString(ofToString(deviceNum,0), ofVec2f(ofGetWidth()/2, ofGetHeight()/2 + 30));
		
		for(int i = 0; i < 3; i++){
			ofDrawBitmapString(getPerformerString(e_performer(i)) + ":", ofVec2f(10, 30 + 30 * i));
			ofDrawBitmapString(ofToString(assigns[i],0), ofVec2f(250, 30 + 30 * i));
			
		}
		
		ofDrawBitmapString("currentSection: " + ofToString(currentSection), 10,200);
	}

	
}




string testApp::getPerformerString(e_performer t){
	
	string p_string = "";
	
	switch (t) {
		case SIMON: p_string = "Simon"; break;
		case LUKE: p_string = "Luke"; break;
		case CIMEON: p_string = "Cimeon"; break;
	}
	
	return p_string;
}

int testApp::getPerformer(string s){
	
	if(s == "SIMON"){
		return SIMON;
	}
	if(s == "LUKE"){
		return LUKE;
	}
	if(s == "CIMEON"){
		return CIMEON;
	}
	
	return MISC;

}

void testApp::orderImages(int t_perf){
	
	if(sectionSequence[currentSection] == "RAND"){
		random_shuffle ( images[currentSection][t_perf].begin(), images[currentSection][t_perf].end() );
	}else if (sectionSequence[currentSection] == "FWDS_BKWDS"  ){
		if(sequenceIndex[t_perf] > 0)reverse( images[currentSection][t_perf].begin(), images[currentSection][t_perf].end() );
	}
	
	sequenceIndex[t_perf] = 0;
	
}

void testApp::nextSection(){

	currentSection = (currentSection + 1)%images.size();
	for(int j = 0; j < 3; j++){
		sequenceIndex[j] = 0;
		orderImages(j);
		if(j != c_perf)sequenceIndex[j] = -1; //override for start of section so that increments to first image
	}
	
	for(int i = 0; i < currentSlides.size(); i ++){
		currentSlides[i].isFading = true;
		currentSlides[i].sustainTime = 0;
	} 
	
	random_shuffle(gridIndexList.begin(), gridIndexList.end());
	gridCount = 0;
	newSectionCount = 10;
	oneShotCount = 0;
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	if(screenMode == 1){
		if(key == 'q')assigns[0] += 1;
		if(key == 'w')assigns[0] -= 1;
		if(key == 'a')assigns[1] += 1;
		if(key == 's')assigns[1] -= 1;
		if(key == 'z')assigns[2] += 1;
		if(key == 'x')assigns[2] -= 1;
		
		if(key == OF_KEY_RETURN){
			currentSection = - 1;
			nextSection();
		}
	}
	
	if(key == ' ')screenMode = (screenMode + 1)%2;
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	
}


void testApp::mousePressed(int device, int button){
	
	if(newSectionCount > 0)return;
	
	deviceNum = device; //for display purposes
	
	for(int i = 0; i < 3; i ++){
		
		if(device == assigns[i]){
			c_perf = e_performer(i);
			
			
			if(button == 0){
				
				if(sectionSlides[currentSection][i].drawType != "NIL")sequenceIndex[i] += 1;
				
				if(sectionSequence[currentSection] == "ONE_SHOT"){
					
					nextSection();
					
				}else if(sectionSlides[currentSection][i].drawType == "NIL"){
					
					return;
					
				}else if(sequenceIndex[i] == images[currentSection][i].size()){
					
					if(sectionSequence[currentSection] == "FIXED" && 
					   (c_perf == triggerPerformer[currentSection] || triggerPerformer[currentSection] == MISC)
					){
						nextSection();
					}else{
						orderImages(i);
					}
				}
					
			}else if(c_perf == SIMON){
				nextSection();
			}
			
			//make the slide here
			
			slide t = sectionSlides[currentSection][i];
			if(t.drawType != "NIL")t.image = images[currentSection][i][sequenceIndex[i]];
			t.alpha = (t.attackTime > 0)? 0:t.max_alpha;
			
			if(t.drawType == "GRID"){
				t.gridPosition = gridIndexList[gridCount];
				gridCount = (gridCount + 1)%gridIndexList.size();
			}
			
			if(t.clearType == "ALL"){
				for(int i = 0; i < currentSlides.size(); i ++){currentSlides[i].isFading = true;} //cancel out other slides
			}else if(t.clearType == "S_POS"){
				for(int i = 0; i < currentSlides.size(); i ++){
					if(currentSlides[i].splitPosition == t.splitPosition)currentSlides[i].isFading = true;
					if(currentSlides[i].drawType != t.drawType)currentSlides[i].isFading = true;
				}
			}else if(t.clearType == "HARD_ALL"){
				 for(int i = 0; i < currentSlides.size(); i ++){
					 currentSlides[i].isFading = true;
					 currentSlides[i].sustainTime = 0;
				 } 
			}else if(t.clearType == "G_POS"){
				for(int i = 0; i < currentSlides.size(); i ++){
					if(currentSlides[i].gridPosition == t.gridPosition)currentSlides[i].isFading = true;
					if(currentSlides[i].drawType != t.drawType)currentSlides[i].isFading = true;
				}
			}


			
			currentSlides.push_back(t);
			break;
			
		}	
	}
	
	
}

void testApp::mouseReleased(int device, int button){

	
}


//--------------------------------------------------------------

void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------

void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------

void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------

void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}

void testApp::exit(){
	
	for(int i = 0; i < images.size(); i++){
		for(int j = 0; j < images[i].size(); j ++){
			for(int k =0; k < images[i][j].size(); k++){
				
				delete images[i][j][k];
			}
		}
	}
	
	
}
