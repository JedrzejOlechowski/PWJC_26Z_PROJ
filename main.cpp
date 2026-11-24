#include <exception>
#include <filesystem>
#include <iostream>

#include "DataLoader.h"
#include "BasicSolver.h"

int main(int argc, char* argv[]) {
    const std::filesystem::path defaultDirectory = "PWJC_przyklady/data";

    std::filesystem::path pathA = argc > 1
        ? std::filesystem::path(argv[1])
        : defaultDirectory / "basic_A.txt";
    std::filesystem::path pathB = argc > 2
        ? std::filesystem::path(argv[2])
        : defaultDirectory / "basic_B.txt";

    try {
        const Dataset datasetA = readDataset(pathA);
        const Dataset datasetB = readDataset(pathB);

        std::cout << "Zbior A: " << pathA << '\n';
        std::cout << "  wymiar: " << datasetA.dimension << '\n';
        std::cout << "  liczba punktow: " << datasetA.points.size() << '\n';

        std::cout << "Zbior B: " << pathB << '\n';
        std::cout << "  wymiar: " << datasetB.dimension << '\n';
        std::cout << "  liczba punktow: " << datasetB.points.size() << '\n';

        if (datasetA.dimension != datasetB.dimension) {
            std::cerr << "Blad: zbiory maja rozne wymiary\n";
            return 2;
        }

        const double threshold = 1.5;
        const double penalty = 1.0;

        const MatchingResult result = solveBasic(datasetA, datasetB, threshold, penalty);

        std::cout << "Liczba par: " << result.matches.size() << '\n';

        for(const auto& m: result.matches)
        {
            std::cout << "  " << m.idA << " - " << m.idB << " (d = " << m.distance << ")\n";
        }

        std::cout << "Nieprzypisane A: " << result.unmatchedA.size() << ", B: " << result.unmatchedB.size() << '\n';
        std::cout << "Koszt: dist=" << result.distanceCost << " + pen=" << result.penaltyCost << " = " << result.totalCost << '\n';

        std::cout << "Laczna liczba punktow: "
                  << datasetA.points.size() + datasetB.points.size() << '\n';

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Blad wczytywania danych: " << error.what() << '\n';
        return 1;
    }
}
