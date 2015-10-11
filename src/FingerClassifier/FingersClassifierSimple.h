#pragma once

#include "BaseFingerClassifier.h"

class FingersClassifierSimple: public BaseFingersClassifier {
public:
	FingersClassifierSimple():sigma(10.0){}
	~FingersClassifierSimple(){}
    //-------------------------------------------------------------------------
    void setup(string language, shared_ptr<BYBGui> g);
	unsigned int classify(const vector<float>& sample);
	unsigned int getPrimaryFinger() const;
	double getProbability(unsigned int i) const;
	void setSigma(double sigma);
	double getSigma() const;
	void addSample(const vector<float>& sample, int index);
    void startCalibration();
	void endCalibration();
protected:
	vector<double> probability;
	float sigma;
    int primaryFinger;

};
