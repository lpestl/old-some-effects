#include "HitekSocket.h"
//using namespace std;

template<class T>                            
struct ThreadToMember                         
{                                              
  template<DWORD (T::*pMemFn)()>               
  struct Dummy                                 
  {                                            
    static DWORD WINAPI Function(PVOID pv)     
    {                                          
      return (static_cast<T*>(pv)->*pMemFn)(); 
    }                                          
  };                                           
};   

HitekSocket::HitekSocket(){
	tinyxml2::XMLDocument doc;
	doc.LoadFile("settings.xml");
	auto docElement = doc.FirstChildElement("document");
	tinyxml2::XMLElement* e1 = docElement->FirstChildElement("inputmode");
	
	if(strcmp(e1->GetText(),"0")==1)
		inputMode=1;
	else
		inputMode=0;

	if(inputMode==0)
		initSocket();

	effectIsOpen=true;
}
void HitekSocket::initSocket(){
	tinyxml2::XMLDocument doc;
	doc.LoadFile("settings.xml");

	auto docElement = doc.FirstChildElement("document");
	tinyxml2::XMLElement* e1;

	e1=docElement->FirstChildElement("port");
	int port=atoi(e1->GetText());

	e1=docElement->FirstChildElement("host");

	WORD ver = MAKEWORD(2,2);
	WSADATA wsaData;
	int retVal=0;

	WSAStartup(ver,(LPWSADATA)&wsaData);
	
	struct in_addr addr = { 0 };

	addr.s_addr = inet_addr((char *)e1->GetText());
	LPHOSTENT hostEnt;
	hostEnt = gethostbyaddr((char *)&addr,4,AF_INET);

	clientSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	SOCKADDR_IN serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr = *((LPIN_ADDR)*hostEnt->h_addr_list);
	
	serverInfo.sin_port = htons(port);
	int size= sizeof (serverInfo);

	bind(clientSock,(struct sockaddr*)&serverInfo,size);
	
     // в качестве параметра (lpParam) передаем указатель на объект, чтобы переходник направил поток именно в этот объект
	LPTHREAD_START_ROUTINE pfnThreadStart = 
	&ThreadToMember<HitekSocket>::Dummy<&HitekSocket::recvFromSocket>::Function;
    threadHandle=CreateThread(0, 0, pfnThreadStart, this, 0, 0);
	//threadHandle = CreateThread(0, 0, &HitekSocket::recvFromSocket, NULL, 0, 0);
}
DWORD  HitekSocket::recvFromSocket(){

	CoordinatesSoc coor[10];
	int fl=1,fl2=0;

	struct sockaddr_in addr1;
	addr1.sin_family = AF_INET;
	int size=sizeof(addr1);

	while((fl==1 || fl2==0) && effectIsOpen){		
		try{
			int retVal = recvfrom(clientSock, coor[0].stringCoor,120,0,(struct sockaddr*)&addr1,&size);
			if(retVal!=SOCKET_ERROR)
				for(int i=0;i<10;i++){
					onePoint.x=coor[i].x;				
					onePoint.y=coor[i].y;
					//if(socketHandler!=NULL)
					hitekSocketListener->receiveAction();
				}
			else
				fl=0;
			fl2=1;			
		}
		catch(...){
		}		
	}	
	return 0;
}

void HitekSocket::stopRecvFromSocket(){
	effectIsOpen=false;
	closesocket(clientSock);
	WSACleanup();
	CloseHandle(threadHandle);
}