//
//  BYBCalibrationGui.cpp
//  FingersReadings
//
//  Created by Roy Macdonald on 21-05-15.
//
//

#include "BYBCalibrationGui.h"

#include "BYBOverlayGui.h"
#include "BYBGui.h"
//----------------------------------------------------------------------
BYBCalibrationGui::BYBCalibrationGui():BYBOverlayGui(), guiPtr(NULL){}
//----------------------------------------------------------------------
void BYBCalibrationGui::setPtr(BYBGui * p){
	guiPtr = p;
}
//----------------------------------------------------------------------
BYBCalibrationGui::~BYBCalibrationGui(){
	guiPtr =NULL;
    BYBOverlayGui::~BYBOverlayGui();
}
//----------------------------------------------------------------------
void BYBCalibrationGui::setLanguage(string lang){
	//cout << "BYBCalibrationGUI: fonts: " << (string)(fonts?"TRUE":"FALSE") << endl;
	/*
	for (std::map<string,ofTrueTypeFont>::iterator it=fonts->begin(); it!=fonts->end(); ++it){
		cout << it->first << endl;
	}
	//*/
	ofXml xml;
	if (xml.load(ofToDataPath("languages/"+lang+".xml", true))) {
		xml.setTo("calibration");
		int c = xml.getNumChildren("text");
		//xml.setTo("text[0]");
		cout << "Calibration GUI text: ";
		for (int i = 0; i < c; i++) {
			text.push_back(xml.getValue("text["+ofToString(i)+"]")); //Value("name"));
			cout << ", " << i << "  / "<< text.back() ;
		}
		cout << endl;
		xml.setToParent();
		xml.setTo("fingers");
		c = xml.getNumChildren("finger");
		xml.setTo("finger[0]");
		for (int i = 0; i < c; i++) {
			fingerNames[xml.getIntValue("ID")] = xml.getValue("name");
			xml.setToSibling();
		}
	}else{
		cout << "CalibrationGUI: Sin XML  " << ofToDataPath("languages/"+lang+".xml", true) << endl;
	}
}
//----------------------------------------------------------------------
void BYBCalibrationGui::update(int sampleNum, int currentFinger, int totalSamples, bool bNoFingers){
	//	cout << "CAlibration gui update" << endl;
	if (bNoFingers) {
		fullText[0] = text.back();
	}else{
		if(text.size() > 2 && currentFinger >= 0 && currentFinger < 5){
			fullText[0] = text[0]+fingerNames[currentFinger]+text[1];
		}
	}
	if(text.size() > 3){
		fullText[1] = text[2]+ofToString(sampleNum)+text[3]+ofToString(totalSamples);
	}
}
//----------------------------------------------------------------------
void BYBCalibrationGui::customDraw(){

        if(fonts.count("FiraSans-Heavy_25") > 0 && fonts.count("FiraSans-Regular_25") > 0){
            ofTrueTypeFont& fh = fonts["FiraSans-Heavy_25"];
            ofTrueTypeFont& fr = fonts["FiraSans-Regular_25"];
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
			
			if (guiPtr) {
				ofRectangle r = (ofRectangle) guiPtr->handImg;
				r.scaleTo(*this);
				r.x = (r.y - y) + x;
				guiPtr->handImg.drawAt(r, ofColor::red, ofMap(sin(ofGetElapsedTimef() * TWO_PI), -1, 1, 0, 255));
			}
		}
	}
        


