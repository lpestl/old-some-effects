#include "ofMain.h"
#include <winsock2.h>
#include "Windows.h"
#include <stdio.h>
#include "ofxXmlSettings.h"

typedef union CoordinatesSoc{
	char stringCoor[12];
	struct{
		int x,y,ttl;
	};
} ;

typedef void (__cdecl *EvtHandler)(void);

class HitekSocket{
	private:
		static ofxXmlSettings XML;
		static char host[255];
		static int portNumber;
		static DWORD WINAPI recvFromSocket(LPVOID param);
		static HANDLE threadHandle;
	public:
		HitekSocket();	
		void stopRecvFromSocket();
		static vector<CoordinatesSoc> myParticles;		
		static EvtHandler socketHandler;
		static CoordinatesSoc onePoint;
		static SOCKET clientSock;
		static bool effectIsOpen;
		
};