/*
 * Copyright (C) 2023 onwards LabsLand, Inc.
 * All rights reserved.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 */
#include <string>
#include <iostream>
#include <cmath>
#include "watertanksimulation.h"


void WatertankSimulation::initialize() {

    this->targetDevice->initializeSimulation(
            {"lowSensorActive", "midSensorActive", "highSensorActive"},
            {"pump1", "pump2"}
    );

    // Calculate the total volume of the tank.
    // V = pi * r*2 * h (in m3)
    float radius = WATERTANK_DIAMETER / 2;
    mState.totalVolume = M_PI * radius * radius * WATERTANK_HEIGHT * 1000;  // With 2m radius and 6m height: 75.398 m3 == 75398 l.

    // Set the starting volume to the middle.
    mState.volume = mState.totalVolume / 2;
    mState.level = 0.5;

    mState.pump1Active = true;
    mState.pump2Active = true;

    setReportWhenMarked(true);
}


 void WatertankSimulation::update(double delta) {

    this->log() << "Updating simulation. Delta: " << delta << std::endl;

    // Check if we have received any interaction from the 3D environment.
    // TO-DO: Maybe a better API would be exception-based so that it can return by value and be slightly more explicit.
    WatertankRequest request;
    bool requestWasRead = readRequest(request);
    if (requestWasRead) {
        // We do have received one, lets handle it and update the simulation accordingly.

        // Update the demand with the new one the interaction request is telling us to.
        mCurrentDemandFlowrate = request.outputFlow;
    }

    double addedWater = 0;

    mState.pump1Active = this->targetDevice->getGpio("pump1");
    mState.pump2Active = this->targetDevice->getGpio("pump2");

    this->log() << "Pumps: pump1: " << mState.pump1Active << "; pump2: " << mState.pump2Active << std::endl;

    if(mState.pump1Active) {
        // Pump1 is adding water at PUMP1_FLOWRATE liters per second.
        addedWater += PUMP1_FLOWRATE * delta;
    }

    if(mState.pump2Active) {
        // Pump2 is adding water at PUMP2_FLOWRATE liters per second.
        addedWater += PUMP2_FLOWRATE * delta;
    }

    // Water is being removed from the watertank to satisfy the dynamic
    // demand.
    float removedWater = 0;
    removedWater = mCurrentDemandFlowrate * delta;

    this->log() << "Old volume: " << mState.volume << "; adding: " << addedWater << "; removing (flow rate=" << mCurrentDemandFlowrate << "): " << removedWater << std::endl;

    // Update the volume.
    mState.volume = mState.volume + addedWater - removedWater;
    if (mState.volume < 0) {
        mState.volume = 0;
    }
    if (mState.volume > mState.totalVolume) {
        // TO-DO: This would actually be an issue; the watertank is overfilled!
        mState.volume = mState.totalVolume;
    }

    mState.level = mState.volume / mState.totalVolume;

    this->log() << "New volume: " << mState.volume << "; which is level=" << mState.level << std::endl;

    // Update the state of the level sensors according to the watertank level.
    mState.lowSensorActive = mState.level >= 0.20;
    mState.midSensorActive = mState.level >= 0.50;
    mState.highSensorActive = mState.level >= 0.80;
    this->log() << "Sensors: Low (0.2): " << mState.lowSensorActive << "; Mid (0.5): " << mState.midSensorActive << "; High (0.8): " << mState.highSensorActive << std::endl;

    this->targetDevice->setGpio("lowSensorActive", mState.lowSensorActive);
    this->targetDevice->setGpio("midSensorActive", mState.midSensorActive);
    this->targetDevice->setGpio("highSensorActive", mState.highSensorActive);
    requestReportState();
}

