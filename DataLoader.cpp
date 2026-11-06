#include "DataLoader.h"

#include <cmath>
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>

Dataset readDataset(const std::filesystem::path& filePath) {
    std::ifstream in(filePath);
    if (!in) {
        throw std::runtime_error("Cannot open file: " + filePath.string());
    }

    Dataset dataset;
    std::string token;

    if (!(in >> token) || token != "dimension") {
        throw std::runtime_error("Missing 'dimension' header");
    }
    if (!(in >> dataset.dimension)) {
        throw std::runtime_error("Invalid dimension value");
    }
    if (dataset.dimension < 2) {
        throw std::runtime_error("Dimension must be at least 2");
    }

    if (!(in >> token) || token != "points") {
        throw std::runtime_error("Missing 'points' header");
    }

    int count = 0;
    if (!(in >> count) || count < 0) {
        throw std::runtime_error("Invalid points count");
    }

    dataset.points.reserve(static_cast<size_t>(count));
    std::set<std::string> seenIds;

    for (int i = 0; i < count; ++i) {
        Point p;
        if (!(in >> p.id)) {
            throw std::runtime_error("Missing point id");
        }

        if (!seenIds.insert(p.id).second) {
            throw std::runtime_error("Duplicate point id: " + p.id);
        }

        p.coords.resize(dataset.dimension);
        for (int d = 0; d < dataset.dimension; ++d) {
            if (!(in >> p.coords[d])) {
                throw std::runtime_error("Missing coordinate for point: " + p.id);
            }
            if (!std::isfinite(p.coords[d])) {
                throw std::runtime_error("Non-finite coordinate for point: " + p.id);
            }
        }

        dataset.points.push_back(p);
    }

    std::string extra;
    if (in >> extra) {
        throw std::runtime_error("Unexpected extra data after declared points");
    }

    return dataset;
}
