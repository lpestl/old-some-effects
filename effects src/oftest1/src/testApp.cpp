#include "testApp.h"

vector<Coordinates> testApp::myParticles;
HitekSocket* testApp::hitekSocket;
int testApp::maxParticles;
int testApp::particlesTTL;
//--------------------------------------------------------------
void testApp::setup(){

	XML.loadFile("settings.xml");

	maxParticles = XML.getValue("settings:maxparticles", 100000);
	particlesTTL = XML.getValue("settings:particlesttl", 60);
	dotSize = XML.getValue("settings:dotsize",10);
	inputMode = XML.getValue("settings:inputmode",0);

	if(inputMode==0){
		hitekSocket=new HitekSocket();
		hitekSocket->socketHandler=&pushSocketData;
	}
	
	
	ofSetFrameRate(60);
}

#include <stdio.h>
#include <algorithm>

void testApp::pushSocketData(){
		Coordinates coor;
		coor.ttl=particlesTTL;
		coor.x=hitekSocket->onePoint.x;
		coor.y=hitekSocket->onePoint.y;
		
		if(myParticles.size()<40000)
			myParticles.push_back(coor);
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofClear(0,0,0);
	for (int i=0; i<myParticles.size(); i++){
		if (myParticles[i].ttl>0){
			ofCircle(myParticles[i].x,myParticles[i].y,dotSize);
			myParticles[i].ttl--;
			
		}
		else {
			vector<Coordinates>::iterator p = myParticles.begin()+i;
			myParticles.erase(p);
		}
			
	}
		
		
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'f') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	if(myParticles.size()<maxParticles && inputMode==1){
		Coordinates pp;
		pp.x = x;
		pp.y = y;
		pp.ttl = particlesTTL;
		myParticles.push_back(pp);
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}