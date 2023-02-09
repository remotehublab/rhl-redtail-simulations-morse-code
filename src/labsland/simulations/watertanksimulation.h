/*
 * Copyright (C) 2023 onwards LabsLand, Inc.
 * All rights reserved.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 */
#ifndef HYBRIDAPI_WATERTANKSIMULATION_H
#define HYBRIDAPI_WATERTANKSIMULATION_H

#include "simulation.h"
#include <string>
#include <sstream>

/**
 * The WatertankData structure contains data that is to be transmitted to the 3D environment.
 */
struct WatertankData : public BaseOutputDataType {

    // Level percent.
    float level;

    // Total volume;
    float totalVolume;

    // Current occupied volume.
    float volume;

    // Whether the first pump is currently active.
    bool pump1Active;

    // Whether the second pump is currently active.
    bool pump2Active;

    // Temperature of the first pump, in Celsius.
    float pump1Temperature;

    // Temperature of the second pump, in Celsius.
    float pump2Temperature;

    // Current load, in liters per second.
    float currentLoad;

    // State of the three sensors at 20%, 50% and 80% height respectively.
    bool lowSensorActive;
    bool midSensorActive;
    bool highSensorActive;

    /**
     * Serializes the structure into a string that can be transmitted through serial.
     */
    std::string serialize() const {
        std::stringstream stream;
        stream << level << "&" << totalVolume << "&" << volume << "&" << pump1Active << "&" << pump2Active << "&" <<
            pump1Temperature << "&" << pump2Temperature << "&" << currentLoad << "&" << lowSensorActive << "&" <<
            midSensorActive << "&" << highSensorActive << "&";
        return stream.str();
    }
};

/**
 * The WatertankRequest structure receives requests and interactions from the 3D environment. In the case of the
 * watertank simulation, the load (flowrate) varies dynamically, and is set externally.
 */
struct WatertankRequest : public BaseInputDataType {
    float outputFlow;

    bool deserialize(std::string const & input) {
        std::stringstream stream(input);
        stream >> outputFlow;
        return true;
    }
};


/**
 * The watertank simulation implements the model of a watertank with a variable output charge. The model has two pumps
 * that are user-controlled and that raise in temperature when active. Those pumps fill the watertank at a constant
 * flowrate when active. Each pump has a temperature sensor. The controller does not receive the temperature itself
 * but receives two bits that indicate whether the pumps are overheating. There are also three level sensors, one
 * bit each, that indicate whether the water level is over them. They are placed at a 10%, 50% and 90% height respectively.
 */
class WatertankSimulation : public Simulation<WatertankData, WatertankRequest> {
private:

    // Watertank height in meters.
    const float WATERTANK_HEIGHT = 6.0f;
    // Watertank diameter in meters.
    const float WATERTANK_DIAMETER = 4.0f;

    const float PUMP1_FLOWRATE = 50;
    const float PUMP2_FLOWRATE = 50;

    float mCurrentDemandFlowrate = 60; // liters per second.

public:

    WatertankSimulation() = default;

    virtual void update(double delta) override;

    virtual void initialize() override;
};


#endif //HYBRIDAPI_WATERTANKSIMULATION_H
