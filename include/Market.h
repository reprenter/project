#ifndef MARKET_H
#define MARKET_H

#include "Asset.h"

#include <memory>
#include <string>
#include <vector>

class Market {
    std::vector<std::unique_ptr<Asset>> assets;

public:
    void addAsset(std::unique_ptr<Asset> asset);
    void tick();
    Asset* findAsset(const std::string& ticker);
    const Asset* findAsset(const std::string& ticker) const;
    void sortByPrice();
    const std::vector<std::unique_ptr<Asset>>& getAssets() const;
    void initializeDefaultMarket();
    int getTickCount() const;
    void resetTickCount();

private:
    int tickCount{0};
};

#endif
