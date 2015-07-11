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
		ofxUIDropDownList *ddl_speed, *ddl_volume, *ddl_pan;
		void setupGUI();
		void exit(); 
		void guiEvent(ofxUIEventArgs &e);

		vector <string> controllers;
		int trackingReference_Speed, trackingReference_Volume, trackingReference_Pan;	// 0 - X-axis;	1 - Y-axis;	2 - Distance to marker;	3 - Distance to mouse


		//Kinect
		nite::Status niteRc;
		nite::UserTracker userTracker;
		nite::UserTrackerFrameRef userTrackerFrame;
		void updateUserState(const nite::UserData& user, unsigned long long ts);
};
