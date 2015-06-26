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
    buttons.resize(3);
    buttons[1].name = "Random";
    buttons[1].font = &fonts->at("FiraSans-Heavy");//["HelveticaNeueLTStd-Md"];
    buttons[1].set(getMaxX() -200, buttons[0].getMaxY() + 10 , 150, 50);
    
    buttons[2].name = "Manual";
    buttons[2].font = &fonts->at("FiraSans-Heavy");//["HelveticaNeueLTStd-Md"];
    buttons[2].set(getMaxX() -200, buttons[1].getMaxY() + 10 , 150, 50);
    testNum = 0;
    correctAnswers = 1;
    totalTests = 100;
}
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
void BYBAccuracyGui::startTest(bool bRandom){
    if (bIsRandom != bRandom) {
        bIsRandom = bRandom;
        testNum = 0;
        correctAnswers = 0;
        currentFinger = 0;
    }
}
void BYBAccuracyGui::endTest(){

}

void BYBAccuracyGui::customDraw(){
  //  cout << "BYBAccuracyGui::customDraw() fonts: "+ ofToString((bool)fonts) << endl;
    if (fonts != NULL ) {
        if(fonts->count("FiraSans-Heavy_25") > 0 && fonts->count("FiraSans-Regular_25") > 0){
            ofTrueTypeFont& fh = fonts->at("FiraSans-Heavy_25");
            ofTrueTypeFont& fr = fonts->at("FiraSans-Regular_25");
            
            float percent = 0;
            if (correctAnswers != 0) {
                percent = testNum/(float)correctAnswers;
            }
            fullText[0] = text[4] + ofToString(percent, 2) + text[5];
            if (fingerNames.count(currentFinger)) {
                if (!bIsRandom) {
                    fullText[1] = text[2]+"\n"+text[3]+"\n";
                    if (fingerNames.size() > 4) {
                        for (int i = 0; i < 5; i++) {
                            if (fingerNames.count(i)) {
                                if (i>0) {
                                    fullText[1] += "  ";
                                }
                                fullText[1] += ofToString(i) + " = " + fingerNames[i];
                            }
                        }
                    }
                }else{
                    fullText[1] = text[0] + ofToString(fingerNames[currentFinger]) + text[1];
                }
            }
            
            ofRectangle rh = fh.getStringBoundingBox(fullText[0], 0, 0);//gap);
            ofRectangle rr = fr.getStringBoundingBox(fullText[1], 0, 0);//2*gap + fh.getLineHeight());
            float gap = (this->getHeight() - rh.getHeight() - rr.getHeight())/3.0f;
            
            //cout << "gap: " << gap << "rects: " << rh << "   " << rr << endl;
            rh.alignToHorz(*this);
            rr.alignToHorz(*this);
            
            //rh.y = gap + y - rh.y;
            //rr.y = gap + rh.getMaxY()+y - rr.y;
            fh.drawString(fullText[0], rh.x, gap - rh.y + y);
            fr.drawString(fullText[1], rr.x, 2*gap -rr.y + y + rh.height);
            
            if (guiPtr && bIsRandom) {
                ofRectangle r = (ofRectangle) guiPtr->handImg;
                r.scaleTo(*this);
                r.x = (r.y - y) + x;
                guiPtr->handImg.drawAt(r, ofColor::red, ofMap(sin(ofGetElapsedTimef() * TWO_PI), -1, 1, 0, 255));
            }
        }
    }
}