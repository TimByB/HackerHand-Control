#include "ofApp.h"
#include "ofxModifierKeys.h"
#include "BYBGui.h"
#include "loPass.h"
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
//	ofSetLogLevel(OF_LOG_VERBOSE);
	ofXml xml;
	string lang = "en";
	if (xml.load(ofToDataPath("lang.xml"))) {
		if(xml.exists("//language")){
			lang = xml.getValue("//language");
		}else cout << "lang.xml error" << endl;
	}

	gui->setup(lang);
	classifier.setup(lang, gui);
	originalData.resize(NUM_GRAPHS);
	for (int i = 0; i < NUM_GRAPHS; i++) {
		originalData[i].resize(ofGetWidth());
	}
	serial.setup();
	
	ofAddListener(serial.newDataEvent, this, &ofApp::newSerialData);

}
//--------------------------------------------------------------
void ofApp::newSerialData(vector<unsigned int> & d){
	vector<float>lp;
	lp.resize(NUM_GRAPHS, 0.0f);
	for (int i = 0; i < d.size() && i < NUM_GRAPHS; i++) {
		memcpy(originalData[i].data(), originalData[i].data()+1, (originalData[i].size()-1)*sizeof(float));
		originalData[i].back() = d[i];
		lp[i] = loPass(originalData[i], gui->lopassSize);
	}
	classifier.update(originalData, lp);
	if(gui->update(lp)){//if has peaks
		classifier.classify(lp);
		classifier.updatePeak(gui->getLastPeak());
	}
}
//--------------------------------------------------------------
void ofApp::startCalibration(){
	classifier.startCalibration();
}
//--------------------------------------------------------------
void ofApp::update(){
	serial.update();
}
//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	gui->draw();
}
//--------------------------------------------------------------
void ofApp::saveFingerProfile(){
	ofFileDialogResult res = ofSystemLoadDialog("Choose folder for saving profile", true, ofToDataPath("profiles"));//, false);
	if (res.bSuccess) {
		classifier.save(res.getPath());
	}
}
//--------------------------------------------------------------
void ofApp::loadFingerProfile(){
	ofFileDialogResult res = ofSystemLoadDialog("Select profile folder", true, ofToDataPath("profiles"));//, false);
	if (res.bSuccess) {
		ofFile f(res.getPath());
		if (f.isDirectory()) {
			classifier.load(res.getPath());
		}
	}
}



