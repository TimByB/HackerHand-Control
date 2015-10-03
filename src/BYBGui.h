//
//  BYBGui.h
//  FingersReadings
//
//  Created by Roy Macdonald on 11-05-15.
//
//
#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "graph.h"
//#include "ofApp.h"
#include "button.h"
#define NUM_GRAPHS 5
#define MARGIN 3

#include "peakData.h"
#include "BYBAccuracyGui.h"
#include "BYBCalibrationGui.h"

#include "handFingers.h"

class ofApp;


class BYBGui {
public:
#ifdef USE_SHARED_FONTS
	shared_ptr<map<string,ofTrueTypeFont> > fonts;
#else
    map<string,ofTrueTypeFont> fonts;
#endif
	BYBGui():bDrawGui(false){}
    ~BYBGui(){
        fonts.clear();
    }
	void setup(string language = "en");
	void setupParameters();
	void setupButtons();
	
	void draw();
	ofRectangle guiArea, logoRect;
	button loadButton, saveButton, calibrateButton, accuracyButton, eucButton, svmButton;

	void loadButtonPressed();
	void saveButtonPressed();
	void calibrateButtonPressed();
	void accuracyButtonPressed();
    void svmButtonPressed();
    void eucButtonPressed();
    
	void loPassChangedI(int & i);
	void loPassChangedF(float & f);
	void useLoPassChanged(bool & b);
	void slopeThresholdChanged(float & f);
	void peakDetSizeChanged(int & i);
	void peakParamsChanged(float & f);
	void opacityChanged(int& i);
	void selectGraph(int i);
	
	bool update(vector<float> & v);
	

	void drawGraphs();
	void drawGraph(int i);

	bool updatePeakDetection(bool bRecalculateAll = true);
    bool searchForPeak(int i, int& k, float& pat, float& pdt);
    void updateLoPass();
	ofImage logo;

	vector<graph1D>graphs;
	vector<int>peaks[NUM_GRAPHS];

	
	void setSizes();
	
	bool bNewPeak;
	
	bool getIsCalibrating();
	
	ofxPanel gui;
	ofParameter<int> lopassSize, peakDetSize, pinkyPeakDetSize, numSamples, overlayOpacity, releaseTime;
	ofParameter<bool> bUseLoPass;
	ofParameter<float> loPassFactor, slopeThreshold, peakAtkThresh, peakDcyThresh, pinkyPeakAtkThresh, pinkyPeakDcyThresh,releaseThreshold;


	int getSelectedGraph(){return selectedGraph;}
	
	void keyPressed(ofKeyEventArgs& args);
	void keyReleased(ofKeyEventArgs& args);
	void mouseMoved(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void mouseScrolled(ofMouseEventArgs& args);
    void mouseEntered(ofMouseEventArgs & args){}
    void mouseExited(ofMouseEventArgs & args){}

    void windowResized(ofResizeEventArgs& args);
	
	shared_ptr<ofApp> controllerPtr;
	
	
	BYBAccuracyGui accuracyGui;
	BYBCalibrationGui calibrationGui;
	
	ofEvent<void>startAccuracyTest, startCalibration;
	
	peakData& getLastPeak();
	peakData lastPeak;
	
	void updateCalibrationGui(int sampleNum, int currentFinger, int totalSamples, bool bNoFingers = false);
	bool bDrawGui;
	
	handFingers handImg;
    
    void moveFinger (int & f);
    void releaseFinger (int & f);
    
    bool bAccuracyTestRunning;
    void accuracyTestEnded();
private:
		int selectedGraph;
    bool movingFinger[5];
	int selectedFinger;
};