//
//  serialManager.h
//
//  Created by Roy Macdonald on 2/2/14.
//  Copyright (c) 2014 micasa. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "BYB_Constants.h"



class serialManager {
public:
	serialManager(){}
	//--------------------------------------------------------------
	void setup(){
		int baud = 9600;
        serial.listDevices();
        vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
		serialOk = false;
		ofXml xml;
		string device = "";
#if defined(TARGET_OSX)
		string serialXML = "//serial_OSX";
#elif defined(TARGET_WIN32)
		string serialXML = "//serial_WIN";
#elif defined(TARGET_LINUX)
		string serialXML = "//serial_LINUX";
#endif
		if (xml.load(ofToDataPath("serial.xml"))) {
			if (xml.exists(serialXML)) {
				device = xml.getValue(serialXML);
			}
		}
        for (int i = 0; i < deviceList.size(); i++) {
            cout << deviceList[i].getDevicePath() << endl;
            std::size_t found = deviceList[i].getDevicePath().find(device);
            if (found!=std::string::npos){
                serialOk = serial.setup(deviceList[i].getDeviceID() ,baud);
                break;
            }
        }
		
		serialString ="";
        lastSentSerial = ofGetElapsedTimeMillis();
		bUseOfflineData = false;
		analogData.resize(NUM_GRAPHS);
		offlineDataUpdateInterval =20;
		bWriteToBuffers = false;
        lastSentSerial = 0;
	}
	//--------------------------------------------------------------
	void update(){
		if(serialOk){
			while (serial.available()) {
				unsigned char i1 = serial.readByte();
                
				//	cout << i1;
				//*
                 if (i1 != '\n') {
					serialString+=i1;
				}else{
               //     cout << serialString << endl;
					vector<string> vals = ofSplitString(serialString, ",");
					for (int i = 0; i < vals.size() && i < NUM_GRAPHS; i++) {
						analogData[i] = ofToInt(vals[i]);
                        if(bWriteToBuffers){
                            saveBuffers[i].append(ofToString(analogData[i]) + ";" );
                        }
					}
					serialString ="";
					ofNotifyEvent(newDataEvent, analogData, this);
				}
                //*/
			}
		}else if(bUseOfflineData){
			unsigned long t = ofGetElapsedTimeMillis();
			if (t - lastOfflineUpdate >= offlineDataUpdateInterval) {
				lastOfflineUpdate = t;
				for (int i = 0; i < NUM_GRAPHS; i++) {
					analogData[i] = offlineData[i][offlineDataCount];
                    if(bWriteToBuffers){
                        saveBuffers[i].append(ofToString(analogData[i]) + ";" );
                    }
				}
				offlineDataCount++;
				if(offlineDataCount >= offlineData[0].size()){
					offlineDataCount = 0;
				}
				ofNotifyEvent(newDataEvent, analogData, this);
			}
		}
	}
	void moveFinger2(int f){
        int ff =f;
        moveFinger(f);
    }
    void releaseFinger2(int f){
        int ff =f;
        releaseFinger(f);
    }
    //--------------------------------------------------------------
	void releaseFinger(int& f){
        if(serialOk){
            unsigned long long t = ofGetElapsedTimeMillis();
                cout << "release finger: " << f << endl;
                serial.writeByte('f');
                serial.writeByte((unsigned char) f);
        }
    }
    //--------------------------------------------------------------
	void moveFinger(int& f){
        
        if(serialOk){
            unsigned long long t = ofGetElapsedTimeMillis();
            if (t-lastSentSerial > 500) {
            cout << "move finger: " << f << endl;
            serial.writeByte('F');
            serial.writeByte((unsigned char) f);
                lastSentSerial = t;
            }
        }
    }
	//--------------------------------------------------------------
    bool isWriting(){
        return bWriteToBuffers;
    }
	//--------------------------------------------------------------
	void loadOfflineData(){
		ofFileDialogResult r = ofSystemLoadDialog("seleccionar carpeta para cargar", true);
		if (r.bSuccess) {
			ofDirectory dir(r.getPath());
			dir.listDir();
            cout << "loadOffline data"<< endl;
			for (int i = 0; i < dir.size(); i++) {
                bUseOfflineData = true;
                ofBuffer b = ofBufferFromFile(dir.getPath(i));
				int num = ofToInt(dir.getName(i));
				if ( num >= 0 && num < NUM_GRAPHS) {
					//unsigned int m =UINT16_MAX;
					vector<string> ss = ofSplitString(b.getText(), ";");
                    cout << "num: " << num << "  size: " << ss.size() << endl;
					offlineData[num].resize(ss.size());
					for (int k = 0; k < ss.size(); k++) {
						offlineData[num][k] = (int)ofToFloat(ss[k]);
						/*
                        if(offlineData[num][k] > 0){
							m = MIN(offlineData[num][k], m);
						}//*/
					}
				//	cout << m << ", ";
				}
			}
//            cout << endl;
		}
		lastOfflineUpdate = ofGetElapsedTimeMillis();
		offlineDataCount =0;
        bWriteToBuffers = false;
	}
	//--------------------------------------------------------------
	void saveData(){
        if (bWriteToBuffers) {
            bWriteToBuffers = false;
            for(int i = 0; i < 5; i++){
                ofBufferToFile(bufferSavePath+"/"+ofToString(i)+".txt", saveBuffers[i]);
            }
        }else{
        ofFileDialogResult r = ofSystemLoadDialog("seleccionar carpeta para cargar", true, ofToDataPath("fingersData"));    
        //ofFileDialogResult r = ofSystemLoadDialog("seleccionar carpeta para cargar", true, ofToDataPath("fingersData"), true);
		if (r.bSuccess) {
            bWriteToBuffers = true;
            bufferSavePath = r.getPath();
            for(int i = 0; i < 5; i++){
                saveBuffers[i].clear();
            }
        }
        }
    }
	//--------------------------------------------------------------
	vector<unsigned int>& getData(){
		return analogData;
	}
	//--------------------------------------------------------------
	
	ofSerial serial;
	string serialString;

	ofEvent<vector<unsigned int> > newDataEvent;
protected:
    ofBuffer saveBuffers [5];
	vector<unsigned int> analogData;
	bool serialOk;
	bool bUseOfflineData;
	vector<unsigned int> offlineData [NUM_GRAPHS];
	unsigned long offlineDataUpdateInterval, lastOfflineUpdate, lastSentSerial;
	int offlineDataCount;
    bool bWriteToBuffers;
    string bufferSavePath;
};