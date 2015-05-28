#pragma once

#include "ofMain.h"
//#define USE_SVM
#ifdef USE_SVM
#include "FingerClassifierSVM.h"
#else
#include "FingersClassifierSimple.h"
#endif
#include "serialManager.h"

class BYBGui;

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
		
	void loadFingerProfile();
	void saveFingerProfile();

	void startCalibration();
	
	const vector<vector<float> >& getOriginalData(){return originalData;}
	vector<vector<float> > originalData;
	
	void newSerialData(vector<unsigned int> & d);
	
#ifdef USE_SVM
    FingersClassifierSVM classifier;
#else
	FingersClassifierSimple classifier;
#endif
	
	serialManager serial;

	shared_ptr<BYBGui> gui;
	
};

