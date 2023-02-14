#include "targetdevice.h"
#include <algorithm>

using namespace LabsLand::Utils;


bool TargetDevice::initializeSimulation(std::vector<std::string> outputGpios, std::vector<std::string> inputGpios) {
    bool succeeded = this->initializeSimulation(outputGpios.size(), inputGpios.size());
    if (!succeeded)
        return false;

    this->outputLabels = outputGpios;
    this->inputLabels = inputGpios;

    return true;
}

void TargetDevice::setGpio(std::string outputPosition, bool value) {
    auto it = std::find (this->outputLabels.begin(), this->outputLabels.end(), outputPosition);
    // Not found
    if (it == this->outputLabels.end())
        return;

    int position = it - this->outputLabels.begin();
    setGpio(position, value);
}

void TargetDevice::resetGpio(std::string outputPosition) {
    this->setGpio(outputPosition, false);
}

bool TargetDevice::getGpio(std::string inputPosition) {
    auto it = std::find (this->inputLabels.begin(), this->inputLabels.end(), inputPosition);
    // Not found
    if (it == this->inputLabels.end())
        return false;

    int position = it - this->inputLabels.begin();
    return getGpio(position);
}

