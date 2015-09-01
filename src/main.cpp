//#define USE_SHARED_FONTS
#include "ofMain.h"
class ofApp;
class BYBGui;
#include "BYBGui.h"
#include "ofApp.h"
#include "BYBOverlayGui.h"
//#include "ofAppGlutWindow.h"

//========================================================================


int main( ){
#ifdef TARGET_OSX
    string newRoot = "../Resources/data/";
    ofEnableDataPath();
    ofSetDataPathRoot(newRoot);
#endif
//	ofAppGlutWindow window;
	//ofSetupOpenGL(&window, 1024,768,OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(1024,768,OF_WINDOW);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
#ifdef USE_SHARED_FONTS
	shared_ptr<map<string,ofTrueTypeFont> > fonts (new map<string,ofTrueTypeFont>);
#endif
    //map<string,ofTrueTypeFont> fonts;
    
	//	fonts["HelveticaNeueLTStd-Md"].load("fonts/HelveticaNeueLTStd-Md.otf", 15);
	
	//BYBOverlayGui::fonts = fonts;

	shared_ptr<ofApp> mainApp (new ofApp);
	shared_ptr<BYBGui> gui (new BYBGui);
	mainApp->gui = gui;
	gui->controllerPtr = mainApp;
    
    ofDirectory dir;
    dir.allowExt("ttf");
    dir.allowExt("otf");
    dir.listDir("fonts");
    for (int i = 0; i < dir.size(); i++ ){
        ofTrueTypeFont f;
        if(f.load(dir.getPath(i),12, true, true)){
            gui->fonts[ofFilePath::removeExt(dir.getName(i))] = f;
            cout << "added Font: " << ofFilePath::removeExt(dir.getName(i)) << endl;
        }
        if(f.load(dir.getPath(i),25, true, true)){
            gui->fonts[ofFilePath::removeExt(dir.getName(i)) + "_25"] = f;
        }
    }


    
    try{
	ofRunApp(mainApp);
    }catch(exception& e){cout << e.what() << '\n';}
    catch (int param) { cout << "int exception"; }
    catch (char param) { cout << "char exception"; }
    catch (...) { cout << "default exception"; }
}
