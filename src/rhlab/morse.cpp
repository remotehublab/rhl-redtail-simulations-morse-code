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


// count for how long a particular signal is happening
void MorseSimulation::update(double delta) {
    // If latch is low, there's nothing to process.
    this-> log() << "Checking morseSignal " << delta << " " << this->targetDevice->getGpio("morseSignal") << endl;
    this->log() << this->timeManager->getAbsoluteTime() << endl;

    if ( this->targetDevice->getGpio("morseSignal")  == 0 ) { 
        this->mState.addCharacter('a');
    } else {
        this->mState.addCharacter('b');
    }

    // capture the time when the signal was high(1) and low(0)
    clock_t startTime = this->timeManager->getAbsoluteTime();
    bool signalWasHigh = this->targetDevice->getGpio("morseSignal") == 1;
    double signalDuration = 0;

    // wait until the signal changes or a timeout occurs
    const double MAX_WAIT_TIME = 1.0; // in seconds
    while ((this->targetDevice->getGpio("morseSignal") == 1) == signalWasHigh) {
        signalDuration = (this->timeManager->getAbsoluteTime() - startTime) / CLOCKS_PER_SEC;
        if (signalDuration > MAX_WAIT_TIME) break;
    }
    
    this->log() << "Signal was " << (signalWasHigh ? "HIGH(1)" : "LOW(0)") << " for " << signalDuration << " seconds" << endl;

    requestReportState();
    if (this->targetDevice->getGpio("morseSignal") == 0) {
        return;
    }
}
