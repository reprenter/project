#include "../include/Asset.h"

#include <iomanip>

Asset::Asset(const std::string& ticker, double initialPrice)
    : ticker(ticker), price(initialPrice) {
    history.push_back(initialPrice);
}

Asset::~Asset() = default;

const std::string& Asset::getTicker() const {
    return ticker;
}

double Asset::getPrice() const {
    return price;
}

const std::deque<double>& Asset::getHistory() const {
    return history;
}

std::ostream& operator<<(std::ostream& os, const Asset& asset) {
    os << asset.ticker << " [" << asset.getTypeName() << "] $"
       << std::fixed << std::setprecision(2) << asset.price;
    return os;
}
