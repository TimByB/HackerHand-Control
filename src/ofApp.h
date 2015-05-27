#pragma once

#include "ofMain.h"
#include "FingersClassifier.h"
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
	
	FingersClassifier classifier;
	
	serialManager serial;

	shared_ptr<BYBGui> gui;
	
};

