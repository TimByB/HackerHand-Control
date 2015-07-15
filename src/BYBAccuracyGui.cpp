//
//  BYBAccuracyGui.cpp
//  FingersReadings
//
//  Created by Roy Macdonald on 29-05-15.
//
//

#include "BYBAccuracyGui.h"
#include "BYBOverlayGui.h"
#include "BYBGui.h"

void BYBAccuracyGui::setPtr(BYBGui * p){guiPtr = p;}
void BYBAccuracyGui::setup(string lang){
    setLanguage(lang);
    buttons.resize(4);
    cout << "BYBAccuracyGui::setup("<< endl;
    float buttonWidth = 150;
    float margin = (getHeight() - 120)/4.0f;
    buttons[1].name = "Random";
    buttons[1].font = &fonts->at("FiraSans-Heavy");//["HelveticaNeueLTStd-Md"];
    buttons[1].set(buttons[0].getX() -buttonWidth -10, getY() + margin , buttonWidth, 50);
    
    buttons[2].name = "Manual";
    buttons[2].font = &fonts->at("FiraSans-Heavy");//["HelveticaNeueLTStd-Md"];
    buttons[2].set(buttons[0].getX() -buttonWidth -10, buttons[1].getMaxY() + margin , buttonWidth, 50);
    
    buttons[3].name = "Export";
    buttons[3].font = &fonts->at("FiraSans-Heavy");//["HelveticaNeueLTStd-Md"];
    buttons[3].set(buttons[0].getX() -buttonWidth -10, buttons[2].getMaxY() + margin , buttonWidth, 20);
    resetTest();
    totalTests = 100;
}
//--------------------------------------------------------------
void BYBAccuracyGui::enable(bool e){
    if (bIsEnabled != e) {
        if (e) {
            resetTest();
            if (buttons.size()>2) {
                ofAddListener(buttons[1].clickedEvent, this, &BYBAccuracyGui::randomPressed);
                ofAddListener(buttons[2].clickedEvent, this, &BYBAccuracyGui::manualPressed);
                ofAddListener(buttons[3].clickedEvent, this, &BYBAccuracyGui::exportPressed);
            }
        }else{
            if (buttons.size()>2) {
                ofRemoveListener(buttons[1].clickedEvent, this, &BYBAccuracyGui::randomPressed);
                ofRemoveListener(buttons[2].clickedEvent, this, &BYBAccuracyGui::manualPressed);
                ofRemoveListener(buttons[3].clickedEvent, this, &BYBAccuracyGui::exportPressed);
            }
        }
    }
    BYBOverlayGui::enable(e);
    //  bIsEnabled = e;
}
//--------------------------------------------------------------
void BYBAccuracyGui::randomPressed(){
    bIsRandom = true;
    startTest();
}
//--------------------------------------------------------------
void BYBAccuracyGui::manualPressed(){
    bIsRandom = false;
    startTest();
}
//--------------------------------------------------------------
void BYBAccuracyGui::exportPressed(){
    ofFileDialogResult r = ofSystemSaveDialog("AccuracyTest.csv", "Save your accuracy test as a .csv file");
    if (r.bSuccess) {
        string s = "Total number of tests," + ofToString(testNum) + "\n";
        float percent = 0;
        if ( testNum != 0) {
            percent = 100*(float)correctAnswers/testNum;
        }
        s+= "Global Accuracy," + ofToString(percent) + "\n";
        s+= "False Negatives" + ofToString(falseNegatives) + "\n";
        for (int i = 0; i< 5; i++) {
            s+= "Finger "+ fingerNames[i] + "\n";
            s+= "Number of Tests, " + ofToString(testNumPerFinger[i]) + "\n";
            s+= "Number of False Negatives, " + ofToString(falseNegativePerFinger[i]) + "\n";
            float p = 0;
            if (testNumPerFinger[i] != 0) {
                p = 100*(float)correctPerFinger[i]/testNumPerFinger[i];
            }
            s+= "Accuracy, " + ofToString(p) + "%\n";
        }
        ofBuffer b(s);
        ofBufferToFile(r.getPath(), b);
    }
}
//--------------------------------------------------------------
void BYBAccuracyGui::resetTest(){
    testNum = 0;
    correctAnswers = 0;
    currentFinger = 0;
    prevFinger = 0;
    falseNegatives = 0;
    memset(testNumPerFinger, 0, sizeof(int)*5);
    memset(correctPerFinger, 0, sizeof(int)*5);
    memset(falseNegativePerFinger, 0, sizeof(int)*5);
}
//--------------------------------------------------------------
void BYBAccuracyGui::moveFinger(int f){
    testNum++;
    testNumPerFinger[currentFinger]++;
    if (currentFinger == f) {
        correctAnswers++;
        correctPerFinger[f]++;
    }
    prevFinger = currentFinger;
    if (bIsRandom) {
        currentFinger = (int)floor(ofRandom(5));
        if (currentFinger >4) {
            currentFinger = 4;
        }
    }
    
}
//--------------------------------------------------------------
void BYBAccuracyGui::releaseFinger(int f){
    
}
//--------------------------------------------------------------
void BYBAccuracyGui::setLanguage(string lang){
    ofXml xml;
    if (xml.load(ofToDataPath("languages/"+lang+".xml", true))) {
        xml.setTo("accuracy");
        int c = xml.getNumChildren("text");
        //xml.setTo("text[0]");
        cout << "AcuracyTest ";
        for (int i = 0; i < c; i++) {
            text.push_back(xml.getValue("text["+ofToString(i)+"]")); //Value("name"));
            cout << text.back() << "//";
        }
        cout << endl;
        if (buttons.size()>2) {
            buttons[1].name = xml.getValue("random");
            buttons[2].name = xml.getValue("manual");
            cout << buttons[0] << "  " << buttons[2] << endl;
        }
        xml.setToParent();
        xml.setTo("fingers");
        c = xml.getNumChildren("finger");
        xml.setTo("finger[0]");
        for (int i = 0; i < c; i++) {
            fingerNames[xml.getIntValue("ID")] = xml.getValue("name");
            xml.setToSibling();
        }
    }else{
        cout << "AccuracyGUI: Sin XML  " << ofToDataPath("languages/"+lang+".xml", true) << endl;
    }
}
void BYBAccuracyGui::startTest(){
    bTestStarted = true;
    resetTest();
}
void BYBAccuracyGui::endTest(){
    bTestStarted = false;
}
bool BYBAccuracyGui::keyReleased(ofKeyEventArgs& args){
    if(BYBOverlayGui::keyReleased(args)){
        return true;
    }else{
        if (args.key == ' ') {
            addFalseNegative();
            return true;
        }
    }
    return false;
}
void BYBAccuracyGui::addFalseNegative(){
    testNum++;
    testNumPerFinger[prevFinger]++;
    falseNegatives++;
    
}
void BYBAccuracyGui::customDraw(){
    
    //  cout << "BYBAccuracyGui::customDraw() fonts: "+ ofToString((bool)fonts) << endl;
    //*
    if (fonts != NULL ) {
        if(fonts->count("FiraSans-Heavy") > 0 && fonts->count("FiraSans-Regular") > 0){
            ofTrueTypeFont& fh = fonts->at("FiraSans-Heavy");
            ofTrueTypeFont& fr = fonts->at("FiraSans-Regular");
            
            float percent = 0;
            if (testNum != 0) {
                percent = 100*(float)correctAnswers/testNum;
            }
            fullText[0] = text[4] + ofToString(percent, 2) + text[5];
            if (bIsRandom) {
                fullText[0] += "\n"+buttons[1].name+" Mode";
            }else{
                fullText[0] += "\nManual Mode";
            }
            fullText[1] = "Test num: " + ofToString(testNum)+"\n";
            if (fingerNames.count(currentFinger)) {
                if (!bIsRandom) {
                    fullText[1] += text[2]+"\n"+text[3]+"\n";
                    if (fingerNames.size() > 4) {
                        for (int i = 0; i < 5; i++) {
                            if (fingerNames.count(i)) {
                                if (i>0) {
                                    fullText[1] += "  ";
                                }
                                fullText[1] += ofToString(i+1) + " = " + fingerNames[i];
                            }
                        }
                        if (currentFinger >= 0 && currentFinger < 5) {
                            fullText[1] += "\ncurrent finger: " + fingerNames[currentFinger];
                        }
                    }
                }else{
                    fullText[1] += "\n"+text[0] + ofToString(fingerNames[currentFinger]) + text[1];
                }
                fullText[1] += "\n"+text.back();
            }
            
            ofRectangle rh = fh.getStringBoundingBox(fullText[0], 0, 0);//gap);
            ofRectangle rr = fr.getStringBoundingBox(fullText[1], 0, 0);//2*gap + fh.getLineHeight());
            float gap = (this->getHeight() - rh.getHeight() - rr.getHeight())/3.0f;
            
            //cout << "gap: " << gap << "rects: " << rh << "   " << rr << endl;
            rh.alignToHorz(*this);
            rr.alignToHorz(*this);
            
            //rh.y = gap + y - rh.y;
            //rr.y = gap + rh.getMaxY()+y - rr.y;
            if (rh.height + rr.height > height ) {
                fh.drawString(fullText[0], rh.x, -rh.y +10);
                fr.drawString(fullText[1], rr.x, height - rr.height -rr.y);
            }else{
            fh.drawString(fullText[0], rh.x, gap - rh.y + y);
            fr.drawString(fullText[1], rr.x, 2*gap -rr.y + y + rh.height);
            }
            if (guiPtr){// && bIsRandom) {
                ofRectangle r = (ofRectangle) guiPtr->handImg;
                r.scaleTo(*this);
                r.x = (r.y - y) + x;
                guiPtr->handImg.selectFinger(currentFinger);
                guiPtr->handImg.drawAt(r, ofColor::red, ofMap(sin(ofGetElapsedTimef() * TWO_PI), -1, 1, 0, 255));
            }
        }
    }
}