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

#endif
