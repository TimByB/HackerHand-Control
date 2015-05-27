//
//  BYBOverlayGui.cpp
//  FingersReadings
//
//  Created by Roy Macdonald on 12-05-15.
//
//

#include "BYBOverlayGui.h"
shared_ptr<map<string,ofTrueTypeFont> > BYBOverlayGui::fonts =  shared_ptr<map<string,ofTrueTypeFont> >();
//--------------------------------------------------------------
BYBOverlayGui::BYBOverlayGui():bIsEnabled(false),opacity(255){
	setButtons();
}
//--------------------------------------------------------------
void BYBOverlayGui::setButtons(){
	buttons.resize(1);
	buttons[0].name = "X";
	buttons[0].font = &fonts->at("FiraSans-Heavy");//["HelveticaNeueLTStd-Md"];
	buttons[0].set(getMaxX() -25, getY() + 5, 20, 20);
}
//--------------------------------------------------------------
void BYBOverlayGui::set(float px, float py, float w, float h){
	ofRectangle::set(px, py, w, h);
	setButtons();
}
//--------------------------------------------------------------
void BYBOverlayGui::set(const ofPoint& p, float w, float h){
	ofRectangle::set(p, w, h);
	setButtons();
}
//--------------------------------------------------------------
void BYBOverlayGui::set(const ofRectangle& rect){
	ofRectangle::set(rect);
	setButtons();
}
//--------------------------------------------------------------
void BYBOverlayGui::enable(bool e){
	if (bIsEnabled != e) {
		if (e) {
			ofRegisterMouseEvents(this, OF_EVENT_ORDER_BEFORE_APP);
			ofRegisterKeyEvents(this, OF_EVENT_ORDER_BEFORE_APP);
			if (buttons.size()) {
				ofAddListener(buttons[0].clickedEvent, this, &BYBOverlayGui::close);
			}
			ofAddListener(ofEvents().draw, this, &BYBOverlayGui::draw, OF_EVENT_ORDER_AFTER_APP);
		}else{
			ofUnregisterMouseEvents(this, OF_EVENT_ORDER_BEFORE_APP);
			ofUnregisterKeyEvents(this, OF_EVENT_ORDER_BEFORE_APP);
			if (buttons.size()) {
				ofRemoveListener(buttons[0].clickedEvent, this, &BYBOverlayGui::close);
			}
			ofRemoveListener(ofEvents().draw, this, &BYBOverlayGui::draw, OF_EVENT_ORDER_AFTER_APP);
			ofNotifyEvent(guiClosed, this);
		}
	}
	bIsEnabled = e;
}
//--------------------------------------------------------------
void BYBOverlayGui::draw(ofEventArgs& args){
	if (bIsEnabled) {
		drawBackground();
		for (int i = 0; i < buttons.size(); i++) {
			buttons[i].draw();
		}
		customDraw();
	}
}
//--------------------------------------------------------------
void BYBOverlayGui::drawBackground(){
	
	ofPushStyle();
	ofSetColor(0,opacity);
	ofFill();
	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	ofDrawRectangle(*this);
	ofSetLineWidth(3);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(ofColor::darkCyan,opacity);
	ofNoFill();
	ofDrawRectangle(*this);
	ofPopStyle();
}
//--------------------------------------------------------------
void BYBOverlayGui::close(){
	enable(false);
}
//--------------------------------------------------------------
bool BYBOverlayGui::keyPressed(ofKeyEventArgs& args){
	if (args.key == OF_KEY_ESC ) {
		close();
		return true;
	}else{
		return false;
	}
}
//--------------------------------------------------------------
bool BYBOverlayGui::keyReleased(ofKeyEventArgs& args){
	return false;
}
//--------------------------------------------------------------
bool BYBOverlayGui::mouseMoved(ofMouseEventArgs& args){
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].mouseMoved(args);
	}
	return true;
}
//--------------------------------------------------------------
bool BYBOverlayGui::mouseDragged(ofMouseEventArgs& args){
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].mouseDragged(args);
	}
	return true;
}
//--------------------------------------------------------------
bool BYBOverlayGui::mousePressed(ofMouseEventArgs& args){
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].mousePressed(args);
	}
	return true;
}
//--------------------------------------------------------------
bool BYBOverlayGui::mouseReleased(ofMouseEventArgs& args){
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].mouseReleased(args);
	}
	return true;
}
//--------------------------------------------------------------
bool BYBOverlayGui::mouseScrolled(ofMouseEventArgs& args){
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].mouseScrolled(args);
	}
	return true;
}
