#pragma once

//#include "ofMain.h"
//#include"windows.h" 
#include "ofxXmlSettings.h"
#include "HitekSocket.h"

using namespace std;


union Coordinates{
	char stringCoor[12];
	struct{
		int x,y,ttl;
	};
} ;

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		//static DWORD WINAPI ThreadFunction(LPVOID param) ;
		static void pushSocketData();
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		static HitekSocket* hitekSocket;
		ofxXmlSettings XML;

		static int maxParticles;
		static int particlesTTL;
		int dotSize;
		int inputMode;

		/*HANDLE socketThrId;*/

		static vector<Coordinates> myParticles;
		
};
