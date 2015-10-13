
#pragma once
#include "ofMain.h"
#include "ofxInteractiveRect.h"

#define DEFAULT_GRAPH_SIZE 100
#define USE_SHARED_PTR_DATA
class plotLabels {
public:
    plotLabels():dataPrt(NULL), labelColor(ofColor::white){
        name = "";
    }
    ~plotLabels(){
        dataPrt = NULL;
    }
    string name;
    vector<unsigned int> labels;
    ofColor labelColor;
    vector<float>* dataPrt;
};

class baseGraph {
public:
    baseGraph()
#ifdef USE_SHARED_PTR_DATA
    :data(nullptr)
#endif
        {}
    ~baseGraph(){
        fonts.clear();
    }
    //--------------------------------------------------------------
    void setup(int size, string name = ""){
#ifdef USE_SHARED_PTR_DATA
        if(data){
   //         data->resize(size);
        }
#else
        data.resize(size);
#endif
        
        maxVal = -FLT_MAX;
        minVal= FLT_MAX;
        bNormalize = true;
        scale= 1.0f;
        this->name = name;
        threshold = 0;
        bShowLegend = true;
        //		mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        mesh.setMode(OF_PRIMITIVE_POINTS);
        slopeThreshold = 10;
        peaks.setMode(OF_PRIMITIVE_POINTS);
        
    }
    //--------------------------------------------------------------
    void update(float newData){
        //cout << " u: " << newData;
        if (newData>maxVal) {
            maxVal = newData;
        }
        if (newData<minVal) {
            minVal = newData;
        }
#ifdef USE_SHARED_PTR_DATA
        if(data){
            memcpy(data->data(), data->data()+1, (data->size()-1)*sizeof(float));
            data->back() = newData;
        }
#else
        memcpy(data.data(), data.data()+1, (data.size()-1)*sizeof(float));
        data.back() = newData;
#endif
        updateMesh();
        updatePeaks();
    }
    //--------------------------------------------------------------
    void draw(float x, float y, float width, float height){
        draw(ofRectangle(x, y, width, height));
    }
    //--------------------------------------------------------------
    void draw(int x, int y, int width, int height){
        draw(ofRectangle(x, y, width, height));
    }
    //--------------------------------------------------------------
    void draw(ofRectangle r){
        drawBackground(r);
        drawData(r);
        drawOverlays(r);
        drawLegend(r);
    }
    //--------------------------------------------------------------
    void drawOverlays(ofRectangle &r){
        ofPushStyle();
        ofSetColor(0, 255, 255);
        ofDrawLine(0, 0,r.width, 0);
        ofSetColor(255, 0, 0);
        ofDrawLine(0, threshold,r.width, threshold);
        ofPopStyle();
    }
    //--------------------------------------------------------------
    void drawBackground(ofRectangle &r){
        ofPushStyle();
        ofSetColor(60);
        ofFill();
        ofDrawRectangle(r);
        ofNoFill();
        ofSetColor(70);
        ofDrawRectangle(r);
        ofPopStyle();
    }
    //--------------------------------------------------------------
    void drawData(ofRectangle &r){
        ofPushStyle();
        
#ifdef USE_SHARED_PTR_DATA
        float xFactor = 1;
        if(data){
            xFactor= r.width/data->size();
        }
#else
        float xFactor = r.width/data.size();
#endif
        
        
        ofPushMatrix();
        if(minVal>=0){
            ofTranslate(0, r.height);
        }else{
            ofTranslate(0, (r.height/2));
        }
        if (bNormalize) {
            if(minVal>0){
                ofScale(xFactor, -r.height/maxVal);
            }else{
                ofScale(xFactor, -r.height/(maxVal - minVal));
            }
        }else{
            ofScale(xFactor, -scale);
        }
        mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        ofSetColor(ofColor::lightBlue);
        mesh.draw();
        mesh.setMode(OF_PRIMITIVE_POINTS);
        ofSetColor(ofColor::white);
        glPointSize(1);
        mesh.draw();
        ofSetColor(ofColor::black);
        glPointSize(10);
        peaks.draw();
        ofSetColor(ofColor::pink);
        glPointSize(8);
        peaks.draw();
        glPointSize(1);
        ofPopStyle();
        ofPopMatrix();
    }
    //--------------------------------------------------------------
    void updateMesh(){
        mesh.clear();
#ifdef USE_SHARED_PTR_DATA
        if(data){
            for (int i = 0; i < data->size(); i++) {
                mesh.addVertex(ofVec3f(i, data->at(i)));
                if(i > 0){
                    if ((data->at(i) - data->at(i-1)) > slopeThreshold  ) {
#else
                        for (int i = 0; i < data.size(); i++) {
                            mesh.addVertex(ofVec3f(i, data[i]));
                            if(i > 0){
                                if ((data[i] - data[i-1]) > slopeThreshold  ) {
#endif
                                    mesh.addColor(ofColor::yellow);
                                }else{
                                    mesh.addColor(ofColor::lightBlue);
                                }
                            }else{
                                mesh.addColor(ofColor::yellow);
                            }
                        }
#ifdef USE_SHARED_PTR_DATA
                    }
#endif
                }
                //--------------------------------------------------------------
                void addPeak(int ind){
#ifdef USE_SHARED_PTR_DATA
                    if(data){
                        int i = data->size() - ind ;
                        if(i>=0){
                            peaks.addVertex(ofVec3f(i, data->at(i)));
                        }
                    }
#else
                    peaks.addVertex(ofVec3f(ind, data[ind]));
#endif
                }
                //--------------------------------------------------------------
                void updatePeaks(){//vector<int> & p){
                    vector<ofVec3f>& v = peaks.getVertices();
                    for (int i = 0; i < v.size(); i++) {
                        v[i].x--;
                        if (v[i].x < 0) {
                            v.erase(v.begin() + i);
                            i--;
                        }
                    }
                }
                //--------------------------------------------------------------
                void drawLegend(ofRectangle & r){
                    ofSetColor(0, 0, 0 );
                    //ofLoadIdentityMatrix();
                    string legend = "";
                    if (bShowLegend) {
                        legend += "min: " + ofToString(minVal) + "\n";
                        legend += "max: " + ofToString(maxVal) + "\n";
#ifdef USE_SHARED_PTR_DATA
                        if(data){
                            if (data->size()) {
                                legend += "last Val: " + ofToString(data->back()) + "\n";
                            }
                        }
#else
                        if (data.size()) {
                            legend += "last Val: " + ofToString(data.back()) + "\n";
                        }
#endif
                        legend+= "num Peaks: " + ofToString(peaks.getNumVertices());
                        ofSetColor(200);
                        if( fonts.count("FiraSans-Heavy") && fonts.count("FiraSans-Regular") ){
                            fonts.find("FiraSans-Heavy")->second.drawString(name, r.x+5, r.y+18);
                            fonts.find("FiraSans-Regular")->second.drawString(legend, r.x+5, r.y+23 + fonts.find("FiraSans-Heavy")->second.getStringBoundingBox(name, 0, 0).height);
                        }else{
                            ofDrawBitmapString(legend, r.x+5, r.y+18);
                        }
                    }
                }
                //--------------------------------------------------------------
                void setName(string n){
                    name = n;
                }
                //--------------------------------------------------------------
                string getName(){
                    return name;
                }
                //--------------------------------------------------------------
                void setScale(float s){
                    scale = s;
                }
                //--------------------------------------------------------------
                float getScale(){
                    return scale;
                }
                //--------------------------------------------------------------
                void setNormalize(bool n = true){
                    bNormalize = n;
                }
                //--------------------------------------------------------------
                void resetMinMax(){
                    maxVal = -FLT_MAX;
                    minVal= FLT_MAX;
#ifdef USE_SHARED_PTR_DATA
                    if(data){
                        for (int i=0; i<data->size(); i++) {
                            if ( data->at(i) > maxVal) {
                                maxVal =  data->at(i);
                            }
                            if ( data->at(i)<minVal) {
                                minVal =  data->at(i);
                            }
                        }
                    }
#else
                    for (int i=0; i<data.size(); i++) {
                        if ( data[i] > maxVal) {
                            maxVal =  data[i];
                        }
                        if ( data[i]<minVal) {
                            minVal =  data[i];
                        }
                    }
#endif
                    
                }
                //--------------------------------------------------------------
                void toggleNormalize(){
                    setNormalize(!bNormalize);
                }
                //--------------------------------------------------------------
                void showLegend(bool bShowLegend){
                    this->bShowLegend = bShowLegend;
                }
                //--------------------------------------------------------------
                void toggleLegend(){
                    bShowLegend ^=true;
                }
                //--------------------------------------------------------------
                void setThreshold(float t){
                    threshold = t;
                }
                //--------------------------------------------------------------
                void resetPeaks(){
                    peaks.clear();
                }
                //--------------------------------------------------------------
                void setFont(map<string,ofTrueTypeFont>& f){
                    fonts = f;
                }
                //--------------------------------------------------------------
                float threshold;
                shared_ptr<vector<float> > data;
                float slopeThreshold;
            protected:
                map<string,ofTrueTypeFont>  fonts;
                float maxVal, minVal;
                bool bNormalize;
                float scale;
                string name;
                bool bShowLegend;
                ofMesh mesh;
                ofMesh peaks;
                vector<plotLabels> labels;
            };
            class graph1D :public ofRectangle, public baseGraph{
                
            public:
                
                graph1D(string s = ""): outerMargin(3), bHighlight(false){}
                ~graph1D(){}
                void setup(int size, string _name = ""){
                    
                    this->width = size;
                    this->height = DEFAULT_GRAPH_SIZE;
                    setRectangles();
                    baseGraph::setup(graphRect.width, _name);
                    
                }
                
                void draw1(){
                    if (lastRect != *this) {
                        setRectangles();
                    }
                    ofPushStyle();
                    if (bHighlight) {
                        ofSetColor(0,174, 239);
                    }else{
                        ofSetColor(20);
                    }
                    ofDrawRectangle(*this);
                    drawBackground(graphRect);
                    
                    ofPushMatrix();
                    ofPushView();
                    ofViewport(graphRect);//graphRect);
                    ofSetupScreen();
                    drawData(graphRect);
                    ofPopView();
                    ofPopMatrix();
                    
                    drawOverlays(graphRect);
                    if (bHighlight) {
                        ofPushStyle();
                        ofFill();
                        ofSetColor(43, 57, 144);
                        
                        ofDrawRectangle(guiRectangle);
                        ofSetLineWidth(outerMargin);
                        ofSetColor(0,174, 239);
                        ofDrawLine(guiRectangle.getMaxX()-outerMargin*0.5f, guiRectangle.y, guiRectangle.getMaxX()-outerMargin*0.5f,guiRectangle.getMaxY());
                        ofPopStyle();
                    }
                    drawLegend(guiRectangle);
                    ofPopStyle();
                    
                }
                void setRectangles(){
                    guiRectangle.set(x+outerMargin,y+outerMargin, 200, this->height-2*outerMargin);//-2*outerMargin);
                    graphRect.set(guiRectangle.getMaxX(), y+ outerMargin, this->width - 2*outerMargin - guiRectangle.width, this->height-2*outerMargin);
                    lastRect = *this;
                }
                ofRectangle graphRect, guiRectangle, lastRect;
                float outerMargin;
                bool bHighlight;
            };
            
            
