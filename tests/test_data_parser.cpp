#include "../DataLoader.h"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>

namespace {
const std::filesystem::path kProjectRoot = PROJECT_SOURCE_DIR;

void requireInvalidDataset(const std::filesystem::path& path) {
    REQUIRE_THROWS(readDataset(path));
}
} // namespace

TEST_CASE("readDataset parses a valid 2D dataset") {
    const auto path = kProjectRoot / "PWJC_przyklady/data/basic_A.txt";
    Dataset data = readDataset(path);

    REQUIRE(data.dimension == 2);
    REQUIRE(data.points.size() == 3);
    REQUIRE(data.points[0].id == "A1");
    REQUIRE(data.points[0].coords.size() == 2);
    REQUIRE(data.points[0].coords[0] == 0.0);
    REQUIRE(data.points[0].coords[1] == 0.0);
}

TEST_CASE("readDataset accepts files with headers but zero points") {
    const auto path = std::filesystem::temp_directory_path() / "pwjc_empty_dataset.txt";

    {
        std::ofstream out(path);
        out << "dimension 4\n";
        out << "points 0\n";
    }

    Dataset data = readDataset(path);

    REQUIRE(data.dimension == 4);
    REQUIRE(data.points.empty());

    std::filesystem::remove(path);
}

TEST_CASE("readDataset rejects files with no headers") {
    const auto path = std::filesystem::temp_directory_path() / "pwjc_missing_headers.txt";

    {
        std::ofstream out(path);
        out << "A1 0 0\n";
        out << "A2 1 1\n";
    }

    REQUIRE_THROWS(readDataset(path));

    std::filesystem::remove(path);
}

TEST_CASE("readDataset accepts a B file whose dimension differs from A") {
    const auto path = kProjectRoot / "PWJC_przyklady/invalid/dimension_mismatch_B.txt";
    REQUIRE_NOTHROW(readDataset(path));
}

TEST_CASE("readDataset rejects dimension one A file") {
    requireInvalidDataset(kProjectRoot / "PWJC_przyklady/invalid/dimension_one_A.txt");
}

TEST_CASE("readDataset rejects duplicate id A file") {
    requireInvalidDataset(kProjectRoot / "PWJC_przyklady/invalid/duplicate_id_A.txt");
}

TEST_CASE("readDataset rejects empty file A file") {
    requireInvalidDataset(kProjectRoot / "PWJC_przyklady/invalid/empty_file_A.txt");
}

TEST_CASE("readDataset rejects missing coordinate B file") {
    requireInvalidDataset(kProjectRoot / "PWJC_przyklady/invalid/missing_coordinate_B.txt");
}

TEST_CASE("readDataset rejects nonfinite A file") {
    requireInvalidDataset(kProjectRoot / "PWJC_przyklady/invalid/nonfinite_A.txt");
}

TEST_CASE("readDataset rejects wrong count A file") {
    requireInvalidDataset(kProjectRoot / "PWJC_przyklady/invalid/wrong_count_A.txt");
}
