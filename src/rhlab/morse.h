#ifndef MORSE_H
#define MORSE_H

#include "../labsland/simulations/simulation.h"
#include <string>
#include <cstring>
#include <sstream>
#include <stdio.h>

using namespace std;

namespace RHLab::Morse {
    // fixed buffer size   
    const int BUFFER_SIZE = 200;

    // struct that receives the string; NOT USED FOR NOW
    struct MorseRequest : public BaseInputDataType {
        bool deserialize(std::string const & input) {
            return false;
        }
    };

    // struct that tracks the virtual LED states
    struct MorseData : public BaseOutputDataType {
        char buffer[BUFFER_SIZE];
        int lastPos = 0;

        public:
            MorseData() {
                for (int i = 0; i < BUFFER_SIZE; i++){
                    buffer[i] = '\0';
                } 
            }

            void addCharacter(char c) {
                buffer[lastPos] = c;
                lastPos++;
                if (lastPos >= BUFFER_SIZE - 1) {
                     lastPos = 0;
                 }
                buffer[BUFFER_SIZE - 1] = '\0';
            }

            string serialize() const {

                return buffer;
            }


    };

    class MorseSimulation : public Simulation<MorseData, MorseRequest> {
        public:
            MorseSimulation() = default;
            void update(double delta) override;
            void initialize() override;

    };
}

#endif
