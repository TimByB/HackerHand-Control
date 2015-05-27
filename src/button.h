//
//  boton.h
//  FingersReadings
//
//  Created by Roy Macdonald on 12-05-15.
//
//
#pragma once
#include "ofMain.h"

class button: public ofRectangle{
public:
	button():name(""), font(NULL), bIsOver(false),bIsPressed(false), bgColor(70), nameColor(10), hlColor(ofColor::yellow){
		ofRegisterMouseEvents(this, OF_EVENT_ORDER_APP-2);
	}
	~button(){
		ofUnregisterMouseEvents(this, OF_EVENT_ORDER_APP-2);
	}
	void setFont(ofTrueTypeFont& f){
		font = &f;
	}
	void draw(){
		ofPushStyle();
		if (bIsPressed) {
			ofSetColor(hlColor);
		}else{
			ofSetColor(bgColor);
		}
		ofDrawRectangle(*this);
		
		if(bIsOver){
			ofPushStyle();
			ofSetLineWidth(3);
			ofNoFill();
			ofSetColor(hlColor);
			ofDrawRectangle(*this);
			ofPopStyle();
		}
		ofSetColor(nameColor);
		if (font) {
			ofRectangle r = font->getStringBoundingBox(name, 0, 0);
			ofRectangle r0 = r;
			r.alignTo(*this);
			//			ofPushStyle();
			//			ofSetColor(255);
			//			ofNoFill();
			//			ofDrawRectangle(r);
			//			ofPopStyle();
			font->drawString(name, r.x, r.y - r0.y);
		}
		ofPopStyle();
	}
	void mouseMoved(ofMouseEventArgs & args){
		if (inside(args.x, args.y)) {
			bIsOver = true;
		}else{
			bIsOver = false;
		}
	}
	void mouseDragged(ofMouseEventArgs & args){}
	void mousePressed(ofMouseEventArgs & args){
		if (inside(args.x, args.y)) {
			bIsPressed = true;
		}
	}
	bool mouseReleased(ofMouseEventArgs & args){
		if (inside(args.x, args.y)) {
			ofNotifyEvent(clickedEvent, this);
			bIsPressed = false;
			return true;
		}
		bIsOver = bIsPressed = false;
		return false;
	}
	void mouseScrolled(ofMouseEventArgs & args){}
    void mouseEntered(ofMouseEventArgs & args){}
	void mouseExited(ofMouseEventArgs & args){}
    ofEvent<void> clickedEvent;
	string name;
	ofColor bgColor, nameColor, hlColor;
	ofTrueTypeFont * font;
	bool bIsOver, bIsPressed;
	
};
