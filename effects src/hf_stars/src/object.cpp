#include "object.h"


Object::Object(ofImage *_image, float _x, float _y, int _ttl, int _rotate, float _scale, int _colorize){

	x = _x;
	y = _y;
	ttl = _ttl;
	maxttl = _ttl;
	image = _image;

	if (_colorize == 1){
		col_r = ofRandom(255);
		col_g = ofRandom(255);
		col_b = ofRandom(255);
	} else {
		col_r = 255;
		col_g = 255;
		col_b = 255;
	}

	if (_rotate == 1){
		r = ofRandom(360);
	} else {
		r = 0;
	}
	scale = _scale;
	a = 0;

}


void Object::draw(){
	
	
	// Draw
	ofPushMatrix();
		
		ofPushMatrix(); //1
		//
		ofTranslate(x-(image->width/2)*scale,y-(image->height/2)*scale);
			ofPushMatrix(); //2
			ofScale(scale,scale,scale);
			ofTranslate(image->width/2, image->height/2, 0);//move pivot to centre
				ofRotate(r);//rotate from centre
				ofPushMatrix();
				ofTranslate(-image->width/2,-image->height/2,0);//move back by the centre offset
					
					ofSetColor(col_r,col_g,col_b,a);
					//image->draw(x-image->width/2,y-image->height/2);
					image->draw(0,0);

				ofPopMatrix();
			ofPopMatrix();
		ofPopMatrix();
	ofPushMatrix();
}