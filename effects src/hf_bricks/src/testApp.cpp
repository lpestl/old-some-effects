#include "testApp.h"

HitekSocket* testApp::hitekSocket;
vector <Brick> testApp::theBricks;
int testApp::donotmove;
int testApp::return_wait;
//--------------------------------------------------------------
void testApp::setup(){
	
	ofHideCursor();

	ofSetFrameRate(60);


	XML.loadFile("settings.xml");
	string background = XML.getValue("settings:background", "bg.bmp");
	string foreground = XML.getValue("settings:foreground", "hiteklab.jpg");
	donotmove = XML.getValue("settings:donotmove",30);
	movespeed = XML.getValue("settings:movespeed",5);
	backspeed = XML.getValue("settings:backspeed",3);
	return_wait = XML.getValue("settings:returnwait",10) * 60;
	moveout = XML.getValue("settings:moveout",1);
	rotate = XML.getValue("settings:rotate",1);
	shadow_enabled = XML.getValue("settings:shadow",1);
	bricksw = XML.getValue("settings:width",24);
	bricksh = XML.getValue("settings:height",18);
	inputMode = XML.getValue("settings:inputmode",0);
	
	hf_background.loadImage(background);
	hf_foreground.loadImage(foreground);

	hf_background.resize(ofGetWindowWidth(), ofGetWindowHeight());
	hf_foreground.resize(ofGetWindowWidth(), ofGetWindowHeight());

		
	float brickw = ofGetWindowWidth() / bricksw;
	float brickh = ofGetWindowHeight() / bricksh;

	for (int i=0; i<bricksw; i++){
		for (int j=0; j<bricksh; j++){
			float xt = i * brickw;
			float yt = j * brickh;
			theBricks.push_back(Brick(&hf_foreground, xt, yt, brickw, brickh, shadow_enabled, donotmove, return_wait, moveout, rotate, movespeed, backspeed));
		}
	}
	
	if(inputMode==0){
		hitekSocket=new HitekSocket();
		hitekSocket->socketHandler=&pushSocketData;
	}
	//ss << "mouse ";
}

void testApp::pushSocketData(){
	for(int i=0; i<theBricks.size(); i++){
		if(theBricks[i].hittest(hitekSocket->onePoint.x,hitekSocket->onePoint.y) == true && theBricks[i].canmove == true){
			theBricks[i].move(return_wait, donotmove);
		} 
	}
}

//--------------------------------------------------------------
void testApp::update(){
	
	for(int i=0; i<theBricks.size(); i++){
		if (theBricks[i].return_wait == 0 && theBricks[i].x != theBricks[i].origX && theBricks[i].y != theBricks[i].origY){
			theBricks[i].move_back(return_wait,donotmove);
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
	

	for(int i=0; i<theBricks.size(); i++){
		if (theBricks[i].at_origin == true) {
			theBricks[i].draw();
		}
	}
	for(int i=0; i<theBricks.size(); i++){
		if (theBricks[i].at_origin == false) {
			theBricks[i].draw();
		}
	}


	//// service info
	//ss.str("");
	//for (int i=0; i<theBricks.size();i++){
	//	ss << "x="<< theBricks[i].x << " y=" <<theBricks[i].y << " w=" << theBricks[i].brickwidth << " h=" << theBricks[i].brickwidth << " canmove=" << theBricks[i].canmove << " shdw=" << theBricks[i].shadow_enabled << " dntmove=" << theBricks[i].donotmoveframes << " rtrnwait=" << theBricks[i].return_wait << " moveout=" << theBricks[i].moveout << " rotate=" << theBricks[i].rotate << endl;
	//}
	//ofDrawBitmapString(ss.str(), ofPoint(10,20));

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
		for(int i=0; i<theBricks.size(); i++){
			if(theBricks[i].hittest(mouseX,mouseY) == true && theBricks[i].canmove == true){
				theBricks[i].move(return_wait, donotmove);
			} 
		}

	

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
//	ss.clear();
//	ss << "mouse pressed " << button << " at " << x << " " << y;
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