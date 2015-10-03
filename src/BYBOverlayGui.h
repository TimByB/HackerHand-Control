//
//  OverlayGui.h
//  FingersReadings
//
//  Created by Roy Macdonald on 12-05-15.
//
//

#pragma once
#include "ofMain.h"
#include "button.h"


class BYBOverlayGui: public ofRectangle{
public:
#ifdef USE_SHARED_FONTS
	static shared_ptr<map<string,ofTrueTypeFont> > fonts;
#else
    map<string,ofTrueTypeFont> fonts;    
#endif

	BYBOverlayGui();
    virtual ~BYBOverlayGui();
	virtual void setButtons();
    virtual void arrangeButtons();
	void loadFonts();
	void set(float px, float py, float w, float h);
    void set(const ofPoint& p, float w, float h);
    void set(const ofRectangle& rect);
	virtual void enable(bool e = true);
	bool isEnabled(){return bIsEnabled;}
	virtual void customDraw(){}
	virtual void close();
	virtual bool keyPressed(ofKeyEventArgs& args);
	virtual bool keyReleased(ofKeyEventArgs& args);
	virtual bool mouseMoved(ofMouseEventArgs& args);
	virtual bool mouseDragged(ofMouseEventArgs& args);
	virtual bool mousePressed(ofMouseEventArgs& args);
	virtual bool mouseReleased(ofMouseEventArgs& args);
	virtual bool mouseScrolled(ofMouseEventArgs& args);
    virtual void mouseEntered(ofMouseEventArgs & args){}
	virtual void mouseExited(ofMouseEventArgs & args){}

	void setOpacity(int o){opacity = o;}
	int getOpacity(){return opacity;}
	ofEvent<void> guiClosed;
protected:
	void draw(ofEventArgs& args);
	void drawBackground();
	bool bIsEnabled;
	vector<button> buttons;
	int opacity;
};
