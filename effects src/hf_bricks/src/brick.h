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

	float x; // ������� �������������� 
	float y; // ������� ��������������
	float destX; // ������� ��������������
	float destY; // ������� ��������������
	float origX; // ��������� ��������������
	float origY; // ��������� ��������������
	float brickwidth; // ������ ������
	float brickheight; // ������ ������
	float sourcex; // ����� � �������� ������ ���������� ����� x
	float sourcey; // ����� � �������� ������ ���������� ����� y
	float r; // ���� ��������
	int return_wait; // ��������
	bool canmove; // ����� �� move
	int rotate; // ����� �� ��������� ������
	int moveout; //����� �� �������� �� �����
	int donotmoveframes; // ����� ������� ������ ����� move
	int movespeed; // �������� �����������
	int backspeed; //�������� �����������
	int shadow_enabled; // ���� �� ����
	bool at_origin; // ��������� �� ������ �� ��������� �����

	ofImage *image;
};

#endif