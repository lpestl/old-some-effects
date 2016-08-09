#include "testApp.h"

HitekSocket* testApp::hitekSocket;
vector <Leaf> testApp::theLeaves;
int testApp::s_donotmove; 
int testApp::s_movespeed; 
int testApp::s_return_wait;
int testApp::s_moveout;
//--------------------------------------------------------------
void testApp::setup(){
	
	ofHideCursor();

	ofSetFrameRate(60);

	XML.loadFile("settings.xml");

	string background = XML.getValue("settings:background", "bg.bmp");

	leaves_count = XML.getValue("settings:leaves_count",100);
	donotmove = XML.getValue("settings:donotmove",30);
	s_donotmove=donotmove;
	movespeed = XML.getValue("settings:movespeed",3);
	s_movespeed=movespeed;
	backspeed = XML.getValue("settings:backspeed",1);
	return_wait = XML.getValue("settings:returnwait",10) * 60;
	s_return_wait=return_wait;
	rotate = XML.getValue("settings:rotate",1);
	moveout = XML.getValue("settings:moveout",1);
	s_moveout=moveout;
	scale = XML.getValue("settings:scale",1.0);
	shadow_enabled = XML.getValue("settings:shadow",1);
	inputMode = XML.getValue("settings:inputmode",0);

	XML.pushTag("settings");
	XML.pushTag("images");
	string imagestr;
	for (int i = 0; i<XML.getNumTags("image"); i++){
		imagestr = XML.getValue("image","!",i);
		if (imagestr!="!"){
			ofImage newimage;
			newimage.loadImage(imagestr);
			if (scale != 1) {
				newimage.resize(newimage.width * scale, newimage.height * scale);
			}
			images.push_back(newimage);
		}
	}

	
	hf_background.loadImage(background);
	
	for (int i=0; i<leaves_count; i++){
		theLeaves.push_back(Leaf(&images[ofRandom(images.size())],scale,shadow_enabled,moveout,rotate));
	}

	if(inputMode==0){
		hitekSocket=new HitekSocket();
		hitekSocket->socketHandler=&pushSocketData;
	}
}

void testApp::pushSocketData(){
	for(int i=0; i<theLeaves.size(); i++){
		if(theLeaves[i].hittest(hitekSocket->onePoint.x,hitekSocket->onePoint.y) == true && theLeaves[i].canmove == true){
			theLeaves[i].move(s_donotmove, s_movespeed, s_return_wait, s_moveout);
		}
	}
}

//--------------------------------------------------------------
void testApp::update(){
	
	for(int i=0; i<theLeaves.size(); i++){
		if (theLeaves[i].return_wait == 0 && theLeaves[i].x!= theLeaves[i].origX && theLeaves[i].y != theLeaves[i].origY){
			theLeaves[i].move_back(donotmove, backspeed, return_wait);
		}
	}
	

	Tweener.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255);
	ofBackground(0,0,0);
	hf_background.draw(0,0,0,ofGetWindowWidth(),ofGetWindowHeight());
	ofEnableAlphaBlending();
	

	for(int i=0; i<theLeaves.size(); i++){
		theLeaves[i].draw();
	}
	

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	//if( key == 'f'){
	//	ofToggleFullscreen();
	//}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

	if(inputMode==1)
		for(int i=0; i<theLeaves.size(); i++){
			if(theLeaves[i].hittest(mouseX,mouseY) == true && theLeaves[i].canmove == true){
				theLeaves[i].move(donotmove, movespeed, return_wait, moveout);
			}
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