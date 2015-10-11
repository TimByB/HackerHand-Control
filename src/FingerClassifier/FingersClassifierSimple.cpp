#include "FingersClassifierSimple.h"
//-------------------------------------------------------------------------
void FingersClassifierSimple::setup(string language, shared_ptr<BYBGui> g){
    cout << "FingersClassifierEuc::setup" << endl;
    name = "Euclidean Distance Classifier";
    ofAddListener(g->startCalibration, this, &FingersClassifierSimple::startCalibration);
	ofAddListener(g->calibrationGui.guiClosed, this, &FingersClassifierSimple::endCalibration);
    BaseFingersClassifier::setup(language, g);
    primaryFinger = 6;
}//-------------------------------------------------------------------------
void FingersClassifierSimple::startCalibration(){
    BaseFingersClassifier::startCalibration();
}
//-------------------------------------------------------------------------
void FingersClassifierSimple::endCalibration(){
    BaseFingersClassifier::endCalibration();
}
//-------------------------------------------------------------------------
double norm (const vector<float> & a1, const vector<float> & a2){
	if (a1.size() != a2.size()) {
		return 0;
	}
	double sum =0;
	for (int i = 0; i < a1.size(); i++) {
		float d = a1[i] - a2[i];
		sum += d * d;
	}
	return sqrt(sum);
}
//-------------------------------------------------------------------------
unsigned int FingersClassifierSimple::classify(const vector<float>& sample) {
	if (!bCalibrating) {
  		vector<float> cur(sample);
		int n = size();
		probability.resize(n);
		if(n == 0) {
			return 0;
		}
		vector<vector<double> > val(n);
		double sum = 0;
		for(int i = 0; i < n; i++){
			int m = fingers[i].size();
			for(int j = 0; j < m; j++){
				double v = norm(cur, fingers[i].getExample(j));
				double p = exp(-v * v / sigma);
				val[i].push_back(p);
				sum += p;
			}
		}
		for(int i = 0; i < n; i++){
			probability[i] = 0;
			int m = fingers[i].size();
			for(int j = 0; j < m; j++) {
				probability[i] += val[i][j];
			}
			probability[i] /= sum;
		}
        
        return  getPrimaryFinger();
        
	}
	return 0;
}
//-------------------------------------------------------------------------
unsigned int FingersClassifierSimple::getPrimaryFinger() const {
	int maxExpression = -1;
	double maxProbability = 0;
	for(int i = 0; i < probability.size(); i++) {
		double cur = getProbability(i);
		if(cur > maxProbability) {
			maxExpression = i;
			maxProbability = cur;
		}
	}
	return maxExpression;
}
//-------------------------------------------------------------------------
double FingersClassifierSimple::getProbability(unsigned int i) const {
	if(i < probability.size()) {
		return probability[i];
	} else {
		return 0;
	}
}
//-------------------------------------------------------------------------
void FingersClassifierSimple::setSigma(double sigma) {
	this->sigma = sigma;
}
//-------------------------------------------------------------------------
double FingersClassifierSimple::getSigma() const {
	return sigma;
}

//-------------------------------------------------------------------------
void FingersClassifierSimple::addSample(const vector<float>& sample, int index) {
	if(fingers.size() > index){
		fingers[index].addSample(sample);
	}
}
