#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "NiTE.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void windowResized(int w, int h);
		
		void setupKinect(), updateKinectData();
		ofPoint headPos, neckPos, handLPos, elbowLPos, handRPos,
			elbowRPos, shoulderLPos, shoulderRPos, torsoPos, hipLPos, hipRPos,
			kneeLPos, kneeRPos, footLPos, footRPos, centerPos;

		ofPoint marker;

		//Song
		ofSoundPlayer sound;
		bool isPlaying;

		//GUI
		ofxUICanvas *gui;
		ofxUIDropDownList *ddl_speed, *ddl_volume, *ddl_pan, *ddl_joints_speed, *ddl_joints_volume, *ddl_joints_pan;
		void setupGUI();
		void exit(); 
		void guiEvent(ofxUIEventArgs &e);

		vector <string> controllers, joints;
		int trackingReference_Speed, trackingReference_Volume, trackingReference_Pan;	// 0 - X-axis;	1 - Y-axis;	2 - Distance to marker;	3 - Distance to mouse
		int joint_Speed, joint_Volume, joint_Pan;	//0 - Head, 1 - Neck, 2 - Shoulder L, 3 - Shoulder R, 4 - Elbow L, 5 - Elbow R, 6 - Hand L, 7 - Hand R, 8 - Torso,
													//9 - Hip L, 10 - Hip R, 11 - Knee L, 12 - Knee R, 13 - Foot L, 14 - Foot R
		bool showSkeleton;
		void drawSkeleton();
		ofPoint dest_speed, dest_volume, dest_panning;


		//Kinect
		nite::Status niteRc;
		nite::UserTracker userTracker;
		nite::UserTrackerFrameRef userTrackerFrame;
		void updateUserState(const nite::UserData& user, unsigned long long ts);
};
