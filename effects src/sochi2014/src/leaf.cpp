#include "leaf.h"
#include "testApp.h"

Leaf::Leaf(ofImage *_image, double _scale, int _shadow_enabled, int _moveout, int _rotate){

	image = _image;
	
	shadow_enabled = _shadow_enabled;
		
	if (_moveout == 0){
		x = ofRandom(ofGetWindowWidth() - image->width);
		y = ofRandom(ofGetWindowHeight() - image->height);
	} else {
		x = ofRandomWidth() - image->width / 2;
		y = ofRandomHeight() - image->width / 2;
	}


	destX = x;
	destY = y;

	origX = x;
	origY = y;

	canmove = true;
	rotate = _rotate;

	if (rotate == 1){
		r = ofRandom( 360 );
	} else {
		r = 0;
	}
	//cout << "x = " << x << "  y = " << y << "  r = " << r << endl;
}

void Leaf::move(int _donotmove, int _movespeed, int _return_wait, int _moveout){

	if (_moveout == 0){
		destX = ofRandom(ofGetWindowWidth() - image->width);
		destY = ofRandom(ofGetWindowHeight() - image->height);
	} else {
		destX = ofRandom(ofGetWindowWidth() * 2) - ofGetWindowWidth() / 2;
		destY = ofRandom(ofGetWindowHeight() * 2) - ofGetWindowHeight() / 2;
	}

	donotmoveframes = _donotmove;
	
	return_wait = _return_wait;

	Tweener.addTween(x, destX, _movespeed);
	Tweener.addTween(y, destY, _movespeed);
	if (rotate == 1) {
		Tweener.addTween(r, ofRandom(1000), _movespeed);
	} 


}

void Leaf::move_back(int _donotmove, int _movespeed, int _return_wait){

	donotmoveframes = _donotmove;

	return_wait = _return_wait;

	Tweener.addTween(x, origX, _movespeed);
	Tweener.addTween(y, origY, _movespeed);
	if (rotate == 1){
		Tweener.addTween(r, ofRandom(1000), _movespeed);
	}
}

void Leaf::draw(){
	
	// Update
	if (donotmoveframes > 0){
		donotmoveframes--;
		canmove = false;
	} else {
		canmove = true;
	}

	if (return_wait > 0) {
		return_wait--;
	} 

	// Draw
	ofPushMatrix(); //1
		ofTranslate(x,y);
		ofPushMatrix(); //2
			ofTranslate(image->width/2, image->height/2, 0);//move pivot to centre
			ofRotate(r);//rotate from centre
			ofPushMatrix();
				ofTranslate(-image->width/2,-image->height/2,0);//move back by the centre offset
				
				if (shadow_enabled == 1){
				ofSetColor(128,64);
				image->draw(0,0);
				
				ofPushMatrix();
					ofRotate(r * -1);
					ofTranslate(-4,-4);
					ofPushMatrix();
						ofRotate(r);
						ofSetColor(255);											
						image->draw(0,0);
					ofPopMatrix();
				ofPopMatrix();
				} else {
					image->draw(0,0);
				}


			ofPopMatrix();
		ofPopMatrix();
	ofPopMatrix();

}

bool Leaf::hittest(int _x, int _y){
	if (_x > x && _y > y && _x < x + image->width && _y < y + image->height){
		return true;
	} else {
		return false;
	}
}