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

class peakDetector{
public:


    
    bool updatePeakDetection(bool bRecalculateAll = true);
    bool searchForPeak(int i, int& k, float& pat, float& pdt);

    
    vector<int>peaks[NUM_GRAPHS];
    bool bNewPeak;

    
    void peakDetSizeChanged(int & i);
    void peakParamsChanged(float & f);
    void slopeThresholdChanged(float & f);
    ofParameterGroup parameters;
    ofParameter<int>  peakDetSize, pinkyPeakDetSize;
    ofParameter<float> slopeThreshold, peakAtkThresh, peakDcyThresh, pinkyPeakAtkThresh, pinkyPeakDcyThresh,releaseThreshold;
    peakData& getLastPeak();
    peakData lastPeak;

};