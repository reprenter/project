#include "../include/FileManager.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

void FileManager::savePortfolio(const Portfolio<Position>& portfolio, const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    file << "CASH " << portfolio.getCash() << '\n';

    for (const auto& entry : portfolio.getHoldings()) {
        file << entry.first << ' '
             << entry.second.amount << ' '
             << entry.second.avgBuyPrice << '\n';
    }

    if (!file.good()) {
        throw std::runtime_error("Error while writing to file: " + filename);
    }
}

void FileManager::loadPortfolio(Portfolio<Position>& portfolio, const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    portfolio.getHoldingsMutable().clear();

    std::string line;
    bool cashLoaded = false;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream stream(line);
        std::string token;
        stream >> token;

        if (token == "CASH") {
            double cash = 0.0;
            if (!(stream >> cash)) {
                throw std::runtime_error("Invalid CASH line in file: " + filename);
            }
            portfolio.setCash(cash);
            cashLoaded = true;
            continue;
        }

        Position position;
        if (!(stream >> position.amount >> position.avgBuyPrice)) {
            throw std::runtime_error("Invalid position line in file: " + filename);
        }
        portfolio.getHoldingsMutable()[token] = position;
    }

    if (!cashLoaded) {
        throw std::runtime_error("CASH record not found in file: " + filename);
    }

    if (!file.eof() && file.fail()) {
        throw std::runtime_error("Error while reading file: " + filename);
    }
}
