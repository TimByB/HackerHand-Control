//
//  loPass.h
//  FingersReadings
//
//  Created by Roy Macdonald on 12-05-15.
//
//

#pragma once

static float loPass(vector<float>& data, int lopassSize){
	float sum = 0;
	int total = data.size();
	for (int j= total - lopassSize ; j < total; j++) {
		sum += data[j];
	}
	return sum/lopassSize;
}
