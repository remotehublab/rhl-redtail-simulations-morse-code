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

    struct MorseRequest : public BaseInputDataType {

        // either 'N' for normal, 'S' for slow, 'F' for fast
        char speed = 'N';
        bool clearing = false;

        // messages are like:
        // "<SPEED>:<CLEARING>"
        // where: 
        // <SPEED> is a string such as "normal", "slow" or "fast"
        // <CLEARING> is either 1 (clearing) or 0 (not clearing)
        bool deserialize(std::string const & input) {
            if (input.size() > 1) {
                size_t pos = input.find(':');
                if (pos != string::npos) {
                    string speed = input.substr(0, pos);
                    string clearing = input.substr(pos + 1, input.size() - (pos + 1));

                    if (speed == "normal") {
                        this->speed = 'N';
                    } else if (speed == "fast") {
                        this->speed = 'F';
                    } else if (speed == "slow") {
                        this->speed = 'S';
                    } else {
                        return false;
                    }

                    if (clearing == "1") {
                        this->clearing = true;
                    } else if (clearing == "0") {
                        this->clearing = false;
                    } else {
                        return false;
                    }
                    
                    return true;
                }
            }
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

            void clearBuffer() {
                for (int i = 0; i < BUFFER_SIZE; i++)  
                    buffer[i] = '\0';
                lastPos = 0;
            }

            void addCharacter(char c) {
                buffer[lastPos] = c;
                lastPos++;
                if (lastPos >= BUFFER_SIZE - 2) {
                     lastPos = 0;
                }
                buffer[lastPos + 1] = '\0';
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
            void interpretSignal(bool isHigh, double duration);

    };
}

#endif
