#ifndef HF_OBJECT
#define HF_OBJECT

#include "ofMain.h"

class Object {
	
public:

	Object(ofImage *_image, float _x, float _y, int _ttl, int _rotate, float _scale, int _colorize);

	void draw();

	float x; 
	float y;
	int ttl; // сколько осталось кадров жить
	int maxttl; // сколько кадров живет всего
	int col_r; //цвет
	int col_g;
	int col_b;
	float a; //прозрачность
	float r; //угол поворота
	float scale; //масштаб

	ofImage *image;

	int *ptr_limit;

};

#endif