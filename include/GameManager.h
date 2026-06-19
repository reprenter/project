#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "ConsoleUI.h"
#include "FileManager.h"
#include "Market.h"
#include "Portfolio.h"

class GameManager {
    Market market;
    Portfolio<Position> portfolio;
    ConsoleUI ui;
    FileManager fileManager;
    bool running{true};

public:
    GameManager();
    void run();

private:
    void processMenuChoice(int choice);
    void handleViewMarket();
    void handleBuy();
    void handleSell();
    void handleViewPortfolio();
    void handleAdvanceTick();
    void handleViewHistory();
    void handleSave();
    void handleLoad();

    double getAssetPrice(const std::string& ticker) const;
    double calculatePortfolioValue() const;
};

#endif
