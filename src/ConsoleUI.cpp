#include "../include/ConsoleUI.h"

#include "../include/Utils.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <deque>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {

constexpr int kChartHeight = 8;
constexpr int kChartWidth = 48;

std::vector<double> sampleHistory(const std::deque<double>& history, std::size_t targetCount) {
    std::vector<double> result;
    if (history.empty()) {
        return result;
    }
    if (history.size() <= targetCount) {
        result.assign(history.begin(), history.end());
        return result;
    }

    result.reserve(targetCount);
    for (std::size_t i = 0; i < targetCount; ++i) {
        const std::size_t index = i * (history.size() - 1) / (targetCount - 1);
        result.push_back(history[index]);
    }
    return result;
}

int priceToRow(double price, double minPrice, double range) {
    const double normalized = (price - minPrice) / range;
    const int row = static_cast<int>(std::lround(normalized * (kChartHeight - 1)));
    return std::max(0, std::min(row, kChartHeight - 1));
}

void drawChartLine(std::vector<std::string>& grid, int row0, int col0, int row1, int col1) {
    const int deltaRow = row1 - row0;
    const int deltaCol = col1 - col0;
    const int steps = std::max(std::abs(deltaRow), std::abs(deltaCol));

    for (int step = 0; step <= steps; ++step) {
        const int row = row0 + (deltaRow * step) / steps;
        const int col = col0 + (deltaCol * step) / steps;
        if (row >= 0 && row < kChartHeight && col >= 0 && col < kChartWidth) {
            grid[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)] = '*';
        }
    }
}

void drawAsciiPriceChart(const Asset& asset) {
    const auto& history = asset.getHistory();
    if (history.empty()) {
        std::cout << "  (no price data)\n";
        return;
    }

    const std::vector<double> samples = sampleHistory(history, static_cast<std::size_t>(kChartWidth));
    const int chartWidth = static_cast<int>(samples.size());
    if (chartWidth == 0) {
        return;
    }

    const auto [minIt, maxIt] = std::minmax_element(samples.begin(), samples.end());
    double minPrice = *minIt;
    double maxPrice = *maxIt;
    double range = maxPrice - minPrice;
    if (range < 1e-9) {
        range = std::max(minPrice * 0.01, 1.0);
        minPrice -= range * 0.5;
        maxPrice += range * 0.5;
        range = maxPrice - minPrice;
    }

    std::vector<std::string> grid(
        static_cast<std::size_t>(kChartHeight),
        std::string(static_cast<std::size_t>(chartWidth), ' '));

    int prevRow = priceToRow(samples.front(), minPrice, range);
    for (std::size_t col = 0; col < samples.size(); ++col) {
        const int row = priceToRow(samples[col], minPrice, range);
        if (col == 0) {
            grid[static_cast<std::size_t>(row)][0] = '*';
        } else {
            drawChartLine(grid, prevRow, static_cast<int>(col) - 1, row, static_cast<int>(col));
        }
        prevRow = row;
    }

    for (int row = kChartHeight - 1; row >= 0; --row) {
        const double label = minPrice + (range * row) / (kChartHeight - 1);
        std::cout << "  " << std::right << std::setw(10) << std::fixed << std::setprecision(2) << label
                  << " |" << grid[static_cast<std::size_t>(row)] << "|\n";
    }

    std::cout << "  " << std::string(10, ' ') << " +"
              << std::string(static_cast<std::size_t>(chartWidth), '-')
              << "+\n";

    std::cout << "  " << std::string(10, ' ') << " 0";
    if (history.size() > 1) {
        const std::string endLabel = std::to_string(history.size() - 1) + " ticks";
        const int gap = chartWidth - 1 - static_cast<int>(endLabel.size());
        if (gap > 0) {
            std::cout << std::string(static_cast<std::size_t>(gap), ' ') << endLabel;
        } else {
            std::cout << ' ' << endLabel;
        }
    }
    std::cout << '\n';
}

}  // namespace

void ConsoleUI::clearScreen() const {
    std::cout << "\033[2J\033[H";
}

void ConsoleUI::drawHeader(const std::string& title) const {
    const int width = static_cast<int>(title.size()) + 4;
    std::cout << "╔";
    for (int i = 0; i < width; ++i) {
        std::cout << "═";
    }
    std::cout << "╗\n";
    std::cout << "║ " << std::left << std::setw(width - 1) << title << "║\n";
    std::cout << "╚";
    for (int i = 0; i < width; ++i) {
        std::cout << "═";
    }
    std::cout << "╝\n";
}

void ConsoleUI::drawSeparator(int width) const {
    const std::string line(static_cast<std::size_t>(width), '-');
    std::cout << line << '\n';
}

void ConsoleUI::drawAsciiLogo() const {
    std::cout << R"(
   ██████╗██████╗ ██╗   ██╗██████╗ ████████╗ ██████╗ 
  ██╔════╝██╔══██╗╚██╗ ██╔╝██╔══██╗╚══██╔══╝██╔═══██╗
  ██║     ██████╔╝ ╚████╔╝ ██████╔╝   ██║   ██║   ██║
  ██║     ██╔══██╗  ╚██╔╝  ██╔═══╝    ██║   ██║   ██║
  ╚██████╗██║  ██║   ██║   ██║        ██║   ╚██████╔╝
   ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚═╝        ╚═╝    ╚═════╝ 
         STOCK / CRYPTO TRADING TERMINAL v1.0
)" << '\n';
}

void ConsoleUI::drawMainMenu() const {
    drawHeader("MAIN MENU");
    std::cout << "  [1] View Market\n";
    std::cout << "  [2] Buy Asset\n";
    std::cout << "  [3] Sell Asset\n";
    std::cout << "  [4] View Portfolio\n";
    std::cout << "  [5] Advance Market Tick\n";
    std::cout << "  [6] View Price History\n";
    std::cout << "  [7] Save Portfolio\n";
    std::cout << "  [8] Load Portfolio\n";
    std::cout << "  [0] Exit\n";
    drawSeparator();
}

void ConsoleUI::drawMarketTable(const std::vector<const Asset*>& assets, int tickCount) const {
    drawHeader("MARKET OVERVIEW  |  Tick: " + std::to_string(tickCount));

    std::cout << std::left
              << std::setw(8) << "Ticker"
              << std::setw(10) << "Type"
              << std::right
              << std::setw(14) << "Price"
              << std::setw(12) << "Commission"
              << std::setw(14) << "Change"
              << '\n';
    drawSeparator(58);

    for (const Asset* asset : assets) {
        const auto& history = asset->getHistory();
        double change = 0.0;
        if (history.size() >= 2) {
            const double prev = history[history.size() - 2];
            change = ((asset->getPrice() - prev) / prev) * 100.0;
        }

        std::ostringstream changeStream;
        changeStream << std::fixed << std::setprecision(2) << change << '%';

        std::cout << std::left
                  << std::setw(8) << asset->getTicker()
                  << std::setw(10) << asset->getTypeName()
                  << std::right
                  << std::setw(14) << std::fixed << std::setprecision(2) << asset->getPrice()
                  << std::setw(12) << std::setprecision(4) << (asset->getCommission() * 100.0) << '%'
                  << std::setw(14) << changeStream.str()
                  << '\n';
    }
    drawSeparator(58);
}

void ConsoleUI::drawPortfolioTable(
    const std::map<std::string, Position>& holdings,
    double cash,
    double totalValue,
    const std::map<std::string, double>& currentPrices) const {
    drawHeader("PORTFOLIO");

    std::cout << std::left
              << std::setw(8) << "Ticker"
              << std::right
              << std::setw(12) << "Amount"
              << std::setw(14) << "Avg Buy"
              << std::setw(14) << "Value"
              << std::setw(14) << "P/L"
              << '\n';
    drawSeparator(62);

    if (holdings.empty()) {
        std::cout << "  (no open positions)\n";
    } else {
        for (const auto& entry : holdings) {
            const Position& position = entry.second;
            double marketPrice = position.avgBuyPrice;
            const auto priceIt = currentPrices.find(entry.first);
            if (priceIt != currentPrices.end()) {
                marketPrice = priceIt->second;
            }

            const double marketValue = position.amount * marketPrice;
            const double profit = calculateProfit(position.avgBuyPrice, marketPrice, position.amount);

            std::cout << std::left
                      << std::setw(8) << entry.first
                      << std::right
                      << std::setw(12) << std::fixed << std::setprecision(4) << position.amount
                      << std::setw(14) << std::setprecision(2) << position.avgBuyPrice
                      << std::setw(14) << std::setprecision(2) << marketValue
                      << std::setw(14) << std::setprecision(2) << profit
                      << '\n';
        }
    }

    drawSeparator(62);
    std::cout << std::left << std::setw(20) << "Cash:"
              << std::right << std::fixed << std::setprecision(2) << cash << '\n';
    std::cout << std::left << std::setw(20) << "Total Value:"
              << std::right << std::fixed << std::setprecision(2) << totalValue << '\n';
    drawSeparator(62);
}

void ConsoleUI::drawAllPriceHistories(const std::vector<const Asset*>& assets) const {
    drawHeader("PRICE HISTORY — ALL ASSETS");

    for (const Asset* asset : assets) {
        const auto& history = asset->getHistory();

        std::cout << '\n';
        drawSeparator(62);
        std::cout << "  " << asset->getTicker() << " [" << asset->getTypeName() << "]"
                  << "  Current: $" << std::fixed << std::setprecision(2) << asset->getPrice();

        if (history.size() >= 2) {
            const double first = history.front();
            const double last = history.back();
            const double delta = ((last - first) / first) * 100.0;
            std::cout << "  Change: " << std::showpos << std::setprecision(2) << delta << "%"
                      << std::noshowpos;
        }
        std::cout << '\n';

        std::cout << "  Prices: ";
        printContainer(history, std::cout);
        std::cout << '\n';

        drawAsciiPriceChart(*asset);
    }

    std::cout << '\n';
    drawSeparator(62);
}

int ConsoleUI::readIntChoice(int min, int max) const {
    while (true) {
        try {
            std::cout << "Enter choice [" << min << "-" << max << "]: ";
            std::string line;
            if (!std::getline(std::cin, line)) {
                throw std::runtime_error("Input stream closed");
            }

            const int value = std::stoi(line);
            if (value < min || value > max) {
                showError("Value out of range. Try again.");
                continue;
            }
            return value;
        } catch (const std::invalid_argument&) {
            showError("Invalid number. Try again.");
        } catch (const std::out_of_range&) {
            showError("Number too large. Try again.");
        } catch (const std::exception& ex) {
            showError(std::string("Input error: ") + ex.what());
            throw;
        }
    }
}

double ConsoleUI::readDouble(const std::string& prompt) const {
    while (true) {
        try {
            std::cout << prompt;
            std::string line;
            if (!std::getline(std::cin, line)) {
                throw std::runtime_error("Input stream closed");
            }

            const double value = std::stod(line);
            if (value < 0.0) {
                showError("Value must be non-negative.");
                continue;
            }
            return value;
        } catch (const std::invalid_argument&) {
            showError("Invalid number. Try again.");
        } catch (const std::out_of_range&) {
            showError("Number too large. Try again.");
        }
    }
}

std::string ConsoleUI::readString(const std::string& prompt) const {
    std::cout << prompt;
    std::string value;
    if (!std::getline(std::cin, value)) {
        throw std::runtime_error("Input stream closed");
    }
    return value;
}

void ConsoleUI::showMessage(const std::string& message) const {
    std::cout << "[OK] " << message << '\n';
}

void ConsoleUI::showError(const std::string& message) const {
    std::cout << "[ERROR] " << message << '\n';
}

void ConsoleUI::pause() const {
    std::cout << "\nPress Enter to continue...";
    std::string line;
    std::getline(std::cin, line);
}
