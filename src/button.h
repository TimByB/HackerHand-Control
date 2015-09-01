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
    button():name(""), bIsOver(false),bIsPressed(false),bSelected(false), bgColor(70), nameColor(10), hlColor(ofColor::yellow),selColor(ofColor::cyan){
#ifdef USE_SHARED_FONTS
        font(NULL);
#endif
        ofRegisterMouseEvents(this, OF_EVENT_ORDER_APP-2);
	}
	~button(){
		ofUnregisterMouseEvents(this, OF_EVENT_ORDER_APP-2);
	}
	void setFont(ofTrueTypeFont& f){
#ifdef USE_SHARED_FONTS
		font = &f;
#else
        font = f;
#endif
	}
	void draw(){
		ofPushStyle();
		if (bIsPressed) {
			ofSetColor(hlColor);
        }else if(bSelected){
            ofSetColor(selColor);
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
#ifdef USE_SHARED_FONTS
		if (font) {
			ofRectangle r = font->getStringBoundingBox(name, 0, 0);
#else
            ofRectangle r = font.getStringBoundingBox(name, 0, 0);
#endif
			ofRectangle r0 = r;
			r.alignTo(*this);
			//			ofPushStyle();
			//			ofSetColor(255);
			//			ofNoFill();
			//			ofDrawRectangle(r);
			//			ofPopStyle();
#ifdef USE_SHARED_FONTS
            font->drawString(name, r.x, r.y - r0.y);
		}
#else
            font.drawString(name, r.x, r.y - r0.y);
#endif
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
    void setSelected(bool s){
        bSelected = s;
    }
    bool getIsSelected(){return bSelected;}
	void mouseScrolled(ofMouseEventArgs & args){}
    void mouseEntered(ofMouseEventArgs & args){}
	void mouseExited(ofMouseEventArgs & args){}
    ofEvent<void> clickedEvent;
	string name;
	ofColor bgColor, nameColor, hlColor, selColor;
#ifdef USE_SHARED_FONTS
	ofTrueTypeFont * font;
#else
    ofTrueTypeFont font;
#endif
    
    bool bIsOver, bIsPressed, bSelected;
	
};
