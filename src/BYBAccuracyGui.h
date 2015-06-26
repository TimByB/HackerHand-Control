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
    void startTest();
    void endTest();
	void customDraw();
    void resetTest();
    virtual void enable(bool e = true);
    void randomPressed();
    void manualPressed();
    void moveFinger(int f);
    void releaseFinger(int f);
	vector<string> text;
	string fullText [2];
	map<int, string> fingerNames;
		BYBGui* guiPtr;
    bool bIsRandom, bTestStarted;

	int testNum, totalTests, correctAnswers, currentFinger;
    
    
};