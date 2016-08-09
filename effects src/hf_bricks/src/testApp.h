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

		int bricksw; //Сколько кубиков по горизонтали
		int bricksh; //Сколько кубиков по вертикали
		static int donotmove; // Сколько кадров после move не воспринимать move
		int movespeed; // Скорость движения
		int backspeed; // Скорость возврата
		static int return_wait; // Через сколько возвращаться назад после move
		int moveout; // Будут ли кубики вылетать за пределы или оставаться в пределах экрана
		int rotate; //Вращать ли кубики при move
		int shadow_enabled; // Есть ли тень

		stringstream ss;
		
};
