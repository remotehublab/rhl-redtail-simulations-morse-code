#ifndef MORSE_H
#define MORSE_H

#include "../labsland/simulations/simulation.h"
#include <string>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <map>

using namespace std;

namespace RHLab::Morse {
    // fixed buffer size   
    const int BUFFER_SIZE = 200;
    const int TEXT_BUFFER_SIZE = 100;

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
        char buffer[BUFFER_SIZE];          // Morse code buffer (dots, dashes, spaces)
        char translatedText[TEXT_BUFFER_SIZE]; // Translated text buffer
        int lastPos = 0;
        int lastTextPos = 0;

        public:
            MorseData() {
                for (int i = 0; i < BUFFER_SIZE; i++){
                    buffer[i] = '\0';
                } 
                for (int i = 0; i < TEXT_BUFFER_SIZE; i++){
                    translatedText[i] = '\0';
                }
            }

            void clearBuffer() {
                for (int i = 0; i < BUFFER_SIZE; i++)  
                    buffer[i] = '\0';
                for (int i = 0; i < TEXT_BUFFER_SIZE; i++)
                    translatedText[i] = '\0';
                lastPos = 0;
                lastTextPos = 0;
            }

            void addCharacter(char c) {
                buffer[lastPos] = c;
                lastPos++;
                if (lastPos >= BUFFER_SIZE - 2) {
                     lastPos = 0;
                }
                buffer[lastPos + 1] = '\0';
            }

            void addTranslatedCharacter(char c) {
                translatedText[lastTextPos] = c;
                lastTextPos++;
                if (lastTextPos >= TEXT_BUFFER_SIZE - 2) {
                    lastTextPos = 0;
                }
                translatedText[lastTextPos + 1] = '\0';
            }

            string serialize() const {
                return translatedText;
            }
    };


    class MorseSimulation : public Simulation<MorseData, MorseRequest> {
        private:
            // Speed threshold variables
            double DOT_THRESHOLD = 1.0;
            double DASH_THRESHOLD = 1.5;
            double LETTER_SPACE = 2.0;
            double WORD_SPACE = 4.0;
            
            // Morse code translator
            std::map<std::string, char> morseToChar;
            
            // Current morse sequence being built
            std::string currentSequence;
            
            // Process morse code and update translated text
            void translateMorse(char symbol);
            
            // Initialize the morse code dictionary
            void initializeMorseDictionary();

        public:
            MorseSimulation();
            void update(double delta) override;
            void initialize() override;
            void interpretSignal(bool isHigh, double duration);
            void updateSpeedThresholds(char speed);
    };
}

#endif