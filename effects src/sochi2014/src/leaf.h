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

	float x; // ������� �������������� 
	float y; // ������� ��������������
	float destX; // ������� ��������������
	float destY; // ������� ��������������
	float origX; // ��������� ��������������
	float origY; // ��������� ��������������
	float scale; // �������
	float r; // ���� ��������
	int return_wait; // ��������
	bool canmove; // ����� �� move
	int donotmoveframes; // ����� ������� ������ ����� move
	int movespeed; // �������� �����������
	int shadow_enabled; // ���� �� ����
	int rotate; // ����� �� ���������
		
	ofImage *image;

};

#endif