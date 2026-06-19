#ifndef STOCK_H
#define STOCK_H

#include "Asset.h"

#include <algorithm>
#include <random>

class Stock : public Asset {
public:
    Stock(const std::string& ticker, double initialPrice);

    void updatePrice() override;
    double getCommission() const override;
    std::string getTypeName() const override;
};

inline Stock::Stock(const std::string& ticker, double initialPrice)
    : Asset(ticker, initialPrice) {}

inline void Stock::updatePrice() {
    static thread_local std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<double> distribution(-0.02, 0.02);

    price = std::max(0.01, price * (1.0 + distribution(generator)));
    history.push_back(price);
    if (history.size() > 100) {
        history.pop_front();
    }
}

inline double Stock::getCommission() const {
    return 0.001;
}

inline std::string Stock::getTypeName() const {
    return "Stock";
}

#endif
