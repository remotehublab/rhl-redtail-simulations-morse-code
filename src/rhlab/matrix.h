
#ifndef MATRIXSIMULATION_H
#define MATRIXSIMULATION_H

#include "../labsland/simulations/simulation.h"
#include <string>
#include <cstring>
#include <sstream>
#include <stdio.h>

using namespace std;

namespace RHLab::LEDMatrix {

    // My constant defines
    #define IS_LITERAL  0
    #define IS_GPIO     1
    #define IS_BUFFER   2
    #define IS_SWITCH   3
    #define IS_LED      4

    #define IS_LITERAL_NEXT_CHAR_SIZE 1
    #define IS_GPIO_NEXT_CHAR_SIZE    2
    #define IS_BUFFER_NEXT_CHAR_SIZE  1
    #define IS_SWITCH_NEXT_CHAR_SIZE  1
    #define IS_LED_NEXT_CHAR_SIZE     1

    #define BUFFER_ARRAY_SIZE   10
    #define LED_ARRAY_SIZE      5
    #define MAX_CHAR_ARRAY_SIZE 1024


    const int INPUTS = 3; // Latch, Pulse and "data out" (from the target device to the simulation)
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
