#include "../DataLoader.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("readDataset parses a valid 2D dataset") {
    Dataset data = readDataset("PWJC_przyklady/data/basic_A.txt");

    REQUIRE(data.dimension == 2);
    REQUIRE(data.points.size() == 3);
    REQUIRE(data.points[0].id == "A1");
    REQUIRE(data.points[0].coords.size() == 2);
    REQUIRE(data.points[0].coords[0] == 0.0);
    REQUIRE(data.points[0].coords[1] == 0.0);
}
