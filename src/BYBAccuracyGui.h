//
//  BYBAccuracyGui.h
//  FingersReadings
//
//  Created by Roy Macdonald on 12-05-15.
//
//

#pragma once
#include "BYBOverlayGui.h"

class BYBGui;

class BYBAccuracyGui: public BYBOverlayGui{
public:
    void setPtr(BYBGui * p);
	void setup(string lang = "en");
	void setLanguage(string lang = "en");
    void startTest(bool bRandom);
    void endTest();
	void customDraw();
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
	vector<string> text;
	string fullText [2];
	map<int, string> fingerNames;
		BYBGui* guiPtr;
    bool bIsRandom;

	int testNum, totalTests, correctAnswers, currentFinger;
    
    
};