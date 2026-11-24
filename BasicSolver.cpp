#include "BasicSolver.h"
#include "distances.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace{

    struct CandidatePair{
        std::size_t indexA;
        std::size_t indexB;
        double distance;
    };
}

MatchingResult solveBasic(const Dataset& setA, const Dataset& setB, double threshold, double penalty)
{
    if(setA.dimension != setB.dimension)
    {
        throw std::invalid_argument("Punkty są w innych wymiarach");
    }

    const double effectiveThreshold = threshold + 1e-9 * std::max(1.0, threshold); //uwzględnienie błędu numerycznego do 9go miejsca
    const double maxUsefulDistance = std::min(effectiveThreshold, 2.0*penalty); //maksymalna odległośc opłacalna do łączenia
    const double maxUsefulDistanceSQ = maxUsefulDistance * maxUsefulDistance;

    std::vector<CandidatePair> candidates;


    for(std::size_t i=0; i<setA.points.size(); ++i)
    {
        for(std::size_t j=0; j<setB.points.size(); ++j)
        {
            const double distanceSQ = squaredDistance(setA.points[i], setB.points[j]);

            if(distanceSQ <= maxUsefulDistanceSQ)
            {
                const double dist = std::sqrt(distanceSQ);

                if(dist <= maxUsefulDistance)
                {
                    candidates.push_back({i,j,dist});
                }
            }
        }
    }


    std::sort(candidates.begin(), candidates.end(), [](const CandidatePair& lhs, const CandidatePair& rhs)
    {return lhs.distance < rhs.distance;
    });

    std::vector<bool> usedA(setA.points.size(), false);
    std::vector<bool> usedB(setB.points.size(), false);

    MatchingResult result;
    result.mode = "basic";
    result.status = "feasible";
    result.dimension = setA.dimension;
    result.threshold = threshold;
    result.penalty = penalty;


    for(const auto& candidate : candidates)
    {
        if(!usedA[candidate.indexA] && !usedB[candidate.indexB])
        {
            usedA[candidate.indexA] = true;
            usedB[candidate.indexB] = true;

            result.matches.push_back({setA.points[candidate.indexA].id, setB.points[candidate.indexB].id, candidate.distance});
            result.distanceCost += candidate.distance;
        }
        
    }

    for(std::size_t i=0; i<setA.points.size(); ++i)
    {
        if(!usedA[i])
        {
            result.unmatchedA.push_back(setA.points[i].id);
        }
    }

    for(std::size_t i=0; i<setB.points.size(); ++i)
    {
        if(!usedB[i])
        {
            result.unmatchedB.push_back(setB.points[i].id);
        }
    }

    const std::size_t totalUnmatched = result.unmatchedA.size()+result.unmatchedB.size();
    result.penaltyCost = penalty * static_cast<double>(totalUnmatched);
    result.totalCost = result.distanceCost + result.penaltyCost;

    return result;

}