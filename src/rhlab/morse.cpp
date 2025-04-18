#include <iostream>
#include <stdio.h>
#include <string.h>
#include "morse.h"

using namespace std;
using namespace RHLab::Morse;

void MorseSimulation::initialize(){
    this->targetDevice->initializeSimulation({}, {"morseSignal"});
    setReportWhenMarked(true);
}

// Interpret the signal based on its duration
void MorseSimulation::interpretSignal(bool isHigh, double duration) {

    this->log() << "Interpreting signal: " << (isHigh ? "HIGH" : "LOW") << " with duration " << (duration * 1000) << " ms" << endl;
    
    // Define time thresholds for dots, dashes, and spaces
    const double DOT_THRESHOLD = 0.001;        // 1 millisecond
    const double DASH_THRESHOLD = 0.0015;      // 1.5 milliseconds
    const double LETTER_SPACE = 0.002;         // 2 milliseconds
    const double WORD_SPACE = 0.004;           // 4 milliseconds
    
    if (isHigh) {
        // Signal was high (mark)
        if (duration < DOT_THRESHOLD) {
            this->mState.addCharacter('.');  // Dot
        } else {
            this->mState.addCharacter('-');  // Dash
        }
    } else {
        // Signal was low (space)
        if (duration > WORD_SPACE) {
            this->mState.addCharacter(' ');  // Word space
        } else if (duration > LETTER_SPACE) {
            this->mState.addCharacter('/');  // Letter space
        }
    }
}



void MorseSimulation::update(double delta) {
    // Get current signal state
    bool currentSignal = this->targetDevice->getGpio("morseSignal");
    
    // Log the current state
    this->log() << "Checking morseSignal " << delta << " " << currentSignal << endl;
    
    // Static variables to persist between calls
    static bool lastSignal = currentSignal;  // Initialize with current value
    static clock_t lastTransitionTime = this->timeManager->getAbsoluteTime();
    static bool initialized = false;
    
    // Get current timestamp
    clock_t currentTime = this->timeManager->getAbsoluteTime();
    
    // First-time initialization
    if (!initialized) {
        lastSignal = currentSignal;
        lastTransitionTime = currentTime;
        initialized = true;
        this->log() << "Initialized signal tracking" << endl;
        return;
    }
    
    // If signal changed, calculate duration and interpret
    if (currentSignal != lastSignal) {
        // Calculate duration in seconds
        double duration = (currentTime - lastTransitionTime) / (double)CLOCKS_PER_SEC;
        
        // Log the duration
        this->log() << "Signal was " << (lastSignal ? "HIGH(1)" : "LOW(0)") << " for " << duration << " seconds" << endl;
        
        // Interpret the previous signal based on its duration
        interpretSignal(lastSignal, duration);
        
        // Update state tracking variables
        lastSignal = currentSignal;
        lastTransitionTime = currentTime;
    }
    
    // Request state report to update the UI
    requestReportState();
}
