
#ifndef MATRIXSIMULATION_H
#define MATRIXSIMULATION_H

#include "../labsland/simulations/simulation.h"
#include <string>
#include <cstring>
#include <sstream>
#include <stdio.h>

using namespace std;

namespace RHLab::LEDMatrix {

    bool readSerialCommunication(bool buffer[], int bits) {
        // we read latch, if no communication, just return false

        // if we read that number of bits, then true
        return true;
    }

    bool MatrixSimulation::readSerialCommunication(bool buffer, int bits) {
        for (int i = 0; i < bits; i += 2) {
            // Wait for a rising edge on the pulse GPIO
            while (this->targetDevice->getGpio("pulse") == 0) {}

            // Read the data bit when the pulse is high
            bufferGreen[i] = this->targetDevice->getGpio("red");
            bufferRed[i+1] = this->targetDevice->getGpio("green");

            // Wait for the pulse to go low again before reading the next bit
            while (this->targetDevice->getGpio("pulse") == 1) {}
        }

        // Return true to indicate successful reading
        return true;
    }

    
    const int INPUTS = 4; // Latch, Pulse and data Green, data Red (from the target device to the simulation)
    const int OUTPUTS = 0; // No need for any data in

    const int COLS = 16;
    const int ROWS = 16;
    const int BITS_PER_LED = 2; // 00 = off; 01 = green; 10 = red; 11 = yellow

    // struct that receives the string; NOT USED FOR NOW
    struct MatrixRequest : public BaseInputDataType {
        bool deserialize(std::string const & input) {
            return false;
        }
    };

    // struct that tracks the virtual LED states
    struct MatrixData : public BaseOutputDataType {
        char leds[ROWS * COLS];

        public:
            MatrixData() {
                for (int row = 0; row < ROWS; row++) {
                    for (int col = 0; col < COLS; col++){
                        leds[row * COLS + col] = 'B';
                    }
                }
            }

            void setLed(int row, int col, char color) {
                if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
                    return;
                }
                leds[row * COLS + col] = color;
            }

            string serialize() const {
                stringstream stream;

                for (int row = 0; row < ROWS; row++) {
                    for (int col = 0; col < COLS; col++){
                        stream << leds[row * COLS + col];
                    }
                    if(row < ROWS - 1){
                        stream << ":";
                    }
                }

                return stream.str();
            }
    };

    class MatrixSimulation : public Simulation<MatrixData, MatrixRequest> {
        public:
            MatrixSimulation() = default;
            void update(double delta) override;
            void initialize() override;

    };
}

#endif
