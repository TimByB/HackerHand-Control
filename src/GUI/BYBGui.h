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

#include "button.h"
#include "BYB_Constants.h"
#define MARGIN 3

#include "peakData.h"
#include "BYBAccuracyGui.h"
#include "BYBCalibrationGui.h"

#include "handFingers.h"

class mainAppController;


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

    void opacityChanged(int& i);
	void selectGraph(int i);

    void update(vector<float> & v, const peakData& p);

	void drawGraphs();
	void drawGraph(int i);

	ofImage logo;

	graph1D graphs [NUM_GRAPHS];
	
	void setSizes();
	
	bool getIsCalibrating();
	
	ofxPanel gui;

    ofParameter<int> lopassSize, numSamples, overlayOpacity, releaseTime;
    ofParameter<bool> bUseLoPass;
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
	
	shared_ptr<mainAppController> controllerPtr;
	
	
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
	
    bool bNewPeak;
    
    int selectedGraph;
    bool movingFinger[5];
	int selectedFinger;
};