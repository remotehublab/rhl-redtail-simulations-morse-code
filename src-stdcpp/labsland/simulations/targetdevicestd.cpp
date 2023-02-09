#include "targetdevicestd.h"

using namespace LabsLand::Utils;

TargetDeviceStd::TargetDeviceStd() {
}

bool TargetDeviceStd::checkSimulationSupport(int outputGpios, int inputGpios) {
    // TODO
    return true;
}

bool TargetDeviceStd::initializeSimulation(int outputGpios, int inputGpios) {
    // TODO
    return true;
}

void TargetDeviceStd::resetAfterSimulation() {
    // TODO
}

bool TargetDeviceStd::initializeCustomSerial() {
    // TODO
    return true;
}

void TargetDeviceStd::setGpio(int outputPosition, bool value) {
    // TODO
}

void TargetDeviceStd::resetGpio(int outputPosition) {
    // TODO
}

bool TargetDeviceStd::getGpio(int inputPosition) {
    // TODO
    return false;
}

void TargetDeviceStd::setGpio(NamedGpio outputPosition, bool value) {
    // TODO
}

void TargetDeviceStd::resetGpio(NamedGpio outputPosition) {
    // TODO
}

bool TargetDeviceStd::getGpio(NamedGpio inputPosition) {
    // TODO
    return false;
}
