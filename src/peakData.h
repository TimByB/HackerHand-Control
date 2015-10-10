//
//  peakData.h
//  FingersReadings
//
//  Created by Roy Macdonald on 12-05-15.
//
//

#pragma once

class peakData{
public:
    peakData():bPeakFound(false), peakDetSize(1){}
	peakData(vector<float>& d, vector<int>& i){
		data = d;
		peakIndices = i;
	}
	void clear(){
		data.clear();
		peakIndices.clear();
        bPeakFound = false;
	}
	vector<float>data;
	vector<int>peakIndices;
    bool bPeakFound;
    int peakDetSize;
};