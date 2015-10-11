//
//  FingerClassifierSVM.cpp
//  FingersReadings
//
//  Created by Roy Macdonald on 27-05-15.
//
//

#include "FingerClassifierSVM.h"


//-------------------------------------------------------------------------
void FingersClassifierSVM::setup(string language, shared_ptr<BYBGui> g){
    cout << "FingersClassifierSVM::setup" << endl;
    name = "Suport Vector Machine Classifier";
    ofAddListener(g->startCalibration, this, &FingersClassifierSVM::startCalibration);
    ofAddListener(g->calibrationGui.guiClosed, this, &FingersClassifierSVM::endCalibration);
    BaseFingersClassifier::setup(language, g);
    
}
//-------------------------------------------------------------------------
void FingersClassifierSVM::load(string directory){
    BaseFingersClassifier::load(directory, true);
    classifier.clearTrainingInstances();
    for (int i = 0; i < fingers.size(); i++) {
        for (int j =0 ; j < fingers[i].size(); j++) {
            vector<double> sample;
            vector<float>& sf = fingers[i].getExample(j);
            for (int k = 0; k < sf.size(); k++) {
                sample.push_back((double)sf[k]);
            }
            classifier.addTrainingInstance(sample , i/(float)(fingers.size()-1));
        }
    }
    classifier.train();
}
/*
//-------------------------------------------------------------------------
void FingersClassifierSVM::update(vector<vector<float> >& data, vector<float>& loPass, peakData& p){
    BaseFingersClassifier::update(data, loPass, p);
}
//*/
//-------------------------------------------------------------------------
void FingersClassifierSVM::startCalibration(){
    BaseFingersClassifier::startCalibration();
    // cout << "classifier.clearTrainingInstances" << endl;
    classifier.clearTrainingInstances();
    tempSamples.clear();
}
//-------------------------------------------------------------------------
void FingersClassifierSVM::endCalibration(){
    normalizeAndAddToClassifier();
    BaseFingersClassifier::endCalibration();
    cout << "FingersClassifierSVM::endCalibration()" << endl;
    //cout << "trainClassifier";
    classifier.train();
    //    ofSystemAlertDialog("CALIBRATION ENDED");
    
}
//-------------------------------------------------------------------------
unsigned int FingersClassifierSVM::classify(const vector<float>& sample) {
    if (!bCalibrating) {
        uint64_t t = ofGetElapsedTimeMicros();
        if (t- lastClassifyTime > minClassifyTime) {
            lastClassifyTime = t;
            
            vector<double> sampleD;
            for (int i = 0; i < sample.size(); i++) {
                sampleD.push_back((double)sample[i]/maxAll);
            }
            double p =classifier.predict(sampleD);
            int pf = (int)(5*p);
           // cout << name << " classify: " << pf << endl;
            
            primaryFinger = pf;
            
#ifdef DEBUG_CLASSIFIERS
            ofNotifyEvent( classifyEvent, this);
#endif
            
            return primaryFinger;
        }
    }
    return 0;
}
//-------------------------------------------------------------------------
unsigned int FingersClassifierSVM::getPrimaryFinger() const {
    return primaryFinger;
}
//-------------------------------------------------------------------------
void FingersClassifierSVM::normalizeAndAddToClassifier(){
    for (map<int, vector< vector <float> > >::iterator it = tempSamples.begin(); it != tempSamples.end(); ++it){
        int index = it->first;
        for (int j = 0; j < it->second.size(); j++) {
            vector<float>&  sample = it->second[j];
            vector<float>sampleNorm;
            vector<double> sampleD;
            for (int i = 0; i < sample.size(); i++) {
                double s = sample[i]/maxAll;
                sampleNorm.push_back((float)s);
                sampleD.push_back(s);
            }
            if(fingers.size() > index){
                fingers[index].addSample(sampleNorm);
            }
            classifier.addTrainingInstance(sampleD, index/5.0f);
        }
    }
}
//-------------------------------------------------------------------------
void FingersClassifierSVM::addSample(const vector<float>& sample, int index) {
    
    
    tempSamples[index].push_back(sample);
    /*
     vector<float>sampleNorm;
     vector<double> sampleD;
     for (int i = 0; i < sample.size(); i++) {
     double s = sample[i]/maxAll;
     sampleNorm.push_back((float)s);
     sampleD.push_back(s);
     }
     if(fingers.size() > index){
     fingers[index].addSample(sampleNorm);
     }
     classifier.addTrainingInstance(sampleD, index/5.0f);
     //*/
}
