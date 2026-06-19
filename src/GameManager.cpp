#include "../include/GameManager.h"

#include "../include/Utils.h"

#include <map>
#include <vector>

GameManager::GameManager() : portfolio(10000.0) {
    market.initializeDefaultMarket();
}

void GameManager::run() {
    ui.clearScreen();
    ui.drawAsciiLogo();
    ui.showMessage("Welcome to Crypto/Stock Terminal!");
    ui.pause();

    while (running) {
        ui.clearScreen();
        ui.drawMainMenu();

        try {
            const int choice = ui.readIntChoice(0, 8);
            processMenuChoice(choice);
        } catch (const std::exception& ex) {
            ui.showError(ex.what());
            ui.pause();
        }
    }

    ui.clearScreen();
    ui.showMessage("Session ended. Goodbye!");
}

void GameManager::processMenuChoice(int choice) {
    switch (choice) {
        case 1:
            handleViewMarket();
            break;
        case 2:
            handleBuy();
            break;
        case 3:
            handleSell();
            break;
        case 4:
            handleViewPortfolio();
            break;
        case 5:
            handleAdvanceTick();
            break;
        case 6:
            handleViewHistory();
            break;
        case 7:
            handleSave();
            break;
        case 8:
            handleLoad();
            break;
        case 0:
            running = false;
            break;
        default:
            ui.showError("Unknown command.");
            ui.pause();
            break;
    }
}

void GameManager::handleViewMarket() {
    market.sortByPrice();

    std::vector<const Asset*> assetViews;
    assetViews.reserve(market.getAssets().size());
    for (const auto& asset : market.getAssets()) {
        assetViews.push_back(asset.get());
    }

    ui.clearScreen();
    ui.drawMarketTable(assetViews, market.getTickCount());
    ui.pause();
}

void GameManager::handleBuy() {
    ui.clearScreen();
    ui.drawHeader("BUY ASSET");

    const std::string ticker = ui.readString("Ticker: ");
    const Asset* asset = market.findAsset(ticker);
    if (asset == nullptr) {
        ui.showError("Asset not found: " + ticker);
        ui.pause();
        return;
    }

    const double amount = ui.readDouble("Amount: ");
    const double price = asset->getPrice();
    const double commission = asset->getCommission();

    if (portfolio.buy(ticker, amount, price, commission)) {
        const double cost = amount * price * (1.0 + commission);
        ui.showMessage("Bought " + std::to_string(amount) + " " + ticker +
                       " for $" + std::to_string(cost));
    } else {
        ui.showError("Purchase failed. Check balance or amount.");
    }
    ui.pause();
}

void GameManager::handleSell() {
    ui.clearScreen();
    ui.drawHeader("SELL ASSET");

    const std::string ticker = ui.readString("Ticker: ");
    const Asset* asset = market.findAsset(ticker);
    if (asset == nullptr) {
        ui.showError("Asset not found: " + ticker);
        ui.pause();
        return;
    }

    const double amount = ui.readDouble("Amount: ");
    const double price = asset->getPrice();
    const double commission = asset->getCommission();

    const auto holdingIt = portfolio.getHoldings().find(ticker);
    const double avgBuyPrice =
        holdingIt != portfolio.getHoldings().end() ? holdingIt->second.avgBuyPrice : price;

    if (portfolio.sell(ticker, amount, price, commission)) {
        const double revenue = amount * price * (1.0 - commission);
        const double profit = calculateProfit(avgBuyPrice, price, amount);
        ui.showMessage("Sold " + std::to_string(amount) + " " + ticker +
                       " for $" + std::to_string(revenue) +
                       " (est. P/L: $" + std::to_string(profit) + ")");
    } else {
        ui.showError("Sale failed. Check holdings or amount.");
    }
    ui.pause();
}

void GameManager::handleViewPortfolio() {
    ui.clearScreen();

    std::map<std::string, double> currentPrices;
    for (const auto& entry : portfolio.getHoldings()) {
        currentPrices[entry.first] = getAssetPrice(entry.first);
    }

    const double totalValue = calculatePortfolioValue();
    ui.drawPortfolioTable(portfolio.getHoldings(), portfolio.getCash(), totalValue, currentPrices);
    ui.pause();
}

void GameManager::handleAdvanceTick() {
    market.tick();
    ui.showMessage("Market advanced to tick " + std::to_string(market.getTickCount()));
    ui.pause();
}

void GameManager::handleViewHistory() {
    ui.clearScreen();

    std::vector<const Asset*> assets;
    assets.reserve(market.getAssets().size());
    for (const auto& asset : market.getAssets()) {
        assets.push_back(asset.get());
    }

    ui.drawAllPriceHistories(assets);
    ui.pause();
}

void GameManager::handleSave() {
    ui.clearScreen();
    ui.drawHeader("SAVE PORTFOLIO");

    const std::string filename = ui.readString("Filename: ");
    try {
        fileManager.savePortfolio(portfolio, filename);
        ui.showMessage("Portfolio saved to " + filename);
    } catch (const std::exception& ex) {
        ui.showError(ex.what());
    }
    ui.pause();
}

void GameManager::handleLoad() {
    ui.clearScreen();
    ui.drawHeader("LOAD PORTFOLIO");

    const std::string filename = ui.readString("Filename: ");
    try {
        fileManager.loadPortfolio(portfolio, filename);
        ui.showMessage("Portfolio loaded from " + filename);
    } catch (const std::exception& ex) {
        ui.showError(ex.what());
    }
    ui.pause();
}

double GameManager::getAssetPrice(const std::string& ticker) const {
    const Asset* asset = market.findAsset(ticker);
    return asset != nullptr ? asset->getPrice() : 0.0;
}

double GameManager::calculatePortfolioValue() const {
    return portfolio.calculateTotalValue(
        [this](const std::string& ticker) { return getAssetPrice(ticker); });
}
