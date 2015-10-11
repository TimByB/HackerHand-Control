//
//  AbstractFingerClassifier.h
//  FingersReadings
//
//  Created by Roy Macdonald on 27-05-15.
//
//

#pragma once

#include "ofMain.h"
#include "Fingers.h"
#include "peakData.h"
#include "BYBGui.h"

//#define DEBUG_CLASSIFIERS

class BaseFingersClassifier {
public:

	virtual void setup(string language, shared_ptr<BYBGui> g);
	virtual void save(string directory) const;
	virtual void load(string directory, bool bNormalizeOnLoad = false);
    virtual void updateCalibration(vector<vector<float> >& data, vector<float>& loPass, peakData& p);
	virtual void updatePeakCalibration(peakData& p);
	virtual unsigned int classify(const vector<float>& sample)=0;
	virtual unsigned int getPrimaryFinger() const =0;
    virtual	void update(vector<vector<float> >& data, vector<float>& loPass, peakData& p);
	
	virtual unsigned int size() const;
    virtual Fingers& getFingers(unsigned int i);
	virtual void setFinger(string description = "", int fingerID = -1);
	virtual void addSample(const vector<float>& sample, int index)=0;
	virtual void reset();
	virtual void startCalibration();
	virtual void endCalibration();
	
    ofEvent<int> fingerMoved, fingerReleased;
	
    string getName(){return name;}
    
	virtual bool isCalibrating(){return bCalibrating;}

    void copyCalibratioFrom(BaseFingersClassifier* orig);
    
    vector<int> getClassifyHist();
    
    string name;
#ifdef DEBUG_CLASSIFIERS
    ofEvent<void>classifyEvent;
#else
protected:
#endif
	vector<Fingers> fingers;

	bool bCalibrating;
	bool bWaitForPeak;
	bool bWaitForAverage;

    
	shared_ptr<BYBGui> gui;
	int currentSampleNum, numSamplesAverage;
    vector<float>maxs, mins;
    float maxAll;
    
protected:
       vector<int> classifyHist;
    unsigned long long prevPeakTime;
    vector<float> average, partialAvg;
    int currentAverageSample;
};
