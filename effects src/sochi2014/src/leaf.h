#ifndef HF_LEAF
#define HF_LEAF

#include "ofMain.h"
#include "ofxTweener.h"

class Leaf {
	
public:

	Leaf(ofImage *_image, double _scale, int _shadow_enabled, int _moveout, int _rotate);

	void move(int _donotmove, int _movespeed, int _return_wait, int _moveout);
	void move_back(int _donotmove, int _movespeed, int _return_wait);
	void draw();
	bool hittest(int _x, int _y); 

	float x; // Текущее местоположение 
	float y; // Текущее местоположение
	float destX; // Целевое местоположение
	float destY; // Целевое местоположение
	float origX; // Начальное местоположение
	float origY; // Начальное местоположение
	float scale; // Масштаб
	float r; // Угол поворота
	int return_wait; // Задержка
	bool canmove; // Можно ли move
	int donotmoveframes; // Через сколько кадров можно move
	int movespeed; // Скорость перемещения
	int shadow_enabled; // Есть ли тень
	int rotate; // Будут ли вращаться
		
	ofImage *image;

};

#endif