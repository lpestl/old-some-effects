#pragma once

#include "ofMain.h"
#include "brick.h"
#include "ofxXmlSettings.h"
#include "HitekSocket.h"
#include <iostream>

using namespace std;

class testApp : public ofBaseApp{

	public:
		static void pushSocketData();
		static HitekSocket* hitekSocket;
		int inputMode;

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxXmlSettings XML;
		
		ofImage hf_background;
		ofImage hf_foreground;

		static vector <Brick> theBricks;

		int bricksw; //������� ������� �� �����������
		int bricksh; //������� ������� �� ���������
		static int donotmove; // ������� ������ ����� move �� ������������ move
		int movespeed; // �������� ��������
		int backspeed; // �������� ��������
		static int return_wait; // ����� ������� ������������ ����� ����� move
		int moveout; // ����� �� ������ �������� �� ������� ��� ���������� � �������� ������
		int rotate; //������� �� ������ ��� move
		int shadow_enabled; // ���� �� ����

		stringstream ss;
		
};
