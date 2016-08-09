#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include <stdio.h>

//typedef void (__cdecl *EvtHandler)(int);


//void f1(int a){
//	printf("Hello %d",a);
//}

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context

	//EvtHandler x = f1;
	//x(1);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
