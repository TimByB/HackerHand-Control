
#include "ofMain.h"
class ofApp;
class BYBGui;
#include "BYBGui.h"
#include "mainAppController.h"
#include "BYBOverlayGui.h"


//========================================================================


int main( ){
#ifdef TARGET_OSX
    //this allows to embed all the data folder inside the app bundle.
    string newRoot = "../Resources/data/";
    ofEnableDataPath();
    ofSetDataPathRoot(newRoot);
#endif

    ofSetupOpenGL(1024,768,OF_WINDOW);

	shared_ptr<mainAppController> mainApp (new mainAppController);
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
