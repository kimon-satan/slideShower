/*
 *  slide.h
 *  slideShower
 *
 *  Created by Simon Katan on 29/02/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef slide_h
#define slide_h

#include "ofMain.h"

class slide{

public:
	
	slide();
	void update();
	
	ofImage * image;
	bool isFading, isSelfRelease;
	string clearType;
	string drawType;
	float alpha, attackTime, sustainTime, releaseTime;
	float max_alpha;
	string splitPosition;
	int gridPosition;
	int gridCount;


};

#endif
