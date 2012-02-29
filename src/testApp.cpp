#include "testApp.h"


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
	
	dir.open("images");
	
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
	nextSection();
	for(int i = 0; i < 3; i++)isMouseDown[i] = false;

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
					
					vector<string> temp_dts;
					vector<string> temp_pos;
					
					vector<float> temp_attacks;
					vector<float> temp_holds;
					vector<float> temp_fades;
					
					
					
					if(XML.pushTag("SIMON", 0)){
						temp_dts.push_back(XML.getValue("DRAW", "STANDARD"));
						temp_pos.push_back(XML.getValue("POS", "CENTER"));
						temp_attacks.push_back(XML.getValue("ATT_SECS",0.0f));
						temp_fades.push_back(XML.getValue("FADE_SECS", 1.0f));
						temp_holds.push_back(XML.getValue("HOLD_SECS", 0.0f));
						
						XML.popTag();
					}
					
					if(XML.pushTag("LUKE", 0)){
						temp_dts.push_back(XML.getValue("DRAW", "STANDARD"));
						temp_pos.push_back(XML.getValue("POS", "CENTER"));
						temp_attacks.push_back(XML.getValue("ATT_SECS",0.0f));
						temp_fades.push_back(XML.getValue("FADE_SECS", 1.0f));
						temp_holds.push_back(XML.getValue("HOLD_SECS", 0.0f));
						XML.popTag();
					}
					
					if(XML.pushTag("CIMEON", 0)){
						temp_dts.push_back(XML.getValue("DRAW", "STANDARD"));
						temp_pos.push_back(XML.getValue("POS", "CENTER"));
						temp_attacks.push_back(XML.getValue("ATT_SECS",0.0f));
						temp_fades.push_back(XML.getValue("FADE_SECS", 1.0f));
						temp_holds.push_back(XML.getValue("HOLD_SECS", 0.0f));
						XML.popTag();
					}
					
					sectionDrawTypes.push_back(temp_dts);
					sectionPositions.push_back(temp_pos);
					sectionAttackTimes.push_back(temp_attacks);
					sectionFadeTimes.push_back(temp_fades);
					sectionHoldTimes.push_back(temp_holds);
					
					XML.popTag();
				}
			}
			
			XML.popTag();
		}
		
	}
	
	
	
}



//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(0);	
	
	for(int i = 0; i < 3; i++){
		
		if(sectionDrawTypes[currentSection][i] == "SPLIT"){  
			
			if(c_perf == e_performer(i))perfAlphas[i] = 255;
			
		}else if(sectionDrawTypes[currentSection][i] == "STANDARD"){
			
			if(c_perf == e_performer(i)){
				perfAlphas[i] = 255;
				//cancel out other images
				for(int j = 1; j < 3; j++)perfAlphas[(i + j)%3] = 0; 
			}else{
				perfAlphas[i] = 0;
			}
			
		}else if(sectionDrawTypes[currentSection][i] == "FADE"){ // asr
			
			if(c_perf == e_performer(i) && isMouseDown[i]){
				
				perfAlphas[i] = (sectionAttackTimes[currentSection][i] > 0) ? 0 : 255;
				preAlphas[i] = sectionHoldTimes[currentSection][i]; 
				
				//cancel out other images
				for(int j = 1; j < 3; j++)perfAlphas[(i + j)%3] = 0; 
				
			}else if(perfAlphas[i] < 255 && preAlphas[i] > 0){
			
				perfAlphas[i] += 255.0f * (1.0f/sectionAttackTimes[currentSection][i]) * (1.0f/ofGetFrameRate());
				
			}else if(perfAlphas[i] >= 255 && preAlphas[i] > 0){
			
				preAlphas[i] -= 1.0f/ofGetFrameRate();
				
			}else if(perfAlphas[i] > 0){
				
				perfAlphas[i] -= 255.0f * (1.0f/sectionFadeTimes[currentSection][i]) * (1.0f/ofGetFrameRate());
				
			}
		
		}else if(sectionDrawTypes[currentSection][i] == "FADE"){ // ar
			
			if(c_perf == e_performer(i) && isMouseDown[i]){
				
				perfAlphas[i] = (sectionAttackTimes[currentSection][i] > 0) ? 0 : 255;
				preAlphas[i] = sectionHoldTimes[currentSection][i]; 
				
				//cancel out other images
				for(int j = 1; j < 3; j++)perfAlphas[(i + j)%3] = 0; 
				
			}else if(perfAlphas[i] < 255 && preAlphas[i] > 0){
				
				perfAlphas[i] += 255.0f * (1.0f/sectionAttackTimes[currentSection][i]) * (1.0f/ofGetFrameRate());
				
			}else if(perfAlphas[i] >= 255 && preAlphas[i] > 0){
				
				preAlphas[i] -= 1.0f/ofGetFrameRate();
				
			}else if(perfAlphas[i] > 0){
				
				perfAlphas[i] -= 255.0f * (1.0f/sectionFadeTimes[currentSection][i]) * (1.0f/ofGetFrameRate());
				
			}
			
		}
		
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){	

	
	ofSetColor(255);
	
	if(screenMode == 0){
		
		ofEnableAlphaBlending();
		
		for(int i = 0; i < 3; i++){
			if(sectionDrawTypes[currentSection][i] == "STANDARD"){
				standardDraw(e_performer(i));
			}else if(sectionDrawTypes[currentSection][i] == "FADE"){
				standardDraw(e_performer(i));
			}else if(sectionDrawTypes[currentSection][i] == "SPLIT"){
				splitDraw(e_performer(i));
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


void testApp::standardDraw(e_performer t_perf){
	
	if(perfAlphas[t_perf] > 0){
		if(images[currentSection][t_perf].size() > 0){
			ofSetColor(255, 255, 255,perfAlphas[t_perf]);
			images[currentSection][t_perf][currentImage[t_perf]]->draw(ofGetScreenWidth()/2,ofGetScreenHeight()/2,1200,800);
		}
	}
	
	ofSetColor(255);
}

void testApp::splitDraw(e_performer t_perf){
	
	if(perfAlphas[t_perf] > 0){
		if(images[currentSection][t_perf].size() > 0){
			
			ofVec2f pos(0,0);
			if(sectionPositions[currentSection][t_perf] == "LEFT")pos.set(ofGetScreenWidth()/6,ofGetScreenHeight()/2);
			if(sectionPositions[currentSection][t_perf] == "CENTER")pos.set(ofGetScreenWidth()/2,ofGetScreenHeight()/2);
			if(sectionPositions[currentSection][t_perf] == "RIGHT")pos.set(ofGetScreenWidth() * 5/6,ofGetScreenHeight()/2);
			images[currentSection][t_perf][currentImage[t_perf]]->draw(pos.x, pos.y, 400, ofGetScreenHeight());
		}
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
		cout << SIMON << endl;
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
		if(currentImage[t_perf] > 0)reverse( images[currentSection][t_perf].begin(), images[currentSection][t_perf].end() );
	}
	
	currentImage[t_perf] = 0;
	
}

void testApp::nextSection(){

	currentSection = (currentSection + 1)%images.size();
	for(int j = 0; j < 3; j++){
		perfAlphas[j] = 0;
		preAlphas[j] = 0;
		currentImage[j] = 0;
		orderImages(j);
		if(j != c_perf)currentImage[j] = -1; //override for start of section so that increments to first image
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	if(key == 'q')assigns[0] += 1;
	if(key == 'w')assigns[0] -= 1;
	if(key == 'a')assigns[1] += 1;
	if(key == 's')assigns[1] -= 1;
	if(key == 'z')assigns[2] += 1;
	if(key == 'x')assigns[2] -= 1;
	
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
	
	deviceNum = device; //for display purposes
	
	for(int i = 0; i < 3; i ++){
		
		if(device == assigns[i]){
			c_perf = e_performer(i);
			isMouseDown[i] = true;
			
			if(button == 0){
				
				currentImage[i] += 1;
				
				
				
				if(currentImage[i] == images[currentSection][i].size()){
					
					if(sectionSequence[currentSection] == "FIXED" && c_perf == triggerPerformer[currentSection]){
						nextSection();
					}else{
						orderImages(i);
					}
				}
					
			}else{
				nextSection();
			}
			break;
		}	
	}
	
	
}

void testApp::mouseReleased(int device, int button){

	for(int i = 0; i < 3; i ++){
		
		if(device == assigns[i]){
			isMouseDown[i] = false;
		}
	}

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
