/*
 * Copyright (C) 2023 onwards LabsLand, Inc.
 * All rights reserved.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 */
#include <iostream>
#include <fstream>
#include <sstream>

#include "targetdevicefiles.h"

using namespace std;
using namespace LabsLand::Utils;
using namespace LabsLand::Protocols;

TargetDeviceFiles::TargetDeviceFiles(string outputFilename, string inputFilename, int numberOfOutputs, int numberOfInputs): inputFilename(inputFilename), outputFilename(outputFilename), numberOfOutputs(numberOfOutputs), numberOfInputs(numberOfInputs) {

}

bool TargetDeviceFiles::checkSimulationSupport(TargetDeviceConfiguration * configuration) {
    return configuration->getOutputGpios() <= this->numberOfOutputs && configuration->getInputGpios() <= this->numberOfInputs;
}

bool TargetDeviceFiles::initializeSimulation(TargetDeviceConfiguration * configuration) {
    if (!checkSimulationSupport(configuration))
        return false;

    this->numberOfSimulationOutputs = configuration->getOutputGpios();
    this->numberOfSimulationInputs = configuration->getInputGpios();
    this->configuration = configuration;

    // Initialize the file
    ofstream ofile(this->outputFilename);
    for (int i = 0; i < this->numberOfSimulationOutputs; ++i)
        ofile << "0";

    ofile.close();
    return true;
}

void TargetDeviceFiles::resetAfterSimulation() {
    this->numberOfSimulationOutputs = 0;
    this->numberOfSimulationInputs = 0;

    ofstream ofile(this->outputFilename);
    ofile.close();
}

bool TargetDeviceFiles::initializeCustomSerial() {
    // TODO
    return true;
}

string TargetDeviceFiles::getOutputValues() {
    ifstream ifile(this->outputFilename);
    stringstream buffer;
    buffer << ifile.rdbuf();
    string gpios = buffer.str();
    ifile.close();

    while(gpios.size() < this->numberOfSimulationOutputs)
        gpios += "0";

    if (gpios.size() > this->numberOfSimulationOutputs)
        gpios = gpios.substr(0, this->numberOfSimulationOutputs);

    return gpios;
}

ostream& TargetDeviceFiles::log() {
    return cout;
}

void TargetDeviceFiles::setGpio(int outputPosition, bool value) {
    string currentOutputs = this->getOutputValues();
    if (outputPosition > currentOutputs.size()) {
        return;
    }

    currentOutputs[outputPosition] = value?'1':'0';
    ofstream ofile(this->outputFilename);
    ofile << currentOutputs;
    ofile.close();
}

void TargetDeviceFiles::resetGpio(int outputPosition) {
    this->setGpio(outputPosition, false);
}

bool TargetDeviceFiles::getGpio(int inputPosition) {
    ifstream ifile(this->inputFilename);
    stringstream buffer;
    buffer << ifile.rdbuf();
    string gpios = buffer.str();
    ifile.close();
    if (inputPosition >= gpios.size())
        return false;

    return gpios[inputPosition] == '1';
}

void TargetDeviceFiles::setGpio(NamedGpio outputPosition, bool value) {
    // TODO
}

void TargetDeviceFiles::resetGpio(NamedGpio outputPosition) {
    // TODO
}

bool TargetDeviceFiles::getGpio(NamedGpio inputPosition) {
    // TODO
    return false;
}
