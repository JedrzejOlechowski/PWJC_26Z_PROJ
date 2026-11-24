#ifndef BASICSOLVER_H
#define BASICSOLVER_H

#include "DataModel.h"

MatchingResult solveBasic(const Dataset& setA, const Dataset& setB, double threshold, double penalty);

#endif