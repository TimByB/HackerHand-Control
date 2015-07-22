#pragma once

#include "ofMain.h"
#include "FingerClassifierSVM.h"
#include "FingersClassifierSimple.h"
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
	vector<vector<float> > loPassData;
	void newSerialData(vector<unsigned int> & d);
	
	
	serialManager serial;

	shared_ptr<BYBGui> gui;
    
    
    BaseFingersClassifier* getClassifier();

    void setClassifier(int i);
    
#ifdef DEBUG_CLASSIFIERS
    void gotClassifyEvent();
    bool bIsClassifying;
#endif
    
protected:
    
    BaseFingersClassifier * currentClassifier;
    
    FingersClassifierSVM classifierSVM;
    FingersClassifierSimple classifierEuc;
    
};

