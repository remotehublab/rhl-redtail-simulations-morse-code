#include <iostream>
#include <stdio.h>
#include <string>
#include "morse.h"

using namespace std;
using namespace RHLab::Morse;

MorseSimulation::MorseSimulation() {
    // Initialize the morse code dictionary in the constructor
    initializeMorseDictionary();
    currentSequence = "";
}

void MorseSimulation::initializeMorseDictionary() {
    // Letters
    morseToChar[".-"] = 'A';
    morseToChar["-..."] = 'B';
    morseToChar["-.-."] = 'C';
    morseToChar["-.."] = 'D';
    morseToChar["."] = 'E';
    morseToChar["..-."] = 'F';
    morseToChar["--."] = 'G';
    morseToChar["...."] = 'H';
    morseToChar[".."] = 'I';
    morseToChar[".---"] = 'J';
    morseToChar["-.-"] = 'K';
    morseToChar[".-.."] = 'L';
    morseToChar["--"] = 'M';
    morseToChar["-."] = 'N';
    morseToChar["---"] = 'O';
    morseToChar[".--."] = 'P';
    morseToChar["--.-"] = 'Q';
    morseToChar[".-."] = 'R';
    morseToChar["..."] = 'S';
    morseToChar["-"] = 'T';
    morseToChar["..-"] = 'U';
    morseToChar["...-"] = 'V';
    morseToChar[".--"] = 'W';
    morseToChar["-..-"] = 'X';
    morseToChar["-.--"] = 'Y';
    morseToChar["--.."] = 'Z';
    
    // Numbers
    morseToChar[".----"] = '1';
    morseToChar["..---"] = '2';
    morseToChar["...--"] = '3';
    morseToChar["....-"] = '4';
    morseToChar["....."] = '5';
    morseToChar["-...."] = '6';
    morseToChar["--..."] = '7';
    morseToChar["---.."] = '8';
    morseToChar["----."] = '9';
    morseToChar["-----"] = '0';
    
    // Punctuation
    morseToChar[".-.-.-"] = '.';
    morseToChar["--..--"] = ',';
    morseToChar["..--.."] = '?';
    morseToChar[".----."] = '\'';
    morseToChar["-.-.--"] = '!';
    morseToChar["-..-."] = '/';
    morseToChar["-.--."] = '(';
    morseToChar["-.--.-"] = ')';
    morseToChar[".-..."] = '&';
    morseToChar["---..."] = ':';
    morseToChar["-.-.-."] = ';';
    morseToChar["-...-"] = '=';
    morseToChar[".-.-."] = '+';
    morseToChar["-....-"] = '-';
    morseToChar["..--.-"] = '_';
    morseToChar[".-..-."] = '"';
    morseToChar["...-..-"] = '$';
    morseToChar[".--.-."] = '@';
}

void MorseSimulation::translateMorse(char symbol) {
    this->log() << "Translating symbol: " << symbol << endl;
    
    // Process according to the symbol
    if (symbol == '.' || symbol == '-') {
        // Add to current sequence
        currentSequence += symbol;
        this->log() << "Current sequence: " << currentSequence << endl;
    }
    else if (symbol == '/') {
        // Letter space - translate the current sequence
        if (!currentSequence.empty()) {
            char translatedChar = '?';
            
            // Look up in dictionary
            if (morseToChar.find(currentSequence) != morseToChar.end()) {
                translatedChar = morseToChar[currentSequence];
            }
            
            this->log() << "Translated " << currentSequence << " to: " << translatedChar << endl;
            
            // Add to translated text
            this->mState.addTranslatedCharacter(translatedChar);
            
            // Reset current sequence
            currentSequence = "";
        }
    }
    else if (symbol == ' ') {
        // Word space - translate the current sequence and add a space
        if (!currentSequence.empty()) {
            char translatedChar = '?';
            
            // Look up in dictionary
            if (morseToChar.find(currentSequence) != morseToChar.end()) {
                translatedChar = morseToChar[currentSequence];
            }
            
            this->log() << "Translated " << currentSequence << " to: " << translatedChar << endl;
            
            // Add to translated text
            this->mState.addTranslatedCharacter(translatedChar);
            
            // Reset current sequence
            currentSequence = "";
        }
        
        // Add space
        this->mState.addTranslatedCharacter(' ');
    }
}

void MorseSimulation::initialize(){
    this->targetDevice->initializeSimulation({}, {"morseSignal"});
    setReportWhenMarked(true);

    // Initialize default speed thresholds
    updateSpeedThresholds('N'); // Default to normal speed
}

// Update thresholds based on speed setting
void MorseSimulation::updateSpeedThresholds(char speed) {
    switch(speed) {
        case 'F': // Fast
            DOT_THRESHOLD = 0.1;    // 0.1 seconds
            DASH_THRESHOLD = 0.3;  // 0.3 seconds
            LETTER_SPACE = 0.7;     // 0.7 seconds
            WORD_SPACE = 1.0;       // 1.0 seconds
            break;
        case 'N': // Normal
            DOT_THRESHOLD = 1.0;    // 1.0 seconds
            DASH_THRESHOLD = 1.5;   // 1.5 seconds
            LETTER_SPACE = 2.0;     // 2.0 seconds
            WORD_SPACE = 4.0;       // 4.0 seconds
            break;
        case 'S': // Slow
            DOT_THRESHOLD = 3.0;    // 2.0 seconds
            DASH_THRESHOLD = 5.0;   // 3.0 seconds
            LETTER_SPACE = 4.0;     // 4.0 seconds
            WORD_SPACE = 8.0;       // 8.0 seconds
            break;
    }
    
    this->log() << "Speed set to " << speed << 
        " - DOT: " << DOT_THRESHOLD << 
        ", DASH: " << DASH_THRESHOLD << 
        ", LETTER SPACE: " << LETTER_SPACE << 
        ", WORD SPACE: " << WORD_SPACE << endl;
}

// Interpret the signal based on its duration
void MorseSimulation::interpretSignal(bool isHigh, double duration) {
    this->log() << "Interpreting signal: " << (isHigh ? "HIGH" : "LOW") << " with duration " << duration << " s" << endl;
    
    if (isHigh) {
        // Signal was high (mark)
        if (duration < DOT_THRESHOLD) {
            this->mState.addCharacter('.'); // Dot
            translateMorse('.');
        } else {
            this->mState.addCharacter('-'); // Dash
            translateMorse('-');
        }
    } else {
        // Signal was low (space)
        if (duration > WORD_SPACE) {
            this->mState.addCharacter(' '); // Word space
            translateMorse(' ');
        } else if (duration > LETTER_SPACE) {
            this->mState.addCharacter('/'); // Letter space
            translateMorse('/');
        }
    }
}

void MorseSimulation::update(double delta) {
    MorseRequest userRequest;
    bool requestWasRead = readRequest(userRequest);
    if(requestWasRead) {
        this->log() << "Updating speed to: " << userRequest.speed << " clearing: " << userRequest.clearing << endl;;
        
        // Update speed thresholds when speed changes
        updateSpeedThresholds(userRequest.speed);

        // do something with speed or clearing
        if (userRequest.clearing) {
            this->mState.clearBuffer();
            this->currentSequence = ""; // Clear current sequence
            this->log() << "Clearing buffer" << endl;

            // Request state report to update the UI
            requestReportState();
        }
    }

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
        double duration = (currentTime - lastTransitionTime) / (double)this->timeManager->getClocksPerSec();
        
        // Log the duration
        this->log() << "Signal was " << (lastSignal ? "HIGH(1)" : "LOW(0)") << " for " << duration << " seconds" << endl;
        
        // Interpret the previous signal based on its duration
        interpretSignal(lastSignal, duration);
        
        // Update state tracking variables
        lastSignal = currentSignal;
        lastTransitionTime = currentTime;
        // Request state report to update the UI
        requestReportState();
    }
}