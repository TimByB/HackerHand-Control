//
//  BaseFingerClassifier.cpp
//  FingersReadings
//
//  Created by Roy Macdonald on 27-05-15.
//
//

#include "BaseFingerClassifier.h"

//-------------------------------------------------------------------------
void BaseFingersClassifier::setup(string language, shared_ptr<BYBGui> g){
	gui = g;
	ofXml xml;
    fingers.resize(6);
	if (xml.load(ofToDataPath("languages/"+language+".xml"))) {
		xml.setTo("fingers");
		int c = xml.getNumChildren("finger");
		xml.setTo("finger[0]");
		for (int i = 0; i < c; i++) {
			setFinger(xml.getValue("name"), xml.getIntValue("ID"));
			xml.setToSibling();
		}
	}
	bWaitForAverage = false;
	bWaitForPeak = false;
	bCalibrating = false;
	currentSampleNum = 0;
	numSamplesAverage = 500;
	
	reset();
    prevPeakTime =0;
    average.resize(5);
    partialAvg.resize(5);
    currentAverageSample = 0;
    maxAll = 0;
}

//-------------------------------------------------------------------------
vector<int> BaseFingersClassifier::getClassifyHist(){
    vector<int> t(classifyHist);
    classifyHist.clear();
    return t;
}
//-------------------------------------------------------------------------
void BaseFingersClassifier::updateCalibration(vector<vector<float> >& data, vector<float>& loPass, peakData& p){
	if (bWaitForAverage && bCalibrating) {
		currentSampleNum ++;
        currentAverageSample ++;
		gui->updateCalibrationGui(currentSampleNum, 5, numSamplesAverage, true);
		
        int numPerAverage = (int)(floor)(numSamplesAverage/(float)gui->numSamples);
        
        for (int i =0; i < loPass.size(); i++) {
            average[i] += loPass[i];
            partialAvg[i] += loPass[i];
        }
        if (currentAverageSample >= numPerAverage) {
            for (int i =0; i < 5; i++) {
                //               average[i] /= numPerAverage;
                partialAvg[i] /= numPerAverage;
            }
            addSample(partialAvg, 5);
            currentAverageSample = 0;
            for (int i =0; i < 5; i++) {
                partialAvg[i]=0;
            }
        }
        if (currentSampleNum > numSamplesAverage) {
            for (int i =0; i < 5; i++) {
                average[i] /= numSamplesAverage;
                fingers[i].floor = average[i];
            }
			gui->calibrationGui.close();
            //			gui->updateCalibrationGui(currentSampleNum, 5, numSamplesAverage, true);
            endCalibration();
            //			bWaitForAverage = false;
            //			bCalibrating = false;
		}
	}
    if (p.bPeakFound) {
        updatePeakCalibration(p);
    }
}
//-------------------------------------------------------------------------
void BaseFingersClassifier::updatePeakCalibration(peakData& p){
    unsigned long long t =ofGetElapsedTimeMillis();
	if (p.peakIndices.size()) {
		if (bCalibrating) {
			if (bWaitForPeak && gui->getSelectedGraph() >=0) {
    			for (int i = 0; i < p.peakIndices.size(); i++) {
					if (gui->getSelectedGraph() == p.peakIndices[i]) {
                        if (t  - prevPeakTime < 300) {
                            cout << "peak to soon: " << t - prevPeakTime<<endl ;
                            return;
                        }
                        if (currentSampleNum < gui->numSamples) {
							currentSampleNum++;
							gui->updateCalibrationGui(currentSampleNum, gui->getSelectedGraph(), gui->numSamples);
							addSample(p.data,gui->getSelectedGraph());
                            cout << "added sample: " << currentSampleNum << " of " << gui->numSamples << " finger: " << gui->getSelectedGraph() << "  dt: " << t - prevPeakTime<<endl ;
                            
							if (currentSampleNum >= gui->numSamples) {
								currentSampleNum = 0;
								int sGraph = gui->getSelectedGraph()+1;
								gui->selectGraph(sGraph);
                                gui->updateCalibrationGui(currentSampleNum, gui->getSelectedGraph(), gui->numSamples);
                                
                                if (sGraph >= NUM_GRAPHS) {
									bWaitForAverage = true;
									currentSampleNum = 0;
									cout << "FingerClassifier start average" << endl;
									gui->selectGraph(-1);
									bWaitForPeak = false;
                                    average.clear();
                                    average.resize(5,0.0f);
                                    currentAverageSample = 0;
								}
                                prevPeakTime = t;
                                return;
							}
						}
                        prevPeakTime = t;
                        return;
                    }
				}
			}
		}
	}
}
//-------------------------------------------------------------------------
void BaseFingersClassifier::update(vector<vector<float> >& data, vector<float>& loPass, peakData& p){
    if (bCalibrating) {
        updateCalibration(data, loPass, p);
    }else{
      //  cout << "clasifierUpdate ";
       // if (data.size()  >= peakOffset) {
            vector<float> v;
        for (int i= 0; i < data.size(); i++) {
            v.push_back(data[i][data[i].size() - p.peakDetSize]);
        }
        
        //   cout << c;
            if (p.bPeakFound) {
           unsigned int c = classify(v);
                if (classifyHist.size() > 100) {
                    classifyHist.clear();
                }
                classifyHist.push_back(c);
                if (c < fingers.size()) {
                    for (int i = 0; i < p.peakIndices.size(); i++) {
                        if (p.peakIndices[i] ==  c && !fingers[c].getIsFingerClosed()) {
                            fingers[c].closeFinger(p.data[c]);
                            int cc = c;
                            ofNotifyEvent(fingerMoved, cc, this);
                        }
                    }
                }
            }else{
                
            }
            for (int i =0; i < 5; i++) {
                if(fingers[i].update(data[i][data[i].size() - p.peakDetSize], gui->releaseThreshold, gui->releaseTime)){
                    ofNotifyEvent(fingerReleased, i, this);
                }
            }
            if (p.bPeakFound) {
                for (int i = 0; i < p.data.size(); i++) {
                    fingers[i].setMax(p.data[i]);
                }
                for (int i = 0; i < 5; i++) {
                    if (maxAll < fingers[i].max) {
                        maxAll = fingers[i].max;
                    }
                }
            }
        //}
    }
//cout << endl;
}
//-------------------------------------------------------------------------
void BaseFingersClassifier::save(string directory) const {
	ofDirectory dir(directory);
	dir.create(true);
	for(int i = 0; i < size(); i++) {
		string filename = dir.path() + "/" + ofToString(fingers[i].getID()) + ".xml";
		cout << "saving to " << filename << endl;
		fingers[i].save(filename);
	}
}
//-------------------------------------------------------------------------
void BaseFingersClassifier::load(string directory, bool bNormalizeOnLoad) {
	ofDirectory dir(directory);
	dir.allowExt("xml");
	dir.listDir();
	for(int i = 0; i < dir.size(); i++){
		int ind = ofToInt(dir.getName(i));
		if (ind >= 0 && ind < fingers.size()) {
            fingers[ind].reset();
			fingers[ind].load(dir.getPath(i), bNormalizeOnLoad);
		}
	}
}
//-------------------------------------------------------------------------
void BaseFingersClassifier::reset() {
    for (int i =0; i < fingers.size(); i++) {
        fingers[i].reset();
    }
    maxs.resize(5, FLT_MIN);
    mins.resize(5, FLT_MAX);
    average.clear();
    partialAvg.clear();
    average.resize(5, 0);
    partialAvg.resize(5, 0);
    currentSampleNum = 0;
}
//-------------------------------------------------------------------------
Fingers& BaseFingersClassifier::getFingers(unsigned int i) {
	return fingers[i];
}
//-------------------------------------------------------------------------
unsigned int BaseFingersClassifier::size() const {
	return fingers.size();
}
//-------------------------------------------------------------------------
void BaseFingersClassifier::setFinger(string description, int fingerID) {
	if(description == "") {
		description = ofToString(fingerID);
	}
	if (fingerID < fingers.size()) {
		fingers[fingerID].set(description, fingerID);
	}
}
//-------------------------------------------------------------------------
void BaseFingersClassifier::startCalibration(){
	cout << "Classifier start calibration" << endl;
    reset();
	gui->selectGraph(0);
	bCalibrating = true;
	bWaitForPeak = true;
	bWaitForAverage = false;
    gui->updateCalibrationGui(currentSampleNum, gui->getSelectedGraph(), gui->numSamples);
}
//-------------------------------------------------------------------------
void BaseFingersClassifier::endCalibration(){
    cout << "BaseFingersClassifier::endCalibration()" << endl;
	gui->selectGraph(-1);
	bCalibrating = false;
	bWaitForPeak = false;
	bWaitForAverage = false;
}

//-------------------------------------------------------------------------
void BaseFingersClassifier::copyCalibratioFrom(BaseFingersClassifier* orig){
    if(orig){
    for (int i = 0; i < orig->size(); i++) {
        fingers[i].reset();
        fingers[i] = orig->getFingers(i);
    }
    }
}

