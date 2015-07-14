#include "ofApp.h"


#define MAX_USERS 10
bool g_visibleUsers[MAX_USERS] = {false};
nite::SkeletonState g_skeletonStates[MAX_USERS] = {nite::SKELETON_NONE};

#define USER_MESSAGE(msg) \
	{printf("[%08llu] User #%d:\t%s\n",ts, user.getId(),msg);}

#define GL_WIN_SIZE_X	1280
#define GL_WIN_SIZE_Y	1024
int g_nXRes = 0, g_nYRes = 0;


const int jointSize = 10;
const int Zintensity = 2;

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

	showSkeleton = false;

	dest_speed = dest_volume = dest_panning = ofPoint(ofGetWidth()/2, ofGetHeight()/2, 1);
}

void ofApp::update(){
	//1. Update joints position information
	updateKinectData();

	//2. Update sound data
	//SPEED
	float soundSpeed = 0;

	switch(joint_Speed){
	case 0:
		dest_speed = headPos;
		break;
	case 1:
		dest_speed = neckPos;
		break;
	case 2:
		dest_speed = shoulderLPos;
		break;
	case 3:
		dest_speed = shoulderRPos;
		break;
	case 4:
		dest_speed = elbowLPos;
		break;
	case 5:
		dest_speed = elbowRPos;
		break;
	case 6:
		dest_speed = handLPos;
		break;
	case 7:
		dest_speed = handRPos;
		break;
	case 8:
		dest_speed = torsoPos;
		break;
	case 9:
		dest_speed = hipLPos;
		break;
	case 10:
		dest_speed = hipRPos;
		break;
	case 11:
		dest_speed = kneeLPos;
		break;
	case 12:
		dest_speed = kneeRPos;
		break;
	case 13:
		dest_speed = footLPos;
		break;
	case 14:
		dest_speed = footRPos;
		break;
	}

	switch(trackingReference_Speed){
	case 0:
		soundSpeed = ofMap(dest_speed.x, 0, ofGetWidth(), 0.5, 1.5);
		break;
	case 1:
		soundSpeed = ofMap( dest_speed.y, ofGetHeight(), 0, 0.5, 1.5 );
		break;
	case 2: 
		soundSpeed = ofMap( abs(marker.x - dest_speed.x), 0, ofGetWidth(), 0.5, 1.5 );
		break;
	case 3:
		soundSpeed = ofMap( abs(mouseX - dest_speed.x), 0, ofGetWidth(), 0.5, 1.5 );
		break;
	}
	sound.setSpeed( soundSpeed );
	
	//VOLUME
	float volume = 0;

	switch(joint_Volume){
	case 0:
		dest_volume = headPos;
		break;
	case 1:
		dest_volume = neckPos;
		break;
	case 2:
		dest_volume = shoulderLPos;
		break;
	case 3:
		dest_volume = shoulderRPos;
		break;
	case 4:
		dest_volume = elbowLPos;
		break;
	case 5:
		dest_volume = elbowRPos;
		break;
	case 6:
		dest_volume = handLPos;
		break;
	case 7:
		dest_volume = handRPos;
		break;
	case 8:
		dest_volume = torsoPos;
		break;
	case 9:
		dest_volume = hipLPos;
		break;
	case 10:
		dest_volume = hipRPos;
		break;
	case 11:
		dest_volume = kneeLPos;
		break;
	case 12:
		dest_volume = kneeRPos;
		break;
	case 13:
		dest_volume = footLPos;
		break;
	case 14:
		dest_volume = footRPos;
		break;
	}

	switch(trackingReference_Volume){
	case 0:
		volume = ofMap(dest_volume.x, 0, ofGetWidth(), 0.0, 1.0);
		break;
	case 1:
		volume = ofMap( dest_volume.y, ofGetHeight(), 0, 0.0, 1.0);
		break;
	case 2: 
		volume = ofMap( abs(marker.x - dest_volume.x), ofGetHeight()/2, 0, 0.0, 1.0 );
		break;
	case 3:
		volume = ofMap( abs(mouseX - dest_volume.x), 0, ofGetWidth(), 0.0, 1.0);
		break;
	}
	sound.setVolume( volume );

	//PANNING
	float pan = 0;

	switch(joint_Pan){
	case 0:
		dest_panning = headPos;
		break;
	case 1:
		dest_panning = neckPos;
		break;
	case 2:
		dest_panning = shoulderLPos;
		break;
	case 3:
		dest_panning = shoulderRPos;
		break;
	case 4:
		dest_panning = elbowLPos;
		break;
	case 5:
		dest_panning = elbowRPos;
		break;
	case 6:
		dest_panning = handLPos;
		break;
	case 7:
		dest_panning = handRPos;
		break;
	case 8:
		dest_panning = torsoPos;
		break;
	case 9:
		dest_panning = hipLPos;
		break;
	case 10:
		dest_panning = hipRPos;
		break;
	case 11:
		dest_panning = kneeLPos;
		break;
	case 12:
		dest_panning = kneeRPos;
		break;
	case 13:
		dest_panning = footLPos;
		break;
	case 14:
		dest_panning = footRPos;
		break;
	}
	switch(trackingReference_Pan){
	case 0:
		pan = ofMap(dest_panning.x, 0, ofGetWidth(), -1.0, 1.0);
		break;
	case 1:
		pan = ofMap( dest_panning.y, ofGetHeight(), 0, -1.0, 1.0);
		break;
	case 2: 
		pan = ofMap( abs(marker.x - dest_panning.x), ofGetHeight()/2, 0, -1.0, 1.0 );
		break;
	case 3:
		pan = ofMap( abs(mouseX - dest_panning.x), 0, ofGetWidth(), -1.0, 1.0);
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

	//Draw skeleton
	if(showSkeleton)
		drawSkeleton();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key){
	case ' ':
		sound.setPaused(isPlaying);
		isPlaying = !isPlaying;
		break;
	case 'h':
		showSkeleton = !showSkeleton;
		break;
	}
}

void ofApp::windowResized(int w, int h){
	marker = ofPoint(ofGetWidth()/2, ofGetHeight()/2, 1);
}

// ---------------------------------------------
void ofApp::drawSkeleton(){
	ofSetColor(ofColor::green);
		ofRect(headPos.x, headPos.y, headPos.z, jointSize, jointSize);
		ofRect(neckPos.x, neckPos.y, neckPos.z, jointSize, jointSize);
		ofRect(handLPos.x, handLPos.y, handLPos.z, jointSize, jointSize);
		ofRect(elbowLPos.x, elbowLPos.y, elbowLPos.z, jointSize, jointSize);
		ofRect(handRPos.x, handRPos.y, handRPos.z, jointSize, jointSize);
		ofRect(elbowRPos.x, elbowRPos.y, elbowRPos.z, jointSize, jointSize);
		ofRect(shoulderLPos.x, shoulderLPos.y, shoulderLPos.z, jointSize, jointSize);
		ofRect(shoulderRPos.x, shoulderRPos.y, shoulderRPos.z, jointSize, jointSize);
		ofRect(torsoPos.x, torsoPos.y, torsoPos.z, jointSize, jointSize);
		ofRect(hipLPos.x, hipLPos.y, hipLPos.z, jointSize, jointSize);
		ofRect(hipRPos.x, hipRPos.y, hipRPos.z, jointSize, jointSize);
		ofRect(kneeLPos.x, kneeLPos.y, kneeLPos.z, jointSize, jointSize);
		ofRect(kneeRPos.x, kneeRPos.y, kneeRPos.z, jointSize, jointSize);
		ofRect(footLPos.x, footLPos.y, footLPos.z, jointSize, jointSize);
		ofRect(footRPos.x, footRPos.y, footRPos.z, jointSize, jointSize);
		
		ofSetColor(ofColor::black);
		ofDrawBitmapString("Speed", dest_speed.x - 5, dest_speed.y - 10, dest_speed.z);
		ofDrawBitmapString("Volume", dest_volume.x - 5, dest_volume.y - 10, dest_volume.z);
		ofDrawBitmapString("Panning", dest_panning.x - 5, dest_panning.y - 10, dest_panning.z);

		ofSetColor(ofColor::green);
		ofLine(headPos, neckPos);
		ofLine(neckPos, shoulderLPos);
		ofLine(neckPos, shoulderRPos);
		ofLine(shoulderLPos, elbowLPos);
		ofLine(elbowLPos, handLPos);
		ofLine(shoulderRPos, elbowRPos);
		ofLine(elbowRPos, handRPos);
		ofLine(shoulderLPos, torsoPos);
		ofLine(shoulderRPos, torsoPos);
		ofLine(torsoPos, hipLPos);
		ofLine(torsoPos, hipRPos);
		ofLine(hipLPos, hipRPos);
		ofLine(hipLPos, kneeLPos);
		ofLine(hipRPos, kneeRPos);
		ofLine(kneeLPos, footLPos);
		ofLine(kneeRPos, footRPos);
}

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

	joints.push_back("Head");
	joints.push_back("Neck");
	joints.push_back("Left Shoulder");
	joints.push_back("Right Shoulder");
	joints.push_back("Left Elbow");
	joints.push_back("Right Elbow");
	joints.push_back("Left Hand");
	joints.push_back("Right Hand");
	joints.push_back("Torso");
	joints.push_back("Left Hip");
	joints.push_back("Right Hip");
	joints.push_back("Left Knee");
	joints.push_back("Right Knee");
	joints.push_back("Left Foot");
	joints.push_back("Right Foot");
	
	gui->addLabel("SPEED");
	gui->addLabel("Joint:", OFX_UI_FONT_SMALL);
	ddl_joints_speed = gui->addDropDownList("Select joint for speed", joints);
	ddl_joints_speed->setAllowMultiple(false);
    ddl_joints_speed->setAutoClose(true);
	ddl_joints_speed->setShowCurrentSelected(true);
	gui->addLabel("Controller:", OFX_UI_FONT_SMALL);
	ddl_speed = gui->addDropDownList("Select speed controller", controllers);
	ddl_speed->setAllowMultiple(false);
    ddl_speed->setAutoClose(true);
	ddl_speed->setShowCurrentSelected(true);
	gui->addLabel("");

	gui->addLabel("VOLUME");
	gui->addLabel("Joint:", OFX_UI_FONT_SMALL);
	ddl_joints_volume = gui->addDropDownList("Select joint for volume", joints);
	ddl_joints_volume->setAllowMultiple(false);
    ddl_joints_volume->setAutoClose(true);
	ddl_joints_volume->setShowCurrentSelected(true);
	gui->addLabel("Controller:", OFX_UI_FONT_SMALL);
	ddl_volume = gui->addDropDownList("Select volume controller", controllers);
	ddl_volume->setAllowMultiple(false);
    ddl_volume->setAutoClose(true);
	ddl_volume->setShowCurrentSelected(true);
	gui->addLabel("");

	gui->addLabel("PANNING");
	gui->addLabel("Joint:", OFX_UI_FONT_SMALL);
	ddl_joints_pan = gui->addDropDownList("Select joint for panning", joints);
	ddl_joints_pan->setAllowMultiple(false);
    ddl_joints_pan->setAutoClose(true);
	ddl_joints_pan->setShowCurrentSelected(true);
	gui->addLabel("Controller:");
	ddl_pan = gui->addDropDownList("Select panning controller", controllers);
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
			else
				if(name == "Select joint for speed"){
					ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
					vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
					for(int i = 0; i < selected.size(); i++){
						cout << "SPEED JOINT: " << selected[i]->getName() << endl;
			
						if(selected[i]->getName() == "Head")
							joint_Speed = 0;
						else
							if(selected[i]->getName() == "Neck")
								joint_Speed = 1;
							else
								if(selected[i]->getName() == "Left Shoulder")
									joint_Speed = 2;
								else
									if(selected[i]->getName() == "Right Shoulder")
										joint_Speed = 3;
									else
										if(selected[i]->getName() == "Left Elbow")
											joint_Speed = 4;
										else
											if(selected[i]->getName() == "Right Elbow")
												joint_Speed = 5;
											else
												if(selected[i]->getName() == "Left Hand")
													joint_Speed = 6;
												else
													if(selected[i]->getName() == "Right Hand")
														joint_Speed = 7;
													else
														if(selected[i]->getName() == "Torso")
															joint_Speed = 8;
														else
															if(selected[i]->getName() == "Left Hip")
																joint_Speed = 9;
															else
																if(selected[i]->getName() == "Right Hip")
																	joint_Speed = 10;
																else
																	if(selected[i]->getName() == "Left Knee")
																		joint_Speed = 11;
																	else
																		if(selected[i]->getName() == "Right Knee")
																			joint_Speed = 12;
																		else
																			if(selected[i]->getName() == "Left Foot")
																				joint_Speed = 13;
																			else
																				if(selected[i]->getName() == "Right Foot")
																					joint_Speed = 14;
					}
				} else
					if(name == "Select joint for volume"){
						ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
						vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
						for(int i = 0; i < selected.size(); i++){
							cout << "VOLUME JOINT: " << selected[i]->getName() << endl;
			
							if(selected[i]->getName() == "Head")
								joint_Volume = 0;
							else
								if(selected[i]->getName() == "Neck")
									joint_Volume = 1;
								else
									if(selected[i]->getName() == "Left Shoulder")
										joint_Volume = 2;
									else
										if(selected[i]->getName() == "Right Shoulder")
											joint_Volume = 3;
										else
											if(selected[i]->getName() == "Left Elbow")
												joint_Volume = 4;
											else
												if(selected[i]->getName() == "Right Elbow")
													joint_Volume = 5;
												else
													if(selected[i]->getName() == "Left Hand")
														joint_Volume = 6;
													else
														if(selected[i]->getName() == "Right Hand")
															joint_Volume = 7;
														else
															if(selected[i]->getName() == "Torso")
																joint_Volume = 8;
															else
																if(selected[i]->getName() == "Left Hip")
																	joint_Volume = 9;
																else
																	if(selected[i]->getName() == "Right Hip")
																		joint_Volume = 10;
																	else
																		if(selected[i]->getName() == "Left Knee")
																			joint_Volume = 11;
																		else
																			if(selected[i]->getName() == "Right Knee")
																				joint_Volume = 12;
																			else
																				if(selected[i]->getName() == "Left Foot")
																					joint_Volume = 13;
																				else
																					if(selected[i]->getName() == "Right Foot")
																						joint_Volume = 14;
						}
					} else
						if(name == "Select joint for panning"){
							ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
							vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
							for(int i = 0; i < selected.size(); i++){
								cout << "PANNING JOINT: " << selected[i]->getName() << endl;
			
								if(selected[i]->getName() == "Head")
									joint_Pan = 0;
								else
									if(selected[i]->getName() == "Neck")
										joint_Pan = 1;
									else
										if(selected[i]->getName() == "Left Shoulder")
											joint_Pan = 2;
										else
											if(selected[i]->getName() == "Right Shoulder")
												joint_Pan = 3;
											else
												if(selected[i]->getName() == "Left Elbow")
													joint_Pan = 4;
												else
													if(selected[i]->getName() == "Right Elbow")
														joint_Pan = 5;
													else
														if(selected[i]->getName() == "Left Hand")
															joint_Pan = 6;
														else
															if(selected[i]->getName() == "Right Hand")
																joint_Pan = 7;
															else
																if(selected[i]->getName() == "Torso")
																	joint_Pan = 8;
																else
																	if(selected[i]->getName() == "Left Hip")
																		joint_Pan = 9;
																	else
																		if(selected[i]->getName() == "Right Hip")
																			joint_Pan = 10;
																		else
																			if(selected[i]->getName() == "Left Knee")
																				joint_Pan = 11;
																			else
																				if(selected[i]->getName() == "Right Knee")
																					joint_Pan = 12;
																				else
																					if(selected[i]->getName() == "Left Foot")
																						joint_Pan = 13;
																					else
																						if(selected[i]->getName() == "Right Foot")
																							joint_Pan = 14;
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

		/*0*/	headPos = ofPoint(head.getPosition().x + ofGetWidth()/2, -(head.getPosition().y - ofGetHeight()/2), -head.getPosition().z/Zintensity);
		/*1*/	neckPos = ofPoint(neck.getPosition().x + ofGetWidth()/2, -(neck.getPosition().y - ofGetHeight()/2), -neck.getPosition().z/Zintensity);
		/*2*/	handLPos = ofPoint(handL.getPosition().x + ofGetWidth()/2, -(handL.getPosition().y - ofGetHeight()/2), -handL.getPosition().z/Zintensity);
		/*3*/	elbowLPos = ofPoint(elbowL.getPosition().x + ofGetWidth()/2, -(elbowL.getPosition().y - ofGetHeight()/2), -elbowL.getPosition().z/Zintensity);
		/*4*/	handRPos = ofPoint(handR.getPosition().x + ofGetWidth()/2, -(handR.getPosition().y - ofGetHeight()/2), -handR.getPosition().z/Zintensity);
		/*5*/	elbowRPos = ofPoint(elbowR.getPosition().x + ofGetWidth()/2, -(elbowR.getPosition().y - ofGetHeight()/2), -elbowR.getPosition().z/Zintensity);
		/*6*/	shoulderLPos = ofPoint(shoulderL.getPosition().x + ofGetWidth()/2, -(shoulderL.getPosition().y - ofGetHeight()/2), -shoulderL.getPosition().z/Zintensity);
		/*7*/	shoulderRPos = ofPoint(shoulderR.getPosition().x + ofGetWidth()/2, -(shoulderR.getPosition().y - ofGetHeight()/2), -shoulderR.getPosition().z/Zintensity);
		/*8*/	torsoPos = ofPoint(torso.getPosition().x + ofGetWidth()/2, -(torso.getPosition().y - ofGetHeight()/2), -torso.getPosition().z/Zintensity);
		/*9*/	hipLPos = ofPoint(hipL.getPosition().x + ofGetWidth()/2, -(hipL.getPosition().y - ofGetHeight()/2), -hipL.getPosition().z/Zintensity);
		/*10*/	hipRPos = ofPoint(hipR.getPosition().x + ofGetWidth()/2, -(hipR.getPosition().y - ofGetHeight()/2), -hipR.getPosition().z/Zintensity);
		/*11*/	kneeLPos = ofPoint(kneeL.getPosition().x + ofGetWidth()/2, -(kneeL.getPosition().y - ofGetHeight()/2), -kneeL.getPosition().z/Zintensity);
		/*12*/	kneeRPos = ofPoint(kneeR.getPosition().x + ofGetWidth()/2, -(kneeR.getPosition().y - ofGetHeight()/2), -kneeR.getPosition().z/Zintensity);
		/*13*/	footLPos = ofPoint(footL.getPosition().x + ofGetWidth()/2, -(footL.getPosition().y - ofGetHeight()/2), -footL.getPosition().z/Zintensity);
		/*14*/	footRPos = ofPoint(footR.getPosition().x + ofGetWidth()/2, -(footR.getPosition().y - ofGetHeight()/2), -footR.getPosition().z/Zintensity);

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
