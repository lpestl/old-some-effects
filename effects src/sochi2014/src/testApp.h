#pragma once

#include "ofMain.h"
#include "leaf.h"
#include "ofxXmlSettings.h"
#include "HitekSocket.h"

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
		
		ofImage hf_background; //������� �����������
		static vector <Leaf> theLeaves; 
		vector <ofImage> images;
		int leaves_count; // ������� �������
		int donotmove; // ������� ������ ����� move �� ������������ move

		static int s_donotmove;
		static int s_movespeed; 
		static int s_return_wait;
		static int s_moveout;

		int movespeed; // �������� ��������
		int backspeed; // �������� �����������
		int return_wait; // ����� ������� ������������ ����� ����� move
		int moveout; // ����� �� ������ �������� �� ������� ��� ���������� � �������� ������
		double scale; // ������� �������
		int shadow_enabled; // ���� �� ����
		int rotate; // ������� �� �������

};
