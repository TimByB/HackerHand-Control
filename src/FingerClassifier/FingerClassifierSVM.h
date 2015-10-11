//
//  FingerClassifierSVM.h
//  FingersReadings
//
//  Created by Roy Macdonald on 27-05-15.
//
//

#pragma once

#include "BaseFingerClassifier.h"
#include "ofxLearn.h"

class FingersClassifierSVM: public BaseFingersClassifier {
public:
    FingersClassifierSVM():lastClassifyTime(0), minClassifyTime(100){};
	~FingersClassifierSVM(){};
    void setup(string language, shared_ptr<BYBGui> g);
    void load(string filename);
//	void update(vector<vector<float> >& data, vector<float>& loPass, peakData& p);
	
    unsigned int classify(const vector<float>& sample);
	unsigned int getPrimaryFinger() const;

    void addSample(const vector<float>& sample, int index);
	void endCalibration();
    void startCalibration();
    void copyCalibratioFrom(BaseFingersClassifier* orig);
protected:

    void normalizeAndAddToClassifier();
    map <int,vector< vector<float> > > tempSamples;
    uint64_t lastClassifyTime, minClassifyTime;
    ofxLearnSVM classifier;
    unsigned int primaryFinger;
    
};
