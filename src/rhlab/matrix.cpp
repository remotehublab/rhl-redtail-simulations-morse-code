#include <iostream>
#include <stdio.h>
#include <string.h>
#include "matrix.h"

using namespace std;
using namespace RHLab::LEDMatrix;

void MatrixSimulation::initialize(){
    this->targetDevice->initializeSimulation({}, {"latch", "pulse", "green", "red"});

    setReportWhenMarked(true);
}

bool MatrixSimulation::readSerialCommunication(bool buffer[], int bits) {
    try {
        for (int i = 0; i < bits; i += 2) {
            // Wait for a rising edge on the pulse GPIO
            while (this->targetDevice->getGpio("pulse") == 0) {}

            // Read the data bit when the pulse is high
            buffer[i] = this->targetDevice->getGpio("red");
            buffer[i+1] = this->targetDevice->getGpio("green");

            // Wait for the pulse to go low again before reading the next bit
            while (this->targetDevice->getGpio("pulse") == 1) {}
        }
    } catch (exception e) {
        // TODO: add some form of exception dump or handling
        return false;
    }
    
    // Return true to indicate successful reading
    return true;
}

void MatrixSimulation::update(double delta) {
    // If latch is low, there's nothing to process.
    if (this->targetDevice->getGpio("latch") == 0) {
        return;
    }

    // Number of bits to read from the device
    int bitsNumber = COLS * ROWS * BITS_PER_LED;
    bool targetDeviceInputData[bitsNumber];

    // Wait for latch to become 0
    while (this->targetDevice->getGpio("latch") == 1) {}

    // Process the received data and update the matrix
    if (readSerialCommunication(targetDeviceInputData, bitsNumber)) {
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                // Map the received data to LED matrix states
                int bitIndex = (row * COLS + col) * BITS_PER_LED;
                bool red = targetDeviceInputData[bitIndex];
                bool green = targetDeviceInputData[bitIndex+1];
                char color = ' ';
                if (red && green) {
                    color = 'Y';
                } else if (green) {
                    color = 'G';
                } else if (red) {
                    color = 'R';
                } else {
                    color = 'B';
                }
                this->mState.setLed(row, col, color);
            }
        }
    }
}
