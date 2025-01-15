#include <iostream>
#include <stdio.h>
#include <string.h>
#include "matrix.h"

using namespace std;
using namespace RHLab::LEDMatrix;

void MatrixSimulation::initialize(){
    this->targetDevice->initializeSimulation(OUTPUTS, INPUTS);

    setReportWhenMarked(true);
}

void MatrixSimulation::update(double delta) {

}