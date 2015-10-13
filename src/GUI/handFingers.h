//
//  handFingers.h
//  FingersReadings
//
//  Created by Roy Macdonald on 21-05-15.
//
//
#pragma once
#include "ofMain.h"


class handFingers: public ofRectangle{
public:
	//----------------------------------------------------------------------
	handFingers(){}
	//----------------------------------------------------------------------
	void setup(){
		hand.load("handImg/hand.png");
		handFilled.load("handImg/hand_filled.png");
		this->set(0,0,hand.getWidth(), hand.getHeight());
		for (int i =0; i < 5; i++) {
			fingers[i].load("handImg/fingers/"+ofToString(i)+".png");
		}
	}
	//----------------------------------------------------------------------
	void draw(ofColor fingerColor = ofColor::red, int fingerAlpha=255){
		drawAt(*this, fingerColor, fingerAlpha);
	}
	//----------------------------------------------------------------------
	void drawAt(ofRectangle& r, ofColor fingerColor,int fingerAlpha=255){
		ofPushStyle();
		ofSetColor(255);
		handFilled.draw(r);
		if (selectedFinger >= 0 && selectedFinger < 5) {
			ofSetColor(fingerColor, fingerAlpha);
			fingers[selectedFinger].draw(r);
		}
		ofSetColor(255);
		hand.draw(r);
		ofPopStyle();
	}
	//----------------------------------------------------------------------
	void selectFinger(int i ){
		selectedFinger = i;
	}
	//----------------------------------------------------------------------
	ofImage hand, handFilled;
	ofImage fingers [5];
	
	int selectedFinger;
	
};