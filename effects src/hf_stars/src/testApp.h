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

		//Преобразование координат мыши в координаты сетки ограничений количества объектов
		int mouse_to_grid_x(int mx); 
		int mouse_to_grid_y(int my);

		ofxXmlSettings XML;

		int ttl; //Время жизни объекта
		int rotate; //Повернут ли на произвольный угол при создании
		float rotateall; //Угол приращения вращения
		float scale; // Начальный масштаб
		float endscale; // Конечный масштаб

		int limit_width; // Ширина сетки ограничений
		int limit_height; // Высота сетки ограничений
		int limit; // Ограничение кол-ва объектов на одну ячейку
		int limit_grid[4000][3000]; // Сетка ограничений
		
		int colorize; // Раскрашивать и в разные произвольные цвета

		ofBlendMode blendmode; // Режим смешения

		ofImage hf_background; // Фон

		vector <ofImage> images;
		vector <Object> objects;


};
