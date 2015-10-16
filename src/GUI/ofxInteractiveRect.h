//
//  interactiveRect.h
//  ofxScaleDragRect
//
//  Created by Roy Macdonald on 8/15/12.
//  Copyright (c) 2012 micasa. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"


#define MARGEN 10

class ofxInteractiveRect : public ofRectangle{
    
public:
    ofxInteractiveRect(string nombre = "");
    ~ofxInteractiveRect();
        
    void enableEdit(bool enable = true);
    void disableEdit();
    void toggleEdit();
    
    void draw();
    
    bool loadSettings(string nombre = "", string path = "");
    void saveSettings(string nombre = "", string path = "");
    
    
    void mouseMoved(ofMouseEventArgs & mouse);
    void mousePressed(ofMouseEventArgs & mouse);
    void mouseDragged(ofMouseEventArgs & mouse);
    void mouseReleased(ofMouseEventArgs & mouse);
    void mouseScrolled(ofMouseEventArgs & args){}
    void mouseEntered(ofMouseEventArgs & args){}
	void mouseExited(ofMouseEventArgs & args){}

    float getRectX(){return x;}
    float getRectY(){return y;}
    float getRectWidth(){return width;}
    float getRectHeight(){return height;}
    
    void setDrawHandlesOnly(bool e = true);
    void allowMove(bool a = true);
    string nombre;
    string path;
    
    ofRectangle getRect();
    void setRect(float x, float y, float width, float height){
        this->x = x; 
        this->y = y;
        this->width = width;
        this->height = height;
    }
    
    ofEvent<ofRectangle>changeEvent;
protected:
    bool bAllowMove;
    bool bIsEditing;
    bool bMove;
	bool bIsOver;
    bool bLeft, bRight, bUp, bDown;
	bool bPressed;
    bool bDrawHandlesOnly;
    ofVec2f mousePrev;
   // ofxXmlSettings settings;
   
    
};