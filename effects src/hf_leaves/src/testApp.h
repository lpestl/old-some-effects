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
		
		ofImage hf_background; //Фоновое изображение
		static vector <Leaf> theLeaves; 
		vector <ofImage> images;
		int leaves_count; // Сколько листьев
		int donotmove; // Сколько кадров после move не воспринимать move

		static int s_donotmove;
		static int s_movespeed; 
		static int s_return_wait;
		static int s_moveout;

		int movespeed; // Скорость движения
		int backspeed; // Скорость возвращения
		int return_wait; // Через сколько возвращаться назад после move
		int moveout; // Будут ли листья вылетать за пределы или оставаться в пределах экрана
		double scale; // Масштаб листьев
		int shadow_enabled; // Есть ли тень
		int rotate; // Вращать ли объекты

};
