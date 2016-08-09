#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

#include "wtypes.h"
#include <iostream>
using namespace std;


void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   // Get a handle to the desktop window
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);

   horizontal = desktop.right;
   vertical = desktop.bottom;
}

//========================================================================
int main( ){
	
	int wdth = 0;
	int hght = 0;

	GetDesktopResolution(wdth, hght);

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, wdth, hght, OF_FULLSCREEN);			// <-------- setup the GL context
	//ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
