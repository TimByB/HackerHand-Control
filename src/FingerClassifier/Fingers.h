#pragma once

#include "ofMain.h"

class Fingers {
public:
    Fingers(string description = "", int ID = 0):bIsFingerClosed(false), bAboveThresh(false){
        set(description, ID);
    }
    //-------------------------------------------------------------------------
    int getID() const{
        return ID;
    }
    //-------------------------------------------------------------------------
    void set(string description, int ID ){
        this->description = description;
        this->ID = ID;
    }
    //-------------------------------------------------------------------------
    void setDescription(string description){
        this->description = description;
    }
    //-------------------------------------------------------------------------
    string getDescription() const{
        return description;
    }
    //-------------------------------------------------------------------------
    void addSample(const vector<float>& sample){
        samples.push_back(sample);
    }
    //-------------------------------------------------------------------------
    vector<float>& getExample(unsigned int i){
        return samples[i];
    }
    //-------------------------------------------------------------------------
    unsigned int size() const{
        return samples.size();
    }
    //-------------------------------------------------------------------------
    void reset(){
        samples.clear();
    }
    
    //----------------------------------------------------------
    Fingers& operator = (Fingers& f) {
        samples.clear();
        samples = f.getSamples();
        return *this;
    }
    //-------------------------------------------------------------------------
    vector<vector<float> >& getSamples(){
        return samples;
    }
    //-------------------------------------------------------------------------
    void save(string filename) const{
        ofXml x = convertToXml();
        x.save(filename);
    }
    //-------------------------------------------------------------------------
    ofXml convertToXml() const {
        ofXml xml;
        xml.addChild("FingerProfile");
        xml.setTo("FingerProfile");
        for(int i = 0; i < size(); i++) {
            xml.addChild("sample");
            int n = xml.getNumChildren("sample");
            xml.setTo("sample[" + ofToString(i) + "]");
            for (int j = 0; j < samples[i].size(); j++) {
                xml.addValue("f"+ofToString(j), ofToString(samples[i][j]));
            }
            xml.setToParent();
            //cout << xml.getName() << endl;
        }
        xml.addValue("Description", description);
        xml.addValue("ID", ID);
        return xml;
    }
    //-------------------------------------------------------------------------
    void load(ofXml& xml){
            if (xml.exists("FingerProfile")) {
                xml.setTo("FingerProfile[0]");
                do {
                    if(xml.getName() == "FingerProfile"){
                        if(xml.getValue<int>("ID") == ID){
                            samples.clear();
                            if(xml.setTo("sample[0]")){
                                do {
                                    vector<float>s;
                                    s.resize(5);
                                    for (int j = 0; j < 5; j++) {
                                        if(xml.exists("f"+ofToString(j))){
                                            s[j] = xml.getFloatValue("f"+ofToString(j));
                                        }
                                    }
                                    addSample(s);
                                }
                                while(xml.setToSibling()); // go the next PT
                                xml.setToParent();
                            }
                            break;
                        }
                    }
                }
                while( xml.setToSibling() ); // go to the next STROKE
                xml.setToParent();
            }
        }
    //-------------------------------------------------------------------------
    void load(string filename, bool bNormalizaOnLoad = false){
        samples.clear();
        ofXml xml;
        if(xml.load(filename)){
            xml.setTo("FingerProfile");
            int c = xml.getNumChildren("sample");
            xml.setTo("sample[0]");
            bool bNeedsNormalize = false;
            for (int i = 0; i < c; i++) {
                vector<float>s;
                s.resize(5);
                for (int j = 0; j < 5; j++) {
                    if(xml.exists("f"+ofToString(j))){
                        s[j] = xml.getFloatValue("f"+ofToString(j));
                    }
                }
                addSample(s);
                xml.setToSibling();
            }
            if (bNeedsNormalize && bNormalizaOnLoad) {
                float mx = 0;
                for (int i = 0; i < samples.size(); i++) {
                    for (int j = 0; j < samples[i].size(); j++) {
                        if (mx < samples[i][j]) {
                            mx = samples[i][j];
                        }
                    }
                }
                for (int i = 0; i < samples.size(); i++) {
                    for (int j = 0; j < samples[i].size(); j++) {
                        samples[i][j] /= mx;
                    }
                }
                
            }
        }
        /*
         for (int i = 0; i < size(); i++) {
         for(int j = 0; j < 5; j++){
         cout <<  samples[i][j] << ", ";
         }
         cout << endl;
         }
         //*/
    }
    //-------------------------------------------------------------------------
    void normalizeSamples(){
        float mx = 0;
        for (int i = 0; i < samples.size(); i++) {
            for (int j = 0; j < samples[i].size(); j++) {
                if (mx < samples[i][j]) {
                    mx = samples[i][j];
                }
            }
        }
        for (int i = 0; i < samples.size(); i++) {
            for (int j = 0; j < samples[i].size(); j++) {
                samples[i][j] /= mx;
            }
        }
    }
    //-------------------------------------------------------------------------
    bool update(float f, float thresh, int releaseTime){
        bool r = false;
        if(bIsFingerClosed){
            setMax(f);
            if( thresh > ofMap(f, floor, lastPeak, 0, 1)){
                if (bAboveThresh) {
                    bAboveThresh = false;
                    releaseStartTime = ofGetElapsedTimeMillis();
                }else if (ofGetElapsedTimeMillis() - releaseStartTime > releaseTime){
                    bIsFingerClosed = false;
                    r = true;
                }
            }else{
                bAboveThresh =true;
            }
        }
        return r;
    }
    //-------------------------------------------------------------------------
    void setMax(float f){
        if (max < f) {
            max = f;
        }
    }
    //-------------------------------------------------------------------------
    bool getIsFingerClosed(){
        return bIsFingerClosed;
    }
    //-------------------------------------------------------------------------
    void closeFinger(float p){
        bIsFingerClosed = true;
        bAboveThresh = false;
        lastPeak =p;
    }
    float max, floor, lastPeak;
protected:
    string description;
    vector<vector<float> > samples;
    int ID;
    bool bIsFingerClosed, bAboveThresh;
    unsigned long long releaseStartTime;
};
