#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "object.h"
#include "HitekSocket.h"

using namespace std;

class testApp : public HitekSocketListener,public  ofBaseApp {

	public:
		void receiveAction();
		HitekSocket* hitekSocket;
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

		//�������������� ��������� ���� � ���������� ����� ����������� ���������� ��������
		int mouse_to_grid_x(int mx); 
		int mouse_to_grid_y(int my);

		ofxXmlSettings XML;

		int ttl; //����� ����� �������
		int rotate; //�������� �� �� ������������ ���� ��� ��������
		float rotateall; //���� ���������� ��������
		float scale; // ��������� �������
		float endscale; // �������� �������

		int limit_width; // ������ ����� �����������
		int limit_height; // ������ ����� �����������
		int limit; // ����������� ���-�� �������� �� ���� ������
		int limit_grid[4000][3000]; // ����� �����������
		
		int colorize; // ������������ � � ������ ������������ �����

		ofBlendMode blendmode; // ����� ��������

		ofImage hf_background; // ���

		vector <ofImage> images;
		vector <Object> objects;


};
