#include "ofApp.h"


#define MAX_USERS 10
bool g_visibleUsers[MAX_USERS] = {false};
nite::SkeletonState g_skeletonStates[MAX_USERS] = {nite::SKELETON_NONE};

#define USER_MESSAGE(msg) \
	{printf("[%08llu] User #%d:\t%s\n",ts, user.getId(),msg);}

#define GL_WIN_SIZE_X	1280
#define GL_WIN_SIZE_Y	1024
int g_nXRes = 0, g_nYRes = 0;

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableSmoothing();
    ofSetFrameRate(60);

	trackingReference_Speed = 0;
	trackingReference_Volume = 1;
	trackingReference_Pan = 2;

	marker = ofPoint(ofGetWidth()/2, ofGetHeight()/2, 1);

	//Setup NiTE data
	setupKinect();

	//Setup sound sample
	sound.loadSound( "budamonk.wav" );
	sound.setMultiPlay( true );
	sound.setLoop(true);

	sound.play();

	isPlaying = true;

	setupGUI();	
}

void ofApp::update(){
	//1. Update joints position information
	updateKinectData();

	//2. Update sound data
	//SPEED
	float soundSpeed = 0;
	switch(trackingReference_Speed){
	case 0:
		soundSpeed = ofMap(headPos.x, 0, ofGetWidth(), 0.5, 1.5);
		break;
	case 1:
		soundSpeed = ofMap( headPos.y, ofGetHeight(), 0, 0.5, 1.5 );
		break;
	case 2: 
		soundSpeed = ofMap( abs(marker.x - headPos.x), 0, ofGetWidth(), 0.5, 1.5 );
		break;
	case 3:
		soundSpeed = ofMap( abs(mouseX - headPos.x), 0, ofGetWidth(), 0.5, 1.5 );
		break;
	}
	sound.setSpeed( soundSpeed );
	
	//VOLUME
	float volume = 0;
	switch(trackingReference_Volume){
	case 0:
		volume = ofMap(headPos.x, 0, ofGetWidth(), 0.0, 1.0);
		break;
	case 1:
		volume = ofMap( headPos.y, ofGetHeight(), 0, 0.0, 1.0);
		break;
	case 2: 
		volume = ofMap( abs(marker.y - headPos.y), ofGetHeight()/2, 0, 0.0, 1.0 );
		break;
	case 3:
		volume = ofMap( abs(mouseX - headPos.x), 0, ofGetWidth(), 0.0, 1.0);
		break;
	}
	sound.setVolume( volume );

	//PANNING
	float pan = 0;
	switch(trackingReference_Pan){
	case 0:
		pan = ofMap(headPos.x, 0, ofGetWidth(), -1.0, 1.0);
		break;
	case 1:
		pan = ofMap( headPos.y, ofGetHeight(), 0, -1.0, 1.0);
		break;
	case 2: 
		pan = ofMap( abs(marker.y - headPos.y), ofGetHeight()/2, 0, -1.0, 1.0 );
		break;
	case 3:
		pan = ofMap( abs(mouseX - headPos.x), 0, ofGetWidth(), -1.0, 1.0);
		break;
	}
	sound.setPan( pan );

	ofSoundUpdate(); //Update sound engine
}

void ofApp::draw(){
	ofBackgroundGradient(ofColor::gray,ofColor::black, OF_GRADIENT_CIRCULAR);
    ofSetColor(ofColor::green);
    ofFill();
    ofCircle( headPos.x, headPos.y, 20);

	ofSetColor(ofColor::black);
	ofCircle(marker.x, marker.y, 10);
	ofDrawBitmapString("Marker", marker.x - 5, marker.y - 10);

	ofSetColor(ofColor::red);
	ofRect(mouseX, mouseY, 10, 10);

	ofSetColor(200,200,124);
	ostringstream buff;
	buff << sound.getSpeed();
	ofDrawBitmapString("Speed: " + buff.str(), 10, ofGetHeight() - 50);
	
	
	buff.str("");
	buff.clear();
	buff << sound.getVolume();
	ofDrawBitmapString("Volume: " + buff.str(), 10, ofGetHeight() - 35);


	buff.str("");
	buff.clear();
	buff << sound.getPan();
	ofDrawBitmapString("Panning: " + buff.str(), 10, ofGetHeight() - 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key){
	case ' ':
		sound.setPaused(isPlaying);
		isPlaying = !isPlaying;
		break;
	}
}

void ofApp::windowResized(int w, int h){
	marker = ofPoint(ofGetWidth()/2, ofGetHeight()/2, 1);
}

// ---------------------------------------------
void ofApp::setupGUI(){
	gui = new ofxUISuperCanvas("SOUND CONTROL:");		//Creates a canvas at (0,0) using the default width	
	gui->setDrawWidgetPadding(false);
	gui->setPosition(5, 5);
	gui->setVisible(true);		//When showJointPanel() method is called it will assign true if this is the GUI refering the joint selected
	
	gui->addSpacer();

	controllers.push_back("X-axis");
	controllers.push_back("Y-axis");
	controllers.push_back("Distance to Marker");
	controllers.push_back("Distance to Mouse");
	
	gui->addLabel("Speed:", OFX_UI_FONT_SMALL);
	ddl_speed = gui->addDropDownList("Select speed controller", controllers);
	ddl_speed->setAllowMultiple(false);
    ddl_speed->setAutoClose(true);
	ddl_speed->setShowCurrentSelected(true);
    

	gui->addLabel("Volume:", OFX_UI_FONT_SMALL);
	ddl_volume = gui->addDropDownList("Select volume controller", controllers);
	ddl_volume->setAllowMultiple(false);
    ddl_volume->setAutoClose(true);
	ddl_volume->setShowCurrentSelected(true);
    

	gui->addLabel("Panning:", OFX_UI_FONT_SMALL);
	ddl_pan = gui->addDropDownList("Select pan controller", controllers);
	ddl_pan->setAllowMultiple(false);
    ddl_pan->setAutoClose(true);
	ddl_pan->setShowCurrentSelected(true);
    

	gui->autoSizeToFitWidgets(); 
	ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
 
    if(name == "Select speed controller")
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
		for(int i = 0; i < selected.size(); i++){
            cout << "SPEED CONTROL: " << selected[i]->getName() << endl;
			
			if(selected[i]->getName() == "X-axis")
				trackingReference_Speed = 0;
			else
				if(selected[i]->getName() == "Y-axis")
					trackingReference_Speed = 1;
				else
					if(selected[i]->getName() == "Distance to Marker")
						trackingReference_Speed = 2;
					else
						trackingReference_Speed = 3;
		}
	}
	else
		if(name == "Select volume controller")
		{
			ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
			vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
			for(int i = 0; i < selected.size(); i++){
				cout << "VOLUME CONTROL: " << selected[i]->getName() << endl;
			
				if(selected[i]->getName() == "X-axis")
					trackingReference_Volume = 0;
				else
					if(selected[i]->getName() == "Y-axis")
						trackingReference_Volume = 1;
					else
						if(selected[i]->getName() == "Distance to Marker")
							trackingReference_Volume = 2;
						else
							trackingReference_Volume = 3;
			}
		}
		else
			if(name == "Select pan controller")
			{
				ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
				vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
				for(int i = 0; i < selected.size(); i++){
					cout << "PAN CONTROL: " << selected[i]->getName() << endl;
			
					if(selected[i]->getName() == "X-axis")
						trackingReference_Pan = 0;
					else
						if(selected[i]->getName() == "Y-axis")
							trackingReference_Pan = 1;
						else
							if(selected[i]->getName() == "Distance to Marker")
								trackingReference_Pan = 2;
							else
								trackingReference_Pan = 3;
				}
			}
}

void ofApp::exit(){
	delete gui;
}
	
// ---------------------------------------------
void ofApp::setupKinect(){
	niteRc = nite::NiTE::initialize();
	if (niteRc != nite::STATUS_OK)	{
		printf("NiTE initialization failed\n");
	}

	niteRc = userTracker.create();
	if (niteRc != nite::STATUS_OK)	{
		printf("Couldn't create user tracker\n");
	}
	printf("\nStart moving around to get detected...\n");
}

void ofApp::updateKinectData(){
	niteRc = userTracker.readFrame(&userTrackerFrame);
	if(niteRc != nite::STATUS_OK)
		printf("Get next frame failed\n");

	const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
	for (int i = 0; i < users.getSize(); i++)
	{
		const nite::UserData& user = users[i];
		updateUserState(user,userTrackerFrame.getTimestamp());
		
		if (user.isNew())
			userTracker.startSkeletonTracking(user.getId());
		else
			if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
			{
				const nite::SkeletonJoint& head = user.getSkeleton().getJoint(nite::JOINT_HEAD);
				const nite::SkeletonJoint& neck = user.getSkeleton().getJoint(nite::JOINT_NECK);
				const nite::SkeletonJoint& handL = user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND);
				const nite::SkeletonJoint& elbowL = user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW);
				const nite::SkeletonJoint& handR = user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND);
				const nite::SkeletonJoint& elbowR = user.getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW);
				const nite::SkeletonJoint& shoulderL = user.getSkeleton().getJoint(nite::JOINT_LEFT_SHOULDER);
				const nite::SkeletonJoint& shoulderR = user.getSkeleton().getJoint(nite::JOINT_RIGHT_SHOULDER);
				const nite::SkeletonJoint& torso = user.getSkeleton().getJoint(nite::JOINT_TORSO);
				const nite::SkeletonJoint& hipL = user.getSkeleton().getJoint(nite::JOINT_LEFT_HIP);
				const nite::SkeletonJoint& hipR = user.getSkeleton().getJoint(nite::JOINT_RIGHT_HIP);
				const nite::SkeletonJoint& kneeL = user.getSkeleton().getJoint(nite::JOINT_LEFT_KNEE);
				const nite::SkeletonJoint& kneeR = user.getSkeleton().getJoint(nite::JOINT_RIGHT_KNEE);
				const nite::SkeletonJoint& footL = user.getSkeleton().getJoint(nite::JOINT_LEFT_FOOT);
				const nite::SkeletonJoint& footR = user.getSkeleton().getJoint(nite::JOINT_RIGHT_FOOT);
				
				const nite::Point3f center = user.getCenterOfMass();

		/*0*/	headPos = ofPoint(head.getPosition().x + ofGetWidth()/2, -(head.getPosition().y - ofGetHeight()/2), -head.getPosition().z);
		/*1*/	neckPos = ofPoint(neck.getPosition().x + ofGetWidth()/2, -(neck.getPosition().y - ofGetHeight()/2), -neck.getPosition().z);
		/*2*/	handLPos = ofPoint(handL.getPosition().x + ofGetWidth()/2, -(handL.getPosition().y - ofGetHeight()/2), -handL.getPosition().z);
		/*3*/	elbowLPos = ofPoint(elbowL.getPosition().x + ofGetWidth()/2, -(elbowL.getPosition().y - ofGetHeight()/2), -elbowL.getPosition().z);
		/*4*/	handRPos = ofPoint(handR.getPosition().x + ofGetWidth()/2, -(handR.getPosition().y - ofGetHeight()/2), -handR.getPosition().z);
		/*5*/	elbowRPos = ofPoint(elbowR.getPosition().x + ofGetWidth()/2, -(elbowR.getPosition().y - ofGetHeight()/2), -elbowR.getPosition().z);
		/*6*/	shoulderLPos = ofPoint(shoulderL.getPosition().x + ofGetWidth()/2, -(shoulderL.getPosition().y - ofGetHeight()/2), -shoulderL.getPosition().z);
		/*7*/	shoulderRPos = ofPoint(shoulderR.getPosition().x + ofGetWidth()/2, -(shoulderR.getPosition().y - ofGetHeight()/2), -shoulderR.getPosition().z);
		/*8*/	torsoPos = ofPoint(torso.getPosition().x + ofGetWidth()/2, -(torso.getPosition().y - ofGetHeight()/2), -torso.getPosition().z);
		/*9*/	hipLPos = ofPoint(hipL.getPosition().x + ofGetWidth()/2, -(hipL.getPosition().y - ofGetHeight()/2), -hipL.getPosition().z);
		/*10*/	hipRPos = ofPoint(hipR.getPosition().x + ofGetWidth()/2, -(hipR.getPosition().y - ofGetHeight()/2), -hipR.getPosition().z);
		/*11*/	kneeLPos = ofPoint(kneeL.getPosition().x + ofGetWidth()/2, -(kneeL.getPosition().y - ofGetHeight()/2), -kneeL.getPosition().z);
		/*12*/	kneeRPos = ofPoint(kneeR.getPosition().x + ofGetWidth()/2, -(kneeR.getPosition().y - ofGetHeight()/2), -kneeR.getPosition().z);
		/*13*/	footLPos = ofPoint(footL.getPosition().x + ofGetWidth()/2, -(footL.getPosition().y - ofGetHeight()/2), -footL.getPosition().z);
		/*14*/	footRPos = ofPoint(footR.getPosition().x + ofGetWidth()/2, -(footR.getPosition().y - ofGetHeight()/2), -footR.getPosition().z);

				centerPos = ofPoint(center.x + ofGetWidth()/2, -(center.y - ofGetHeight()/2), -center.z);
			}
	}
}

void ofApp::updateUserState(const nite::UserData& user, unsigned long long ts)
{
	if (user.isNew())
		USER_MESSAGE("New")
	else if (user.isVisible() && !g_visibleUsers[user.getId()])
		USER_MESSAGE("Visible")
	else if (!user.isVisible() && g_visibleUsers[user.getId()])
		USER_MESSAGE("Out of Scene")
	else if (user.isLost())
		USER_MESSAGE("Lost")

	g_visibleUsers[user.getId()] = user.isVisible();


	if(g_skeletonStates[user.getId()] != user.getSkeleton().getState())
	{
		switch(g_skeletonStates[user.getId()] = user.getSkeleton().getState())
		{
		case nite::SKELETON_NONE:
			USER_MESSAGE("Stopped tracking.")
			break;
		case nite::SKELETON_CALIBRATING:
			USER_MESSAGE("Calibrating...")
			break;
		case nite::SKELETON_TRACKED:
			USER_MESSAGE("Tracking!")
			break;
		case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
		case nite::SKELETON_CALIBRATION_ERROR_HANDS:
		case nite::SKELETON_CALIBRATION_ERROR_LEGS:
		case nite::SKELETON_CALIBRATION_ERROR_HEAD:
		case nite::SKELETON_CALIBRATION_ERROR_TORSO:
			USER_MESSAGE("Calibration Failed... :-|")
			break;
		}
	}
}
