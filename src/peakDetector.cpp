//
//  peakDetector.cpp
//  BYB_HackerHand_Control
//
//  Created by Roy Macdonald on 10-10-15.
//
//

#include "peakDetector.h"

//--------------------------------------------------------------
peakDetector::peakDetector(): bRecalculateAll(false){}
//--------------------------------------------------------------
void peakDetector::setup(){
    parameters.setName("Peak Det Params");
    parameters.add(peakDetSize.set("peakDetSize", 15, 3, 100));
    parameters.add(peakAtkThresh.set("Peak Attack Threshold", 45.91, 1, 500));
    parameters.add(peakDcyThresh.set("Peak Decay Threshold", 5.485, 1, 300));
    
    parameters.add(pinkyPeakAtkThresh.set("Pinky Peak Attack Threshold", 45.91, 1, 500));
    parameters.add(pinkyPeakDcyThresh.set("Pinky Peak Decay Threshold", 5.485, 1, 300));
    parameters.add(pinkyPeakDetSize.set("Pinky PeakDetSize", 15, 3, 100));
    
    peakAtkThresh.addListener(this, &peakDetector::peakParamsChanged);
    peakDcyThresh.addListener(this, &peakDetector::peakParamsChanged);
    peakDetSize.addListener(this, &peakDetector::peakDetSizeChanged);
    pinkyPeakAtkThresh.addListener(this, &peakDetector::peakParamsChanged);
    pinkyPeakDcyThresh.addListener(this, &peakDetector::peakParamsChanged);
    pinkyPeakDetSize.addListener(this, &peakDetector::peakDetSizeChanged);
    
}
//--------------------------------------------------------------
peakData peakDetector::update(vector<vector<float> >& data){
    bool bPeakFound = false;
    lastPeak.clear();
    lastPeak.peakDetSize = peakDetSize;
    for (int i = 0; i < NUM_GRAPHS; i++) {
        if(bRecalculateAll){
//            graphs[i].resetPeaks();
        }
        int pds =peakDetSize;
        float pat = peakAtkThresh;
        float pdt = peakDcyThresh;
        if(i == 4) {
            pds = pinkyPeakDetSize;
            pat = pinkyPeakAtkThresh;
            pdt = pinkyPeakDcyThresh;
        }
        
        if (bRecalculateAll) {
            for (int k = pds; k < data[i].size()-pds; k++) {
                if (searchForPeak(data,i, k,pds, pat, pdt)){
                    bPeakFound = true;
                }
            }
        }else{
//            lastPeak.clear();
            int k = data[i].size()-peakDetSize;
            lastPeak.data.push_back(data[i][k]);
            if (searchForPeak(data, i, k, pds, pat , pdt)) {
                lastPeak.peakIndices.push_back(i);
                lastPeak.bPeakFound = true;
            }
        }
    }
    bRecalculateAll = false;
    return lastPeak;
}
//--------------------------------------------------------------
bool peakDetector::searchForPeak(vector<vector<float> >& data, int i, int& k, int pds, float& pat, float& pdt){
    bool isMax = true;
    bool bPeakFound = false;
    
//    peakData p;
//    for (int i = 0; i < data.size(); i++) {
        for (int j = k-pds; j < k +pds; j++) {
            if (data[i][j] > data[i][k]) {
                isMax = false;
            }
        }
        if (isMax) {
            if (data[i][k] - data[i][k-pds] > pat &&
                data[i][k] - data[i][k+pds] > pdt) {
  //              p.data.push_back(data[i][k]);
                //graphs[i].addPeak(k);
                cout << "Peak Found!" << endl;
                bPeakFound = true;
            }
        }
    ///}
    return  bPeakFound;
}
//--------------------------------------------------------------
void peakDetector::peakDetSizeChanged(int & i){
    bRecalculateAll = true;
}
//--------------------------------------------------------------
void peakDetector::peakParamsChanged(float & f){
    bRecalculateAll = true;
}
//--------------------------------------------------------------
peakData& peakDetector::getLastPeak(){return lastPeak;}
//--------------------------------------------------------------
