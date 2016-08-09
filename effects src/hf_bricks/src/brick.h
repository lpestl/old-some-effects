#ifndef HF_BRICK
#define HF_BRICK

#include "ofMain.h"
#include "ofxTweener.h"

class Brick {
	
public:

	Brick(ofImage *source_mage, float xt, float yt, float w, float h, int _shadow_enabled, int _donotmove, int _return_wait, int _moveout, int _rotate, int _movespeed, int _backspeed);

	void move(int _return_wait, int _donotmove);
	void move_back(int _return_wait, int _donotmove);
	void draw();
	bool hittest(int _x, int _y); 

	float x; // Текущее местоположение 
	float y; // Текущее местоположение
	float destX; // Целевое местоположение
	float destY; // Целевое местоположение
	float origX; // Начальное местоположение
	float origY; // Начальное местоположение
	float brickwidth; // Ширина кубика
	float brickheight; // Высота кубика
	float sourcex; // Место с картинки откуда начинается кубик x
	float sourcey; // Место с картинки откуда начинается кубик y
	float r; // Угол поворота
	int return_wait; // Задержка
	bool canmove; // Можно ли move
	int rotate; // Будут ли вращаться кубики
	int moveout; //Будут ли выходить за экран
	int donotmoveframes; // Через сколько кадров можно move
	int movespeed; // Скорость перемещения
	int backspeed; //Скорость возвращения
	int shadow_enabled; // Есть ли тень
	bool at_origin; // Находится ли объект на начальном месте

	ofImage *image;
};

#endif