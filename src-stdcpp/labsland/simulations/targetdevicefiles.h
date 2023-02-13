/*
 * Copyright (C) 2023 onwards LabsLand, Inc.
 * All rights reserved.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 */
#ifndef LL_TARGET_DEVICE_STD
#define LL_TARGET_DEVICE_STD

#include <string>
#include "labsland/simulations/targetdevice.h"

namespace LabsLand::Utils {

    class TargetDeviceFiles: public TargetDevice {

        private:
            const std::string inputFilename;
            const std::string outputFilename;
            const int numberOfOutputs;
            const int numberOfInputs;

            int numberOfSimulationOutputs;
            int numberOfSimulationInputs;

            std::string getOutputValues();

        public:
            TargetDeviceFiles(std::string outputFilename, std::string inputFilename, int numberOfOutputs, int numberOfInputs);

            /*
             * Does it support this number of inputs and outputs?
             */
            virtual bool checkSimulationSupport(int outputGpios, int inputGpios);

            /*
             * Allocate a set of outputs and inputs, in whichever order the device defines.
             *
             * It returns true/false if possible.
             */
            virtual bool initializeSimulation(int outputGpios, int inputGpios);

            /*
             * Reset to the default state of the target device (e.g., all GPIOs available for regular use)
             */
            virtual void resetAfterSimulation();

            /*
             * If custom serial is going to be used, initialize it. It will return false if not possible.
             */
            virtual bool initializeCustomSerial();

            virtual std::ostream& log();

            // Add other protocols in the future

            /*
             * Regular operations with GPIO (set/get) based on the position.
             * Important: setGpio(1) does not set GPIO1 to 1: it sets to 1 whichever is the second
             * output GPIO for simulations reserved. In the example above, where the simulation 
             * reserved GPIO17, GPIO16, GPIO14, GPIO13, GPIO12, setGpio(1) is GPIO16=1
             */
            virtual void setGpio(int outputPosition, bool value = true);
            virtual void resetGpio(int outputPosition);
            virtual bool getGpio(int inputPosition);

            /**
             * Same, but using custom names
             */
            virtual void setGpio(NamedGpio outputPosition, bool value = true);
            virtual void resetGpio(NamedGpio outputPosition);
            virtual bool getGpio(NamedGpio inputPosition);
    };
}

#endif
