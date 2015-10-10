//
//  peakDetector.h
//  BYB_HackerHand_Control
//
//  Created by Roy Macdonald on 03-10-15.
//
//

#pragma once
#include "ofMain.h"
#include "peakData.h"
#include "BYB_constants.h"

class peakDetector{
public:

    peakDetector();
    void setup();
    peakData update(vector<vector<float> >& data);
    
    

    bool searchForPeak(vector<vector<float> >& data, int i, int& k,int pds, float& pat, float& pdt);
    
    bool bRecalculateAll;
    void peakDetSizeChanged(int & i);
    void peakParamsChanged(float & f);

    ofParameterGroup parameters;
    ofParameter<int>  peakDetSize, pinkyPeakDetSize;
    ofParameter<float> slopeThreshold, peakAtkThresh, peakDcyThresh, pinkyPeakAtkThresh, pinkyPeakDcyThresh,releaseThreshold;
    peakData& getLastPeak();
    peakData lastPeak;

};