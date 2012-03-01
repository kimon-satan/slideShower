/*
 *  slide.cpp
 *  slideShower
 *
 *  Created by Simon Katan on 29/02/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "slide.h"

slide::slide(){
	
	isFading = false;
	isSelfRelease = false;
	clearType = "ALL";
	max_alpha = max_alpha;
}

void slide::update(){

	if(alpha < max_alpha && !isFading){
		
		alpha += max_alpha/(attackTime * ofGetFrameRate());
		if(isSelfRelease && alpha >= max_alpha)isFading = true;
		
	}else if(sustainTime > 0 && isFading){
		
		sustainTime -= 1.0f/ofGetFrameRate();
		
	}else if(alpha > 0 && isFading){
		
		alpha -= max_alpha/(releaseTime * ofGetFrameRate());
		
	}
		
	
}