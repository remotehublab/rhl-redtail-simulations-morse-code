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

    clock_t startTime = this->timeManager->getAbsoluteTime();

    while ( this->targetDevice->getGpio("morseSignal") == 1) ;

    clock_t endTime = this->timeManager->getAbsoluteTime();

    this->log() << "Was 1 for " << (endTime - startTime) << endl;

    requestReportState();
    if (this->targetDevice->getGpio("morseSignal") == 0) {
        return;
    }
}
