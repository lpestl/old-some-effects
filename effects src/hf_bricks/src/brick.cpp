#include "brick.h"
#include "testApp.h"

Brick::Brick(ofImage *source_image, float xt, float yt, float w, float h, int _shadow_enabled, int _donotmove, int _return_wait, int _moveout, int _rotate, int _movespeed, int _backspeed){

	x = xt;
	y = yt;

	destX = x;
	destY = y;

	origX = x;
	origY = y;

	sourcex = x;
	sourcey = y;

	brickwidth = w;
	brickheight = h;

	canmove = true;
	movespeed = _movespeed;
	backspeed = _backspeed;
	moveout = _moveout;
	rotate = _rotate;
	donotmoveframes = _donotmove;
	return_wait = _return_wait;

	at_origin = true;

	r = 0;

	image = source_image;

	shadow_enabled = _shadow_enabled;

	//r = ofRandom( 360 );
	////cout << "x = " << x << "  y = " << y << "  r = " << r << endl;
}

void Brick::move(int _return_wait, int _donotmove){
	return_wait = _return_wait;
	donotmoveframes = _donotmove;

	if (moveout == 0){
		destX = ofRandom(ofGetWindowWidth() - brickwidth);
		destY = ofRandom(ofGetWindowHeight() - brickheight);
	} else {
		destX = ofRandom(ofGetWindowWidth() * 2) - ofGetWindowWidth() / 2;
		destY = ofRandom(ofGetWindowHeight() * 2) - ofGetWindowHeight() / 2;
	}


	Tweener.addTween(x, destX, movespeed);
	Tweener.addTween(y, destY, movespeed);

	if (rotate == 1){
		Tweener.addTween(r, ofRandom(1000), movespeed);
	}

}

void Brick::move_back(int _return_wait, int _donotmove){
	return_wait = _return_wait;
	donotmoveframes = _donotmove;


	Tweener.addTween(x, origX, backspeed, &ofxTransitions::easeInSine);
	Tweener.addTween(y, origY, backspeed, &ofxTransitions::easeInSine);
	if (rotate == 1){
		Tweener.addTween(r, 0, backspeed, &ofxTransitions::easeInSine);
	}
}

void Brick::draw(){
	
	// Update
	if (donotmoveframes > 0){
		donotmoveframes--;
		canmove = false;
	} else {
		canmove = true;
	}

	if (x == origX && y == origY){
		at_origin = true;
	} else {
		at_origin = false;
	}


	if (return_wait > 0) {
		return_wait--;
	} 

	// Draw
	ofPushMatrix(); //1
		ofTranslate(x,y);
		ofPushMatrix(); //2
			ofTranslate(brickwidth/2, brickheight/2, 0);//move pivot to centre
			ofRotate(r);//rotate from centre
			ofPushMatrix();
				ofTranslate(-brickwidth/2,-brickheight/2,0);//move back by the centre offset
				

				
				if (shadow_enabled == 1){
				ofSetColor(128,48);
				image->drawSubsection(0,0,brickwidth,brickheight,sourcex,sourcey);
				
				ofPushMatrix();
					ofRotate(r * -1);
					ofTranslate(-3,-3);
					ofPushMatrix();
						ofRotate(r);
						ofSetColor(255);											
						image->drawSubsection(0,0,brickwidth,brickheight,sourcex,sourcey);
					ofPopMatrix();
				ofPopMatrix();
				} else {
					image->drawSubsection(0,0,brickwidth,brickheight,sourcex,sourcey);
				}




				//image->drawSubsection(0,0,brickwidth,brickheight,sourcex,sourcey);




			ofPopMatrix();
		ofPopMatrix();
	ofPopMatrix();

}

bool Brick::hittest(int _x, int _y){
	if (_x > x && _y > y && _x < x + brickwidth && _y < y + brickwidth){
		return true;
	} else {
		return false;
	}
}