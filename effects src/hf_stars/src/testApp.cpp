#include "testApp.h"
#include "object.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofHideCursor();

	ofSetFrameRate(60);
	
	XML.loadFile("settings.xml");

	string background = XML.getValue("settings:background", "black.png");
	int blendm = XML.getValue("settings:blendmode", 2);
	colorize = XML.getValue("settings:colorize", 1);
	ttl = XML.getValue("settings:ttl", 60);
	rotate = XML.getValue("settings:rotate", 1);
	rotateall = XML.getValue("settings:rotateall", 0.1);
	scale = XML.getValue("settings:startscale",1.0);
	endscale = XML.getValue("settings:endscale",1.0);
	limit_width = XML.getValue("settings:limitwidth",64);
	limit_height = XML.getValue("settings:limitheight",36);
	limit = XML.getValue("settings:limit",1);
	inputMode = XML.getValue("settings:inputmode",0);

	for (int i=0;i<limit_width; i++){
		for (int j=0;j<limit_height; j++){
			limit_grid[i][j] = 0;
		}
	}
	
	switch (blendm) {
        case 1:
            blendmode = OF_BLENDMODE_ALPHA;
            break;
        case 2:
            blendmode = OF_BLENDMODE_ADD;
            break;
        case 4:
            blendmode = OF_BLENDMODE_MULTIPLY;
            break;
        case 3:
            blendmode = OF_BLENDMODE_SUBTRACT;
            break;
        case 5:
            blendmode = OF_BLENDMODE_SCREEN;
            break;
        default:
			blendmode = OF_BLENDMODE_ADD;
            break;
    }


	hf_background.loadImage(background);

	XML.pushTag("settings");
	XML.pushTag("images");
	
	string imagestr;

	for (int i = 0; i<XML.getNumTags("image"); i++){
		imagestr = XML.getValue("image","!",i);
		if (imagestr!="!"){
			ofImage newimage;
			newimage.loadImage(imagestr);
			images.push_back(newimage);
		}
	}
	hitekSocket=new HitekSocket();
	if(inputMode==0){
		hitekSocket->hitekSocketListener=this;
	}

	ofEnableAlphaBlending();

}

void testApp::receiveAction(){//pushSocketData(){
	if (limit_grid[mouse_to_grid_x(hitekSocket->onePoint.x)][mouse_to_grid_y(hitekSocket->onePoint.y)]<limit){
		limit_grid[mouse_to_grid_x(hitekSocket->onePoint.x)][mouse_to_grid_y(hitekSocket->onePoint.y)]+=1;
		//cout << " lim grid x y = " << limit_grid[mouse_to_grid_x(x)][mouse_to_grid_y(y)] << endl;
		if(objects.size()<40000){
			//printf("%d\n",hitekSocket->onePoint.ttl);
			objects.push_back(Object(&images[ofRandom(images.size())], hitekSocket->onePoint.x, hitekSocket->onePoint.y, ttl, rotate, scale, colorize));
		}
			
	}
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetColor(255);
	ofBackground(0,0,0);
	
	hf_background.draw(0,0,ofGetWindowWidth(),ofGetWindowHeight());
	ofEnableBlendMode(blendmode);
	for (int i=0; i<objects.size(); i++){
		if (objects[i].ttl>0){	
			float maxttlf = objects[i].maxttl;
			float ttlf = objects[i].ttl;
			// Вращение
			if (objects[i].ttl>=objects[i].maxttl/2){
				float tm = ((ttlf-maxttlf)/(-maxttlf/2));
				float aa = 255 * tm;
				objects[i].a = aa;
			} else {
				//objects[i].a = ttlf * (255/(maxttlf/2)); // <-this part works!
				float tm = (ttlf/(maxttlf/2));
				float aa = 255 * tm;
				objects[i].a = aa;
			}
			// Масштаб
			objects[i].scale = (endscale - scale) * ((ttlf-maxttlf)/(-maxttlf))+scale;

			objects[i].r+=rotateall;
			objects[i].draw();
			objects[i].ttl--;
		}
		else {
			limit_grid[mouse_to_grid_x(objects[i].x)][mouse_to_grid_y(objects[i].y)]-=1;
			//cout << " lim grid x y = " << limit_grid[mouse_to_grid_x(objects[i].x)][mouse_to_grid_y(objects[i].y)] << endl;
			vector<Object>::iterator p = objects.begin()+i;
			objects.erase(p);
		}
			
	}

	//ofSetColor(255,0,0);
	
	//tempimage.draw(mouseX-tempimage.width/2,mouseY-tempimage.height/2);
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	//cout << "gridx = " << mouse_to_grid_x(x) << " gridy = " << mouse_to_grid_y(y) << endl;
	if(inputMode==1)
		if (limit_grid[mouse_to_grid_x(x)][mouse_to_grid_y(y)]<limit){
			limit_grid[mouse_to_grid_x(x)][mouse_to_grid_y(y)]+=1;
			//cout << " lim grid x y = " << limit_grid[mouse_to_grid_x(x)][mouse_to_grid_y(y)] << endl;
			objects.push_back(Object(&images[ofRandom(images.size())], x, y, ttl, rotate, scale, colorize));
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








int testApp::mouse_to_grid_x(int mx){
	float wd = ofGetWindowWidth();
	float temp = (limit_width/wd)*mx;
	return temp;
}

int testApp::mouse_to_grid_y(int my){
	float hg = ofGetWindowHeight();
	float temp = (limit_height/hg)*my;
	return temp;
}