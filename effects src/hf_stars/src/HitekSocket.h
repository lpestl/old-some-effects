#include "ofMain.h"
#include <winsock2.h>
#include "Windows.h"
#include <stdio.h>
#include "ofxXmlSettings.h"
#include "HitekSocketListener.h"

//typedef void (__cdecl *EvtHandler)(void);
typedef union CoordinatesSoc{
	char stringCoor[12];
	struct{
		int x,y,ttl;
	};
} ;

class HitekSocket{
	private:
		ofxXmlSettings XML;
		char host[255];
		int portNumber;		
		HANDLE threadHandle;
	public:
		HitekSocketListener *hitekSocketListener;
		DWORD  recvFromSocket();
		HitekSocket();	
		void stopRecvFromSocket();
		vector<CoordinatesSoc> myParticles;	
		typedef void (__cdecl HitekSocket::*EvtHandler)(void);
		//EvtHandler socketHandler;
		CoordinatesSoc onePoint;
		SOCKET clientSock;
		bool effectIsOpen;
		
};