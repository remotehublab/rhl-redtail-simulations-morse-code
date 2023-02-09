#include "targetdevicefiles.h"

using namespace std;
using namespace LabsLand::Utils;

TargetDeviceFiles::TargetDeviceFiles(string inputFilename, string outputFilename): inputFilename(inputFilename), outputFilename(outputFilename) {

}

bool TargetDeviceFiles::checkSimulationSupport(int outputGpios, int inputGpios) {
    // TODO
    return true;
}

bool TargetDeviceFiles::initializeSimulation(int outputGpios, int inputGpios) {
    // TODO
    return true;
}

void TargetDeviceFiles::resetAfterSimulation() {
    // TODO
}

bool TargetDeviceFiles::initializeCustomSerial() {
    // TODO
    return true;
}

void TargetDeviceFiles::setGpio(int outputPosition, bool value) {
    // TODO
}

void TargetDeviceFiles::resetGpio(int outputPosition) {
    // TODO
}

bool TargetDeviceFiles::getGpio(int inputPosition) {
    // TODO
    return false;
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
