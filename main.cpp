#include <iostream>
#include "watertanksimulation.h"

int main() {

    WatertankSimulation simulation;
    std::clock_t currentClock = clock();

    int i = 0;
    simulation._initialize();

    while(i < 100) {

        currentClock += 0.1 * CLOCKS_PER_SEC; // Make the simulation advance 100 ms.
        simulation._update(currentClock);
        i++;

        std::cout << "Current water level: " << simulation.mState.level << std::endl;
        std::cout << "Current volume out of total: " << simulation.mState.volume << " l out of " << simulation.mState.totalVolume << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
