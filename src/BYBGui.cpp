//
//  BYBGui.cpp
//  FingersReadings
//
//  Created by Roy Macdonald on 11-05-15.
//
//

#include "BYBGui.h"
#include "ofApp.h"
//--------------------------------------------------------------
void BYBGui::setup( string language){
	ofXml xml;
	map<int, string> fingerNames;
	if (xml.load(ofToDataPath("languages/"+language+".xml"))) {
		xml.setTo("fingers");
		int c = xml.getNumChildren("finger");
		xml.setTo("finger[0]");
		for (int i = 0; i < c; i++) {
			fingerNames[xml.getIntValue("ID")] = xml.getValue("name");
			xml.setToSibling();
		}
		xml.setToParent();
		xml.setToParent();
		xml.setTo("buttons");
		if (xml.exists("calibrate")) {
			calibrateButton.name = xml.getValue("calibrate");
		}
		if (xml.exists("accuracy")) {
			accuracyButton.name = xml.getValue("accuracy");
		}
		if (xml.exists("save")) {
			saveButton.name = xml.getValue("save");
		}
		if (xml.exists("load")) {
			loadButton.name = xml.getValue("load");
		}

	}
	
	graphs.resize(NUM_GRAPHS);
	for (int i = 0; i < NUM_GRAPHS; i++) {
		graphs[i].setup(ofGetWidth()-5, (string)((fingerNames.count(i)>0)?fingerNames[i]:""));
		graphs[i].setFont(fonts.get());
		graphs[i].setNormalize(true);
		
	}
	setSizes();
	
	setupParameters();
	setupButtons();
	logo.load("logo.png");
	
	
	
	selectedGraph = -1;
	bNewPeak = false;
	
	ofRegisterKeyEvents(this);
	ofRegisterMouseEvents(this);
	ofAddListener(ofEvents().windowResized, this, &BYBGui::windowResized);
	
	calibrationGui.setPtr(this);
	accuracyGui.setLanguage(language);
	calibrationGui.setLanguage(language);
	accuracyGui.set(MARGIN, MARGIN, ofGetWidth() - (2*MARGIN), guiArea.height);
	calibrationGui.set(MARGIN, MARGIN, ofGetWidth() - (2*MARGIN), guiArea.height);
	
	calibrationGui.update(0, 0, numSamples, false);

	logoRect.set(guiArea.x + MARGIN,guiArea.y + MARGIN, logo.getWidth(), logo.getHeight());
	
	handImg.setup();
	
	handImg.scaleTo(guiArea , OF_ASPECT_RATIO_KEEP);
	
	handImg.x = logoRect.getMaxX() + (calibrateButton.x - logoRect.getMaxX() - handImg.getWidth())/2.0f;
	
}
//--------------------------------------------------------------
void BYBGui::setupParameters(){
	gui.setup();
	gui.setPosition(300, MARGIN);
	gui.setWidthElements(200);
	gui.add(lopassSize.set("loPass Size", 5, 1, 30));
	gui.add(bUseLoPass.set("Use Lo Pass", false));
	gui.add(loPassFactor.set("loPass Factor", 0.3, 0, 1));
	gui.add(slopeThreshold.set("slopeThreshold", 10, 0, 10 ));
	gui.add(peakDetSize.set("peakDetSize", 5, 3, 100));
	gui.add(peakAtkThresh.set("Peak Attack Threshold", 10, 1, 500));
	gui.add(peakDcyThresh.set("Peak Decay Threshold", 5, 1, 300));
	gui.add(numSamples.set("Num Samples per finger", 5, 1, 20));
	gui.add(overlayOpacity.set("overlay GUI opacity", 255, 0, 255));
	
	overlayOpacity.addListener(this, &BYBGui::opacityChanged);
	peakAtkThresh.addListener(this, &BYBGui::peakParamsChanged);
	peakDcyThresh.addListener(this, &BYBGui::peakParamsChanged);
	peakDetSize.addListener(this, &BYBGui::peakDetSizeChanged);
	slopeThreshold.addListener(this, &BYBGui::slopeThresholdChanged);
	loPassFactor.addListener(this, &BYBGui::loPassChangedF);
	bUseLoPass.addListener(this, &BYBGui::useLoPassChanged);
	lopassSize.addListener(this, &BYBGui::loPassChangedI);
	
	gui.loadFromFile("settings.xml");
	
}
//--------------------------------------------------------------
void BYBGui::setupButtons(){
	/*
	loadButton.name = "Load Profile";
	saveButton.name = "Save Profile";
	calibrateButton.name = "Calibrate";
	accuracyButton.name = "Accuracy Test";
//*/
	loadButton.font = &fonts->at ("FiraSans-Heavy");//["HelveticaNeueLTStd-Md"];
	saveButton.font = &fonts->at ("FiraSans-Heavy");//["HelveticaNeueLTStd-Md"];
	calibrateButton.font = &fonts->at ("FiraSans-Heavy");//["HelveticaNeueLTStd-Md"];
	accuracyButton.font = &fonts->at ("FiraSans-Heavy");//["HelveticaNeueLTStd-Md"];
	
	loadButton.set(guiArea.getMaxX() -200 - 10, 15 + MARGIN, 200, 40);
	saveButton.set(guiArea.getMaxX() -200 - 10, 65 + MARGIN, 200, 40);
	calibrateButton.set(loadButton.getX() - 200 - 10, 15 + MARGIN, 200, 40);
	accuracyButton.set(loadButton.getX() - 200 - 10, 65 + MARGIN, 200, 40);
	
	ofAddListener(loadButton.clickedEvent, this, &BYBGui::loadButtonPressed);
	ofAddListener(saveButton.clickedEvent, this, &BYBGui::saveButtonPressed);
	ofAddListener(calibrateButton.clickedEvent, this, &BYBGui::calibrateButtonPressed);
	ofAddListener(accuracyButton.clickedEvent, this, &BYBGui::accuracyButtonPressed);
}
//--------------------------------------------------------------
void BYBGui::slopeThresholdChanged(float & f){
	for (int i = 0; i < NUM_GRAPHS; i++) {
		graphs[i].slopeThreshold = f;
		graphs[i].updateMesh();
	}
}
//--------------------------------------------------------------
bool BYBGui::update(vector<float> & v){
	bool bPeakFound = false;
	lastPeak.clear();
	for (int i =0; i < NUM_GRAPHS && i < v.size(); i++) {
		graphs[i].update(v[i]);
		
		bool isMax = true;
		int k = graphs[i].data.size();
		for (int j = k -(2 * peakDetSize); j <  k; j++) {
			if (graphs[i].data[j] > graphs[i].data[k - peakDetSize]) {
				isMax = false;
			}
		}
		
		if (isMax) {
			if (graphs[i].data[k-peakDetSize] - graphs[i].data[k-2*peakDetSize] > peakAtkThresh &&
				graphs[i].data[k-peakDetSize] - graphs[i].data[k-1] > peakDcyThresh) {
				graphs[i].addPeak(k-peakDetSize);
				lastPeak.peakIndices.push_back(i);
				bPeakFound = true;
			}
		}
		lastPeak.data.push_back(graphs[i].data[k-peakDetSize]);
	}
	
	bNewPeak = bPeakFound;
	return bPeakFound;
}
//--------------------------------------------------------------
peakData& BYBGui::getLastPeak(){
	 return lastPeak;
}
//--------------------------------------------------------------
void BYBGui::updatePeakDetection(){
	
	for (int i = 0; i < NUM_GRAPHS; i++) {
		peaks[i].clear();
		graphs[i].resetPeaks();
		for (int k = peakDetSize; k < graphs[i].data.size()-peakDetSize; k++) {
			bool isMax = true;
			for (int j = k-peakDetSize; j < k +peakDetSize; j++) {
				if (graphs[i].data[j] > graphs[i].data[k]) {
					isMax = false;
				}
			}
			if (isMax) {
				if (graphs[i].data[k] - graphs[i].data[k-peakDetSize] > peakAtkThresh &&
					graphs[i].data[k] - graphs[i].data[k+peakDetSize] > peakDcyThresh) {
					graphs[i].addPeak(k);
					peaks[i].push_back(k);
				}
			}
		}
	}
}
//--------------------------------------------------------------
void BYBGui::updateLoPass(){
	
	const vector< vector<float> > & originalData = controllerPtr->getOriginalData();
	for (int i = 0; i < graphs.size() && i<originalData.size(); i++) {
		for (int k =0; k < originalData[i].size() && k < graphs[i].data.size(); k++) {
			if (bUseLoPass) {
				if (k > 0) {
					graphs[i].data[k] = originalData[i][k] * loPassFactor + (1 - loPassFactor) * graphs[i].data[k-1];
				}else {
					graphs[i].data[0] = originalData[i][0];
				}
			}else{
				if (k <lopassSize) {
					graphs[i].data[k] = originalData[i][k];
				}else{
					float sum = 0;
					for(int j = 0; j < lopassSize; j++){
						sum += originalData[i][k-j];
					}
					graphs[i].data[k]=sum/lopassSize;
				}
			}
		}
		graphs[i].resetMinMax();
		graphs[i].updateMesh();
	}
}
//--------------------------------------------------------------
void BYBGui::updateCalibrationGui(int sampleNum, int currentFinger, int totalSamples, bool bNoFingers){
	calibrationGui.update(sampleNum, currentFinger, totalSamples, bNoFingers);
}
//--------------------------------------------------------------
void BYBGui::draw(){
	drawGraphs();
	ofSetColor(255);
	logo.draw(logoRect);
	if (bDrawGui) {
	gui.draw();
	}
	int p;
	if (getIsCalibrating()) {
		p = selectedGraph;
	}else{
		p = controllerPtr->classifier.getPrimaryFinger();
		handImg.selectFinger(p);
	}
	if (p >= 0 && p < graphs.size()) {
		ofPushStyle();
		if (bNewPeak) {
			bNewPeak = false;
			ofSetColor(255);
		}else if (getIsCalibrating()) {
			ofSetColor(ofColor::red, ofMap(sin(ofGetElapsedTimef() * TWO_PI), -1, 1, 0, 255));
		}else{
			ofSetColor(ofColor::orange);
		}
		ofDrawRectangle(graphs[p].guiRectangle.getMaxX() -20, graphs[p].guiRectangle.y, 20, graphs[p].guiRectangle.height);
		ofPopStyle();
	}
	loadButton.draw();
	saveButton.draw();
	calibrateButton.draw();
	accuracyButton.draw();
	if (!getIsCalibrating()){
		handImg.draw(ofColor::orange);
	}
}
//--------------------------------------------------------------
void BYBGui::loPassChangedF(float & f){
	updateLoPass();
	updatePeakDetection();
}
//--------------------------------------------------------------
void BYBGui::loPassChangedI(int & i){
	updateLoPass();
	updatePeakDetection();
}
//--------------------------------------------------------------
void BYBGui::selectGraph(int i){
	if (i >= 0 && i < NUM_GRAPHS) {
		selectedGraph = i;
		for (int j = 0; j < NUM_GRAPHS; j++) {
			if (selectedGraph == j) {
				graphs[j].bHighlight = true;
			}else{
				graphs[j].bHighlight = false;
			}
		}
	}else{
		selectedGraph = -1;
		for (int j = 0; j < NUM_GRAPHS; j++) {
			graphs[j].bHighlight = false;
		}
	}
	handImg.selectFinger(selectedGraph);
	cout << "selectGraph("<<selectedGraph << ");" << endl;

}
//--------------------------------------------------------------
bool BYBGui::getIsCalibrating(){
	if (controllerPtr) {
		return controllerPtr->classifier.isCalibrating();
	}else return false;
}
//--------------------------------------------------------------
void BYBGui::peakParamsChanged(float & f){
	updatePeakDetection();
}
//--------------------------------------------------------------
void BYBGui::peakDetSizeChanged(int & i){
	updatePeakDetection();
}
//--------------------------------------------------------------
void BYBGui::useLoPassChanged(bool & b){
	updateLoPass();
	updatePeakDetection();
}
//--------------------------------------------------------------
void BYBGui::opacityChanged(int& i){
	accuracyGui.setOpacity(i);
	calibrationGui.setOpacity(i);
}
//--------------------------------------------------------------
void  BYBGui::drawGraphs(){
	for (int i = 0; i < graphs.size(); i++) {
		drawGraph(i);
	}
}
//--------------------------------------------------------------
void  BYBGui::drawGraph(int i){
	graphs[i].draw1();
}
//--------------------------------------------------------------
void BYBGui::loadButtonPressed(){
	controllerPtr->loadFingerProfile();
}
//--------------------------------------------------------------
void BYBGui::saveButtonPressed(){
	controllerPtr->saveFingerProfile();
}
//--------------------------------------------------------------
void BYBGui::calibrateButtonPressed(){
	calibrationGui.enable();
	ofNotifyEvent(startCalibration, this);
}
//--------------------------------------------------------------
void BYBGui::accuracyButtonPressed(){
	accuracyGui.enable();
	ofNotifyEvent(startAccuracyTest, this);
}
//--------------------------------------------------------------
void BYBGui::setSizes(){
	guiArea.set(MARGIN, MARGIN, ofGetWidth() - MARGIN, 150);
	float f = ((ofGetHeight()-  guiArea.getMaxY() - MARGIN)/float(NUM_GRAPHS)) - 6;
	for (int i = 0; i < NUM_GRAPHS; i++) {
		graphs[i].setY(guiArea.height+MARGIN+(i * f)+(5*i));
		graphs[i].setHeight(f);
		graphs[i].setWidth(ofGetWidth()-5);
		graphs[i].setX(0);
		graphs[i].setRectangles();
	}
}

//--------------------------------------------------------------
void BYBGui::windowResized(ofResizeEventArgs& args){
	setSizes();
}
//--------------------------------------------------------------
void BYBGui::keyPressed(ofKeyEventArgs& args){
	switch (args.key) {
		case ' ':
		//	bUpdateGraphs ^= true;
			break;
		case 'r':
			for (int i = 0; i < NUM_GRAPHS; i++) {
				graphs[i].resetMinMax();
			}
			break;
		case 'a':
			//			addFinger();
			break;
		case 's':
			controllerPtr->saveFingerProfile();
			break;
		case 'n':
			//			addFinger(true);
			break;
		case 'w':
			//	bWaitForPeak = true;
			break;
        case 'g':
            bDrawGui ^= true;
            break;
		case 'l':
			controllerPtr->serial.loadOfflineData();
			break;
		case 'S':
			controllerPtr->classifier.save("fingers");
			break;
		case '0':
			selectGraph(0);
			break;
		case '1':
			selectGraph(1);
			break;
		case '2':
			selectGraph(2);
			break;
		case '3':
			selectGraph(3);
			break;
		case '4':
			selectGraph(4);
			break;
		default:
			break;
	}
}
//--------------------------------------------------------------
void BYBGui::keyReleased(ofKeyEventArgs& args){}
//--------------------------------------------------------------
void BYBGui::mouseMoved(ofMouseEventArgs& args){}
//--------------------------------------------------------------
void BYBGui::mouseDragged(ofMouseEventArgs& args){}
//--------------------------------------------------------------
void BYBGui::mousePressed(ofMouseEventArgs& args){}
//--------------------------------------------------------------
void BYBGui::mouseScrolled(ofMouseEventArgs& args){}
//--------------------------------------------------------------
void BYBGui::mouseReleased(ofMouseEventArgs& args){
	bool bInside = false;
	for (int i = 0; i < NUM_GRAPHS; i++) {
		if (graphs[i].inside(args.x, args.y)) {
			selectGraph(i);
			bInside = true;
		}
	}
	if (!bInside) {
	//	cout <<"BYBGui::mouseReleased"<< endl;
		selectGraph(-1);
		
	}
}
