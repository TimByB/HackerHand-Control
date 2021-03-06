//
//  BYBGui.cpp
//  FingersReadings
//
//  Created by Roy Macdonald on 11-05-15.
//
//

#include "BYBGui.h"
#include "mainAppController.h"
//--------------------------------------------------------------
void BYBGui::setup( string language){
    bAccuracyTestRunning = false;
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
        if (xml.exists("svm")) {
            svmButton.name = xml.getValue("svm");
        }
        if (xml.exists("euc")) {
            eucButton.name = xml.getValue("euc");
        }
        if (xml.exists("arduino")) {
            arduinoButton.name = xml.getValue("arduino");
        }
        
    }
    for (int i = 0; i < NUM_GRAPHS; i++) {
        graphs[i].setup(ofGetWidth()-5, (string)((fingerNames.count(i)>0)?fingerNames[i]:""));
        graphs[i].setFont(fonts);
        graphs[i].setNormalize(true);
        
    }
    setSizes();
    
    setupParameters();
    setupButtons();
    logo.load("logo.png");
    
    
    
    selectedGraph = selectedFinger= -1;

    ofRegisterKeyEvents(this);
    ofRegisterMouseEvents(this);
    ofAddListener(ofEvents().windowResized, this, &BYBGui::windowResized);
    
    calibrationGui.setPtr(this);
    accuracyGui.setPtr(this);
    
    
    calibrationGui.setButtons();
    accuracyGui.setButtons();
    
    accuracyGui.set(MARGIN, MARGIN, ofGetWidth() - (2*MARGIN), guiArea.height);
    accuracyGui.setup(language);
    calibrationGui.setLanguage(language);
    
    calibrationGui.set(MARGIN, MARGIN, ofGetWidth() - (2*MARGIN), guiArea.height);
    
    calibrationGui.update(0, 0, numSamples, false);
    
    logoRect.set(guiArea.x + MARGIN,guiArea.y + MARGIN, logo.getWidth(), logo.getHeight());
    
    handImg.setup();
    
    handImg.scaleTo(guiArea , OF_ASPECT_RATIO_KEEP);
    
    handImg.x = logoRect.getMaxX() + (calibrateButton.x - logoRect.getMaxX() - handImg.getWidth())/2.0f;
    
    svmButton.setSelected(true);

    bNewPeak = false;
    
}
//--------------------------------------------------------------
void BYBGui::setupParameters(){
    gui.setup();
    gui.setPosition(300, MARGIN);
    //	gui.setWidthElements(200);
    gui.add(lopassSize.set("loPass Size", 5, 1, 30));
    gui.add(bUseLoPass.set("Use Lo Pass", false));
    gui.add(loPassFactor.set("loPass Factor", 0.3, 0, 1));
    gui.add(slopeThreshold.set("slopeThreshold", 10, 0, 10 ));
    gui.add(numSamples.set("Num Samples per finger", 5, 1, 20));
    gui.add(overlayOpacity.set("overlay GUI opacity", 255, 0, 255));
    gui.add(releaseTime.set("Release Time", 500, 1, 2000));
    gui.add(releaseThreshold.set("Release Threshold", 0.5, 0, 1));
    gui.add(bUseKeys.set("Use Keys", false));
    
    overlayOpacity.addListener(this, &BYBGui::opacityChanged);
    
    slopeThreshold.addListener(this, &BYBGui::slopeThresholdChanged);
    loPassFactor.addListener(this, &BYBGui::loPassChangedF);
    bUseLoPass.addListener(this, &BYBGui::useLoPassChanged);
    lopassSize.addListener(this, &BYBGui::loPassChangedI);
    
    gui.loadFromFile("settings.xml");
    
}
//--------------------------------------------------------------
void BYBGui::setupButtons(){
    eucButton.font = fonts["FiraSans-Heavy"];
    svmButton.font = fonts["FiraSans-Heavy"];
    loadButton.font = fonts["FiraSans-Heavy"];
    saveButton.font = fonts["FiraSans-Heavy"];
    calibrateButton.font = fonts["FiraSans-Heavy"];
    accuracyButton.font = fonts["FiraSans-Heavy"];
    arduinoButton.font = fonts["FiraSans-Heavy"];
    
    int buttonWidth = 180;
    
    loadButton.set(guiArea.getMaxX() -buttonWidth - 10, 15 + MARGIN, buttonWidth, 20);
    saveButton.set(guiArea.getMaxX() -buttonWidth - 10, loadButton.getMaxY() + MARGIN, buttonWidth, 20);
    calibrateButton.set(loadButton.getX() - buttonWidth - 10, 15 + MARGIN, buttonWidth, 20);
    accuracyButton.set(loadButton.getX() - buttonWidth - 10, calibrateButton.getMaxY() + MARGIN, buttonWidth, 20);
    eucButton.set(loadButton.getX() - buttonWidth - 10, accuracyButton.getMaxY() + MARGIN, buttonWidth, 20);
    svmButton.set(guiArea.getMaxX() -buttonWidth - 10, accuracyButton.getMaxY() + MARGIN, buttonWidth, 20);
    arduinoButton.set(guiArea.getMaxX() -buttonWidth - 10, svmButton.getMaxY() + MARGIN, buttonWidth, 20);
    
    ofAddListener(loadButton.clickedEvent, this, &BYBGui::loadButtonPressed);
    ofAddListener(saveButton.clickedEvent, this, &BYBGui::saveButtonPressed);
    ofAddListener(calibrateButton.clickedEvent, this, &BYBGui::calibrateButtonPressed);
    ofAddListener(accuracyButton.clickedEvent, this, &BYBGui::accuracyButtonPressed);
    ofAddListener(eucButton.clickedEvent, this, &BYBGui::eucButtonPressed);
    ofAddListener(svmButton.clickedEvent, this, &BYBGui::svmButtonPressed);
    ofAddListener(arduinoButton.clickedEvent, this, &BYBGui::arduinoButtonPressed);
}
//--------------------------------------------------------------
void BYBGui::arduinoButtonPressed(){
    ofSystem("open " + ofToDataPath("arduino/HackerHand_v2.3/") );
}
//--------------------------------------------------------------
void BYBGui::eucButtonPressed(){
    controllerPtr->setClassifier(1);
    eucButton.setSelected(true);
    svmButton.setSelected(false);
}
//--------------------------------------------------------------
void BYBGui::svmButtonPressed(){
    controllerPtr->setClassifier(0);
    eucButton.setSelected(false);
    svmButton.setSelected(true);
}
//--------------------------------------------------------------
void BYBGui::slopeThresholdChanged(float & f){
    for (int i = 0; i < NUM_GRAPHS; i++) {
        graphs[i].slopeThreshold = f;
        graphs[i].updateMesh();
    }
}
//--------------------------------------------------------------
void BYBGui::update(vector<float> & v, const peakData& p){
    bNewPeak = p.bPeakFound;
    for (int i = 0; i < p.peakIndices.size(); i++) {
        graphs[p.peakIndices[i]].addPeak(p.peakDetSize);
    }

    for (int i =0; i < NUM_GRAPHS && i < v.size(); i++) {
        graphs[i].update(v[i]);
    }
    
}
//--------------------------------------------------------------
void BYBGui::moveFinger (int & f){
    if (f < 5) {
        movingFinger[f] = true;
        selectedFinger = f;
    }
    if (bAccuracyTestRunning) {
        accuracyGui.moveFinger(f);
    }
}
//--------------------------------------------------------------
void BYBGui::releaseFinger (int & f){
    if (f < 5) {
        movingFinger[f] = false;
        selectedFinger = 6;
    }
    if (bAccuracyTestRunning) {
        accuracyGui.releaseFinger(f);
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
    if (getIsCalibrating() || bAccuracyTestRunning) {
        p = selectedGraph;
    }else{
        p = selectedFinger;
        handImg.selectFinger(selectedFinger);
    }
    if (p >= 0 && p < NUM_GRAPHS) {
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
    eucButton.draw();
    svmButton.draw();
    arduinoButton.draw();
    if (!getIsCalibrating()){
        handImg.draw(ofColor::orange);
    }
    if (controllerPtr->serial.isWriting()) {
        ofPushStyle();
        ofSetColor(ofColor::red);
        ofFill();
        ofDrawCircle(100, 100, 70);
        ofPopStyle();
    }
}
//--------------------------------------------------------------
void BYBGui::loPassChangedF(float & f){

}
//--------------------------------------------------------------
void BYBGui::loPassChangedI(int & i){

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
//    cout << "selectGraph("<<selectedGraph << ");" << endl;
    
}
//--------------------------------------------------------------
bool BYBGui::getIsCalibrating(){
    if (controllerPtr) {
        if (controllerPtr->getClassifier()) {
            return controllerPtr->getClassifier()->isCalibrating();
        }
    }
    return false;
}
//--------------------------------------------------------------
void BYBGui::useLoPassChanged(bool & b){
}
//--------------------------------------------------------------
void BYBGui::opacityChanged(int& i){
    accuracyGui.setOpacity(i);
    calibrationGui.setOpacity(i);
}
//--------------------------------------------------------------
void  BYBGui::drawGraphs(){
    for (int i = 0; i < NUM_GRAPHS; i++) {
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
    bAccuracyTestRunning = true;
    ofNotifyEvent(startAccuracyTest, this);
    ofAddListener(accuracyGui.guiClosed, this, &BYBGui::accuracyTestEnded);
}
//--------------------------------------------------------------
void BYBGui::accuracyTestEnded(){
    bAccuracyTestRunning = false;
    ofRemoveListener(accuracyGui.guiClosed, this, &BYBGui::accuracyTestEnded);
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
    if (bUseKeys) {
        switch (args.key) {
            case 'r':
                for (int i = 0; i < NUM_GRAPHS; i++) {
                    graphs[i].resetMinMax();
                }
                break;
            case 's':
                controllerPtr->serial.saveData();
                break;
            case 'g':
                bDrawGui ^= true;
                break;
            case 'l':
                controllerPtr->serial.loadOfflineData();
                break;
            case 'S':
                if (controllerPtr->getClassifier()) {
                    controllerPtr->getClassifier()->save("fingers.xml");
                }
                break;
            default:
                break;
        }
    }
    switch (args.key) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
            if(bAccuracyTestRunning){
                accuracyGui.currentFinger = args.key - '0' -1;
            }else{
                if (movingFinger[args.key - '0' -1]) {
                    controllerPtr->serial.releaseFinger2(args.key - '0' -1);
                }else{
                    controllerPtr->serial.moveFinger2(args.key - '0' -1);
                }
                movingFinger[args.key - '0'-1] ^=true;
            }
            break;
        case '0':
            controllerPtr->serial.moveFinger2(5);
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
