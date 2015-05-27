//
//  BYBAccuracyGui.h
//  FingersReadings
//
//  Created by Roy Macdonald on 12-05-15.
//
//

#pragma once
#include "BYBOverlayGui.h"


class BYBAccuracyGui: public BYBOverlayGui{
public:
	
	
	void setLanguage(string lang = "en"){}
	virtual void customDraw(){}
	/*
	 virtual void close();
	 virtual bool keyPressed(ofKeyEventArgs& args);
	 virtual bool keyReleased(ofKeyEventArgs& args);
	 virtual bool mouseMoved(ofMouseEventArgs& args);
	 virtual bool mouseDragged(ofMouseEventArgs& args);
	 virtual bool mousePressed(ofMouseEventArgs& args);
	 virtual bool mouseReleased(ofMouseEventArgs& args);
	 virtual bool mouseScrolled(ofMouseEventArgs& args);
	 //*/
	
	
};