#ifndef SIMULATION_COMMUNICATIONS_H
#define SIMULATION_COMMUNICATIONS_H

namespace LabsLand::Simulations::Utils {

    template <class OutputDataType, class InputDataType>
    class SimulationCommunicator
    {
        public:
           /*
            * Receive data from the user interface (web browser). Override
            * request only if data is provided (in which case, return true).
            * 
            * IMPORTANT: internally, if the user interface sends information
            * very fast and your simulation makes long sleeps, you might miss
            * information as the requests can be overriden by newer requests.
            * Ideally, send always the full state (instead of only the diffs),
            * and do not use sleep too much.
            */
            virtual bool readRequest(InputDataType & request) = 0;
         
           /**
            * Update latest data so, when the user interface (web browser)
            * tries to receive this information, it will receive the report
            * sent the last time this method was called.
            *
            * IMPORTANT: this means that data is not guaranteed to be received
            * by the user interface. If you send 3 reports by the time the
            * web interface requests data, the previous 2 reports will never be
            * read. So store the full state (instead of diffs).
            */
            virtual void sendReport(OutputDataType & report) = 0;
    };

}

#endif
