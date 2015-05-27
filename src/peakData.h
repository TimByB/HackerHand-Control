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
	peakData(){}
	peakData(vector<float>& d, vector<int>& i){
		data = d;
		peakIndices = i;
	}
	void clear(){
		data.clear();
		peakIndices.clear();
	}
	vector<float>data;
	vector<int>peakIndices;
};