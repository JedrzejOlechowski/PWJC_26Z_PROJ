#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <string>
#include <vector>

struct Point {
    std::string id;
    std::vector<double> coords;
};

struct Dataset {
    int dimension = 0;
    std::vector<Point> points;
};

struct Match{
    std::string idA;
    std::string idB;

    double distance = 0.0;
};

struct MatchingResult{
    std::string mode; //użyty tryb do uzyskania wyniku
    std::string status; //status wyniku
    int dimension = 0; //wymiar punktów
    double threshold =0.0;
    double penalty = 0.0;


    std::vector<Match> matches;
    std::vector<std::string> unmatchedA;
    std::vector<std::string> unmatchedB;

    double distanceCost = 0.0;
    double penaltyCost = 0.0;
    double totalCost = 0.0;
};

#endif
