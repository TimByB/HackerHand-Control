#pragma once

#include "ofMain.h"
#include "FingerClassifierSVM.h"
#include "FingersClassifierSimple.h"
#include "serialManager.h"
#include "peakDetector.h"

class BYBGui;

class mainAppController : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
		
	void loadFingerProfile();
	void saveFingerProfile();

	void startCalibration();
	
	const vector< vector<float> >& getOriginalData(){return originalData;}
	vector< vector<float> > originalData;
    vector< shared_ptr<vector<float> > > graphData;
	vector<vector<float> > loPassData;
  	void newSerialData(vector<unsigned int> & d);
	
	
	serialManager serial;

	shared_ptr<BYBGui> gui;
    
    
    BaseFingersClassifier* getClassifier();

    void setClassifier(int i);
    
    peakDetector peakDet;
    
#ifdef DEBUG_CLASSIFIERS
    void gotClassifyEvent();
    bool bIsClassifying;
#endif
    
protected:
    
    BaseFingersClassifier * currentClassifier;
    
    FingersClassifierSVM classifierSVM;
    FingersClassifierSimple classifierEuc;
    
};

