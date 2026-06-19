#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "Asset.h"
#include "Portfolio.h"

#include <map>
#include <string>
#include <vector>

class ConsoleUI {
public:
    void clearScreen() const;
    void drawHeader(const std::string& title) const;
    void drawSeparator(int width = 60) const;
    void drawMainMenu() const;
    void drawMarketTable(const std::vector<const Asset*>& assets, int tickCount) const;
    void drawPortfolioTable(
        const std::map<std::string, Position>& holdings,
        double cash,
        double totalValue,
        const std::map<std::string, double>& currentPrices = {}) const;
    void drawAllPriceHistories(const std::vector<const Asset*>& assets) const;
    void drawAsciiLogo() const;

    int readIntChoice(int min, int max) const;
    double readDouble(const std::string& prompt) const;
    std::string readString(const std::string& prompt) const;
    void showMessage(const std::string& message) const;
    void showError(const std::string& message) const;
    void pause() const;
};

#endif
