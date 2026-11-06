#include "../DataModel.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Point stores an identifier and coordinates") {
    Point point;
    point.id = "P1";
    point.coords = {1.0, 2.0, 3.0};

    REQUIRE(point.id == "P1");
    REQUIRE(point.coords.size() == 3);
    REQUIRE(point.coords[0] == 1.0);
    REQUIRE(point.coords[1] == 2.0);
    REQUIRE(point.coords[2] == 3.0);
}

TEST_CASE("Dataset starts empty by default") {
    Dataset dataset;

    REQUIRE(dataset.dimension == 0);
    REQUIRE(dataset.points.empty());
}
