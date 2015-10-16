#[BackyardBrains](https://backyardbrains.com) HackerHand Control

###This repository hosts the software made to control the [Hackerhand](https://backyardbrains.com/products/HackerHand) using electrodes attached to the users forearm.


#####Install
If you wish to install and use this software only please download the compiled version of it from [Backyard Brains website.](https://backyardbrains.com/products/HackerHand)


######Compiling
If you want to modify this software, thus having to compile it, you must first install the following.

1. [OpenFrameworks v0.84](http://openframeworks.cc/download/)
2. The required openFrameworks addons
	1. [ofxModifierKeys](https://github.com/satoruhiga/ofxModifierKeys.git)
	2. [ofxLearn](https://github.com/roymacdonald/ofxLearn/tree/byb)
	
To install addons you just need to copy them to the addons folder of your openframeworks installation

Once all these are installed, copy this repo to the apps/BYB/ folder of your openframworks instalation. it is very important that you respect the folder structure as if you don't the projects will not compile.

##About this software


The Backyard Brains HackerHand software consists of two pieces
The Arduino Sketch that allows our 5 channel SpikerShield to talk to the Arduino (you will most likely not ever change this).
The Laptop Software that analyzes the 5 channels of EMG, decodes classifies the signals, and controls the hand. 

###Arduino Sketch
Reads the 5 Analog Input Channels (the amplified EMG signals-Analog In 0-4) and sends the signals to the computer via serial USB port. The HackerHand Software then analyses the signales, and when it decodes a finger movement, sends output commands via the same USB Port to the Digital Outs on the Arduino can then control the Robotic Hand. The Digital Outs used 8-12. You can find the arduino sketch by pressing the arduino button in the HackerHand Control software.

###Computer Software
The software is written in Open Frameworks, an open C++ tool. The Arduino + 5 Channel EMG SpikerShield must be plugged into your computer for the Software to Correctly Load.

####How to use
-To manually move the HackerHand fingers, press keys 1 (thumb), 2 (index), 3 (middle), 4 (ring), and 5 (pinky) to get the fingers to contract. Pressing the numbers again will relax the fingers. 
-To reset the levels on the visualization, press “r” on the keypad
-The calibrate the system, select the algorithm you want to use (for now “euclidean” or “SVM” - support vector machine), and then select “calibrate” and follow the onscreen instructions

###The algorithm

The algorithm, the algorithm, the algorithm controls all. The most dificult part of this project is the signal classification, which is analyzing the five EMG signals coming into the hand and decoding which finger is moving. We have built this software to allow you to learn and play with different algorithms to try to achieve the best finger movement classification.

This software, written in C++, using the openFrameworks creative coding tool. It is mainly built using the [MVC](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller) scheme so that the graphical interface coding doesn’t interfere with the coding of the "behind the scenes" processes.
The GUI is really simple and intuitive and there's not much more from what you see when you just start the app.
The internal workflow of the app is as follows:

#####Data Acquisition
Handled by the [serialManager class](https://github.com/TimByB/HackerHand-Control/blob/master/src/serialManager.h). It gets the signal readings from the arduino via serial port, parses the serial data and notifies the [mainAppController](https://github.com/TimByB/HackerHand-Control/blob/master/src/mainAppController.h) that new data has arrived.
Each time the mainAppController gets a notification about new data it processes it by applying a low pass filter, performing peak detection and classify the signals using the selected algorithm.

- [Lo pass filter (LPF)](https://github.com/TimByB/HackerHand-Control/blob/master/src/loPass.h): this uses an extremely simple, yet effective algorithm. As it just averages defined number of successive samples. The number of samples can be modified by using the secret gui (which can be activated by modifying the settings.xml file).

- [Peak detection:](https://github.com/TimByB/HackerHand-Control/blob/master/src/peakDetector.h) This is a time and gradient based peak detector rather that a threshold/level one. It scans the incoming signal for a sample that is larger than all the other samples within the time window, in which also all the previous samples must have a positive slope (dx/dy) and all the following ones must have a negative slope.
After several trials with several algorithms this one proved to be the most effective and less prone to error. The only parameter that can be adjusted is the time window. This has been adjusted to perform well for most users, yet you can still tweak it using the secret gui.

- [Classification:](https://github.com/TimByB/HackerHand-Control/tree/master/src/FingerClassifier) This part of the process is the tricky one. It uses supervised Machine Learning (ML) algorithms to be able to classify the signals. This means that the first thing to do is to “teach” or calibrate the algorithm, by telling it how the signals must look when a certain finger is moved. Once calibrated the algorithm tries to find classify according to what it has been taught. For these we’ve implemented, so far, one well known ML algorithm, the Support Vector Machines (SVM) and a Matrix Euclidean Distance algorithm, which simply computes the euclidean distance from the incoming data matrix to the learnt and classified data matrices. The signal will get labeled according to the closest learnt data matrix. The SVM algorithm performs by far much better that the Euclidean. The SVM classification is performed using the dLib open-sourced library.

Once classification has been performed, the GUI gets updated accordingly and the serialManager class gets notified about the classification so it can send a signal to the arduino in order to move the corresponding finger.

###Your own algorithm
If you want to modify the algorithm the modify the [according .h and .cpp files](https://github.com/TimByB/HackerHand-Control/blob/master/src/FingerClassifier/).
Once done, recompile the whole app. (you’ll need to download, install and setup openFrameworks.
In the future a simpler way will be provided in which there will be no need to recompile the whole app, just the new classifier.
