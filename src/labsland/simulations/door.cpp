#include <string>
#include <iostream>
#include "door.h"

void DoorSimulation::initialize()
{

    this->targetDevice->initializeSimulation(
        {"doorOpened", "doorClosed", "personSensor"},
        {"open", "close"});

    setReportWhenMarked(true);
}

void DoorSimulation::update(double delta)
{

    this->log() << "Updating simulation. Delta: " << delta << std::endl;

    DoorRequest request;
    bool requestWasRead = readRequest(request);

    mState.open = this->targetDevice->getGpio("open");
    mState.close = this->targetDevice->getGpio("close");

    this->log() << std::endl
                << "Open door: " << mState.open << "; Close door: " << mState.close << std::endl;

    this->log() << "Input:" << std::endl
                << " Opened: " << request.doorOpened << "; Closed: " << request.doorClosed << "; Person waiting: " << request.personSensor << std::endl;

    this->targetDevice->setGpio("doorOpened", request.doorOpened);
    this->targetDevice->setGpio("doorClosed", request.doorClosed);
    this->targetDevice->setGpio("personSensor", request.personSensor);

    requestReportState();
}