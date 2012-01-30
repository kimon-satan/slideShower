#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	for(int i = 0; i < 3; i++){
		currentImage[i] = 0;
		perfAlphas[i] = 0;
	}
	
	c_perf = MISC;
	screenMode = 0;
	currentSection = 0;
	
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
				
				random_shuffle ( t_person.begin(), t_person.end() );
				
				t_section[index] = t_person;
			}
			
		}
		
		images.push_back(t_section);
	}


}

void testApp::loadSectionSettings(){
	
	ofxXmlSettings XML;
	
	if( XML.loadFile("settings.xml") ){
		
		
		if(XML.pushTag("SET_FILLER",0)){
		
			numSections = XML.getNumTags("SECTION");
			
			for(int i = 0; i < numSections; i++){
			
				if(XML.pushTag("SECTION", i)){
					
					sectionTitles.push_back(XML.getValue("TITLE",""));
					
					vector<string> temp_dts;
					vector<string> temp_pos;
					
					if(XML.pushTag("SIMON", 0)){
						temp_dts.push_back(XML.getValue("DRAW", "STANDARD"));
						temp_pos.push_back(XML.getValue("POS", "CENTER"));
						XML.popTag();
					}
					
					if(XML.pushTag("LUKE", 0)){
						temp_dts.push_back(XML.getValue("DRAW", "STANDARD"));
						temp_pos.push_back(XML.getValue("POS", "CENTER"));
						XML.popTag();
					}
					
					if(XML.pushTag("CIMEON", 0)){
						temp_dts.push_back(XML.getValue("DRAW", "STANDARD"));
						temp_pos.push_back(XML.getValue("POS", "CENTER"));
						XML.popTag();
					}
					
					sectionDrawTypes.push_back(temp_dts);
					sectionPositions.push_back(temp_pos);
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
		 }
	}
	
	// overrides
	
	for(int i = 0; i < 3; i++){
		
		if(sectionDrawTypes[currentSection][i] == "STANDARD"){
			if(c_perf == e_performer(i)){
				perfAlphas[i] = 255;
				for(int j = 1; j < 3; j++)perfAlphas[(i + j)%3] = 0;
			}else{
				perfAlphas[i] = 0;
			}
		}
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){	
	
	ofSetColor(255);
	
	if(screenMode == 0){
		
		for(int i = 0; i < 3; i++){
			if(sectionDrawTypes[currentSection][i] == "STANDARD"){
				standardDraw(e_performer(i));
			}else if(sectionDrawTypes[currentSection][i] == "SPLIT"){
				splitDraw(e_performer(i));
			}
		}
		
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
			images[currentSection][t_perf][currentImage[t_perf]]->draw(0,0);
		}
	}
			
}

void testApp::splitDraw(e_performer t_perf){

	if(perfAlphas[t_perf] > 0){
		if(images[currentSection][t_perf].size() > 0){
			
			ofVec2f pos(0,0);
			if(sectionPositions[currentSection][t_perf] == "LEFT")pos.set(0,0);
			if(sectionPositions[currentSection][t_perf] == "CENTER")pos.set(ofGetScreenWidth()/3,0);
			if(sectionPositions[currentSection][t_perf] == "RIGHT")pos.set(ofGetScreenWidth() * 2/3,0);
			images[currentSection][t_perf][currentImage[t_perf]]->draw(pos.x, pos.y, ofGetScreenWidth()/3, ofGetScreenHeight());
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
	
	deviceNum = device;
	
	for(int i = 0; i < 3; i ++){
		if(device == assigns[i]){
			c_perf = e_performer(i);
			if(button == 0){
				currentImage[i] += 1;
				if(currentImage[i] == images[currentSection][i].size()){
					random_shuffle ( images[currentSection][i].begin(), images[currentSection][i].end() );
					currentImage[i] = 0;
				}
			}else{
				for(int j = 0; j < 3; j++)currentImage[j] = 0;
				currentSection = (currentSection + 1)%images.size();
			}
			break;
		}	
	}
	
	
}

void testApp::mouseReleased(int device, int button){}


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
