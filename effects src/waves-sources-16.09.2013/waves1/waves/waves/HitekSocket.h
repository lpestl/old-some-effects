#include <tinyxml2.h>
#include <winsock2.h>

#include "Windows.h"
#include <stdio.h>
#include "HitekSocketListener.h"

typedef union CoordinatesSoc{
	char stringCoor[12];
	struct{
		int x,y,ttl;
	};
} ;

class HitekSocket{
	private:			
		HANDLE threadHandle;
		void HitekSocket::initSocket();
	public:		
		HitekSocketListener *hitekSocketListener;
		DWORD  recvFromSocket();
		HitekSocket();	
		void stopRecvFromSocket();
		CoordinatesSoc onePoint;
		SOCKET clientSock;
		bool effectIsOpen;
		int inputMode;
		
};