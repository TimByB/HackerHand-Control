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

#define TEST_PEAK_DET_CLASS

class BYBGui {
public:
    map<string,ofTrueTypeFont> fonts;
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
#ifndef TEST_PEAK_DET_CLASS
	void peakDetSizeChanged(int & i);
	void peakParamsChanged(float & f);
#endif
    void opacityChanged(int& i);
	void selectGraph(int i);
#ifdef TEST_PEAK_DET_CLASS
    bool update(vector<float> & v, const peakData& p);
#else
    bool update(vector<float> & v);
#endif

	void drawGraphs();
	void drawGraph(int i);

#ifndef TEST_PEAK_DET_CLASS
	bool updatePeakDetection(bool bRecalculateAll = true);
    bool searchForPeak(int i, int& k, float& pat, float& pdt);
#endif
    void updateLoPass();
	ofImage logo;

	graph1D graphs [NUM_GRAPHS];
#ifndef TEST_PEAK_DET_CLASS
	vector<int>peaks[NUM_GRAPHS];
    bool bNewPeak;
    peakData& getLastPeak();
    peakData lastPeak;
#endif
	
	void setSizes();
	
	bool getIsCalibrating();
	
	ofxPanel gui;

    ofParameter<int> lopassSize, numSamples, overlayOpacity, releaseTime;
    ofParameter<bool> bUseLoPass;
#ifndef TEST_PEAK_DET_CLASS
    ofParameter<int> peakDetSize, pinkyPeakDetSize;
    ofParameter<float> peakAtkThresh, peakDcyThresh, pinkyPeakAtkThresh, pinkyPeakDcyThresh,
#endif
    ofParameter<float> loPassFactor, slopeThreshold, releaseThreshold;


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