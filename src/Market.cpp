#include "../include/Market.h"

#include "../include/Bond.h"
#include "../include/Crypto.h"
#include "../include/Stock.h"

#include <algorithm>

void Market::addAsset(std::unique_ptr<Asset> asset) {
    assets.push_back(std::move(asset));
}

void Market::tick() {
    for (const auto& asset : assets) {
        asset->updatePrice();
    }
    ++tickCount;
}

Asset* Market::findAsset(const std::string& ticker) {
    const auto it = std::find_if(assets.begin(), assets.end(),
        [&ticker](const std::unique_ptr<Asset>& asset) {
            return asset->getTicker() == ticker;
        });
    return it != assets.end() ? it->get() : nullptr;
}

const Asset* Market::findAsset(const std::string& ticker) const {
    const auto it = std::find_if(assets.cbegin(), assets.cend(),
        [&ticker](const std::unique_ptr<Asset>& asset) {
            return asset->getTicker() == ticker;
        });
    return it != assets.cend() ? it->get() : nullptr;
}

void Market::sortByPrice() {
    std::sort(assets.begin(), assets.end(),
        [](const std::unique_ptr<Asset>& lhs, const std::unique_ptr<Asset>& rhs) {
            return lhs->getPrice() > rhs->getPrice();
        });
}

const std::vector<std::unique_ptr<Asset>>& Market::getAssets() const {
    return assets;
}

void Market::initializeDefaultMarket() {
    assets.clear();
    tickCount = 0;

    addAsset(std::make_unique<Stock>("AAPL", 178.50));
    addAsset(std::make_unique<Stock>("GOOGL", 141.20));
    addAsset(std::make_unique<Stock>("MSFT", 378.90));
    addAsset(std::make_unique<Crypto>("BTC", 64250.00));
    addAsset(std::make_unique<Crypto>("ETH", 3450.00));
    addAsset(std::make_unique<Crypto>("SOL", 145.75));
    addAsset(std::make_unique<Bond>("US10Y", 98.40));
    addAsset(std::make_unique<Bond>("CORP-AA", 102.15));
}

int Market::getTickCount() const {
    return tickCount;
}

void Market::resetTickCount() {
    tickCount = 0;
}
