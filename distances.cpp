#include "distances.h"

#include <cmath>
#include <stdexcept>

double squaredDistance(const Point& a, const Point& b)
{
    if(a.coords.size() != b.coords.size())
    {
        throw std::invalid_argument("Points are from different dimensions");
    }


    double sum = 0.0;

    for(std::size_t i=0; i<a.coords.size(); i++)
    {
        const double diff = a.coords[i] - b.coords[i];
        sum = sum + diff * diff;
    }
    return sum;
}

double euclideanDistance(const Point& a, const Point& b)
{
    return std::sqrt(squaredDistance(a,b));
}