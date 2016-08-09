#ifndef HF_OBJECT
#define HF_OBJECT

#include "ofMain.h"

class Object {
	
public:

	Object(ofImage *_image, float _x, float _y, int _ttl, int _rotate, float _scale, int _colorize);

	void draw();

	float x; 
	float y;
	int ttl; // ������� �������� ������ ����
	int maxttl; // ������� ������ ����� �����
	int col_r; //����
	int col_g;
	int col_b;
	float a; //������������
	float r; //���� ��������
	float scale; //�������

	ofImage *image;

	int *ptr_limit;

};

#endif