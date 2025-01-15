#include <iostream>
#include <stdio.h>
#include <string.h>
#include "matrix.h"

using namespace std;
using namespace RHLab::LEDMatrix;

void MatrixSimulation::initialize(){
    this->targetDevice->initializeSimulation({}, {"latch", "pulse", "data"});

    setReportWhenMarked(true);
}

void MatrixSimulation::update(double delta) {
    if (this->targetDevice->getGpio("latch") == 0){
        return;
    }

    // if latch is one, then wait until it becomes 0 and then start waiting for bits
    int bitsNumber = COLS * ROWS * BITS_PER_LED;
    bool targetDeviceInputData[bitsNumber];


    if (readSerialCommunication(targetDeviceInputData, bitsNumber)) {

        // if we receive any communication, then we do this
        this->mState.setLed(0, 0, 'R');

        // and everything else
    }

}