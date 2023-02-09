/*
 * Copyright (C) 2023 onwards LabsLand, Inc.
 * All rights reserved.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 */
#ifndef SIMULATION_COMMUNICATIONS_FILES_H
#define SIMULATION_COMMUNICATIONS_FILES_H

#include <string>
#include <iostream>
#include <fstream>

#include "labsland/simulations/utils/communicator.h"

namespace LabsLand::Simulations::Utils {

    template <class OutputDataType, class InputDataType>
    class SimulationCommunicatorFiles: public SimulationCommunicator<OutputDataType, InputDataType>
    {
        private:
            const std::string inputFilename;
            const std::string outputFilename;
        public:

            SimulationCommunicatorFiles(std::string outputFilename, std::string inputFilename): outputFilename(outputFilename), inputFilename(inputFilename) {}

            /*
             * Receive data from the user interface (web browser). 
             *
             * In this implementation, we use a file to handle this information,
             * where we are constantly reading data from disk.
             *
             * It returns true if something was ready into the structure.
             */
            bool readRequest(InputDataType & request) { 
                std::ifstream ifile(inputFilename);
                if (ifile.is_open()) {
                    std::stringstream buffer;
                    buffer << ifile.rdbuf();
                    if (!ifile) {
                        ifile.close();
                        return false;
                    }

                    std::string serialized = buffer.str();
                    return request.deserialize(serialized);
                }
                return false;
            }
          
            /**
             * Update latest data so the user interface (web browser) receives it. 
             *
             * In this implementation, we use a filen to handle this information,
             * writing the information into disk.
             */
            void sendReport(OutputDataType & report) {
                std::ofstream ofile(outputFilename);
                if (!ofile.is_open())
                    return;

                ofile << report.serialize();
                ofile.close();
            }
    };

}

#endif
