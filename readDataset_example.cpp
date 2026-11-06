#include "DataLoader.h"

#include <filesystem>
#include <iostream>

int main() {
    const std::filesystem::path projectRoot = std::filesystem::current_path();
    const auto aPath = projectRoot / "PWJC_przyklady" / "data" / "basic_A.txt";
    const auto bPath = projectRoot / "PWJC_przyklady" / "data" / "basic_B.txt";

    try {
        const Dataset a = readDataset(aPath);
        const Dataset b = readDataset(bPath);

        std::cout << "A dimension: " << a.dimension << "\n";
        std::cout << "A points: " << a.points.size() << "\n";
        std::cout << "First A point: " << a.points[0].id << " -> ";
        for (double value : a.points[0].coords) {
            std::cout << value << ' ';
        }
        std::cout << "\n";

        std::cout << "B dimension: " << b.dimension << "\n";
        std::cout << "B points: " << b.points.size() << "\n";
        std::cout << "First B point: " << b.points[0].id << " -> ";
        for (double value : b.points[0].coords) {
            std::cout << value << ' ';
        }
        std::cout << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
