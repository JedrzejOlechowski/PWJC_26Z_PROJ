#include "DataLoader.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {

// Zgłasza błąd w formacie "plik:wiersz: opis"; wiersz 0 oznacza błąd bez konkretnego wiersza.
[[noreturn]] void fail(const std::filesystem::path& filePath, int lineNumber, const std::string& message) {
    std::string location = filePath.string();
    if (lineNumber > 0) {
        location += ":" + std::to_string(lineNumber);
    }
    throw std::runtime_error(location + ": " + message);
}

// Czyta kolejne niepuste wiersze pliku jako listy pól i pamięta numer bieżącego wiersza.
class LineReader {
public:
    LineReader(std::istream& in, const std::filesystem::path& filePath)
        : in_(in), filePath_(filePath) {}

    // Zwraca false na końcu pliku. Puste wiersze są pomijane.
    bool next(std::vector<std::string>& fields) {
        std::string line;
        while (std::getline(in_, line)) {
            ++lineNumber_;
            // Pomijamy znacznik BOM, który część edytorów dopisuje do plików UTF-8.
            if (lineNumber_ == 1 && line.starts_with("\xEF\xBB\xBF")) {
                line.erase(0, 3);
            }
            fields = splitFields(line);
            if (!fields.empty()) {
                return true;
            }
        }
        if (in_.bad()) {
            fail(filePath_, 0, "read error");
        }
        return false;
    }

    [[noreturn]] void failHere(const std::string& message) const {
        fail(filePath_, lineNumber_, message);
    }

    [[noreturn]] void failAtEnd(const std::string& message) const {
        fail(filePath_, 0, message);
    }

private:
    // Pola są rozdzielone spacjami lub tabulatorami; końcowe '\r' z plików CRLF też jest separatorem.
    static std::vector<std::string> splitFields(const std::string& line) {
        std::vector<std::string> fields;
        std::istringstream stream(line);
        std::string field;
        while (stream >> field) {
            fields.push_back(field);
        }
        return fields;
    }

    std::istream& in_;
    const std::filesystem::path& filePath_;
    int lineNumber_ = 0;
};

// Liczba musi zajmować całe pole: "2x" albo "2.5" nie są poprawną liczbą całkowitą.
bool parseInt(const std::string& text, int& value) {
    const char* end = text.data() + text.size();
    const auto [ptr, ec] = std::from_chars(text.data(), end, value);
    return ec == std::errc() && ptr == end;
}

// Akceptuje zapis dziesiętny i wykładniczy z kropką; "nan" i "inf" odrzuca wywołujący.
bool parseDouble(const std::string& text, double& value) {
    const char* begin = text.data();
    const char* end = begin + text.size();
    // from_chars nie przyjmuje wiodącego '+', choć "+1.5" jest poprawną liczbą.
    if (text.size() > 1 && text[0] == '+' && text[1] != '-') {
        ++begin;
    }
    const auto [ptr, ec] = std::from_chars(begin, end, value);
    return ec == std::errc() && ptr == end;
}

// Identyfikator: niepusty ciąg liter ASCII, cyfr, '_' lub '-'.
bool isValidId(const std::string& id) {
    return !id.empty() && std::all_of(id.begin(), id.end(), [](unsigned char c) {
        return std::isalnum(c) || c == '_' || c == '-';
    });
}

// Czyta nagłówek postaci "<keyword> <liczba całkowita>".
int readHeader(LineReader& reader, const std::string& keyword) {
    std::vector<std::string> fields;
    if (!reader.next(fields)) {
        reader.failAtEnd("missing '" + keyword + "' header");
    }
    int value = 0;
    if (fields.size() != 2 || fields[0] != keyword || !parseInt(fields[1], value)) {
        reader.failHere("expected '" + keyword + " <integer>'");
    }
    return value;
}

} // namespace

Dataset readDataset(const std::filesystem::path& filePath) {
    std::ifstream in(filePath);
    if (!in) {
        fail(filePath, 0, "cannot open file");
    }
    LineReader reader(in, filePath);

    Dataset dataset;
    dataset.dimension = readHeader(reader, "dimension");
    if (dataset.dimension < 2) {
        reader.failHere("dimension must be at least 2, got " + std::to_string(dataset.dimension));
    }

    const int count = readHeader(reader, "points");
    if (count < 0) {
        reader.failHere("points must not be negative, got " + std::to_string(count));
    }

    // Ograniczamy wstępną rezerwację, aby błędny nagłówek nie wymusił ogromnej alokacji.
    dataset.points.reserve(static_cast<std::size_t>(std::min(count, 100000)));
    std::unordered_set<std::string> seenIds;
    const std::size_t expectedFields = static_cast<std::size_t>(dataset.dimension) + 1;
    std::vector<std::string> fields;

    for (int i = 0; i < count; ++i) {
        if (!reader.next(fields)) {
            reader.failAtEnd("expected " + std::to_string(count) + " points, found " + std::to_string(i));
        }
        if (fields.size() != expectedFields) {
            reader.failHere("expected identifier and " + std::to_string(dataset.dimension)
                            + " coordinates, got " + std::to_string(fields.size()) + " fields");
        }

        Point point;
        point.id = fields[0];
        if (!isValidId(point.id)) {
            reader.failHere("invalid identifier '" + point.id + "'");
        }
        if (!seenIds.insert(point.id).second) {
            reader.failHere("duplicate identifier '" + point.id + "'");
        }

        point.coords.reserve(static_cast<std::size_t>(dataset.dimension));
        for (std::size_t f = 1; f < fields.size(); ++f) {
            double value = 0.0;
            if (!parseDouble(fields[f], value)) {
                reader.failHere("invalid coordinate '" + fields[f] + "' for point '" + point.id + "'");
            }
            if (!std::isfinite(value)) {
                reader.failHere("non-finite coordinate '" + fields[f] + "' for point '" + point.id + "'");
            }
            point.coords.push_back(value);
        }

        dataset.points.push_back(std::move(point));
    }

    if (reader.next(fields)) {
        reader.failHere("unexpected data after " + std::to_string(count) + " declared points");
    }

    return dataset;
}
