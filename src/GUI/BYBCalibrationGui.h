//
//  BYBCalibrationGui.h
//  FingersReadings
//
//  Created by Roy Macdonald on 12-05-15.
//
//

#pragma once
#include "BYBOverlayGui.h"

class BYBGui;

class BYBCalibrationGui: public BYBOverlayGui{
public:
	BYBCalibrationGui();
	void setPtr(BYBGui * p);
	~BYBCalibrationGui();
	void setLanguage(string lang = "en");
	void update(int sampleNum, int currentFinger, int totalSamples, bool bNoFingers);
	virtual void customDraw();
	
	
	BYBGui* guiPtr;
	map<int, string> fingerNames;
	vector<string> text;
	string fullText [2];
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