#ifndef BOND_H
#define BOND_H

#include "Asset.h"

#include <algorithm>
#include <random>

class Bond : public Asset {
public:
    Bond(const std::string& ticker, double initialPrice);

    void updatePrice() override;
    double getCommission() const override;
    std::string getTypeName() const override;
};

inline Bond::Bond(const std::string& ticker, double initialPrice)
    : Asset(ticker, initialPrice) {}

inline void Bond::updatePrice() {
    static thread_local std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<double> distribution(-0.005, 0.005);

    price = std::max(0.01, price * (1.0 + distribution(generator)));
    history.push_back(price);
    if (history.size() > 100) {
        history.pop_front();
    }
}

inline double Bond::getCommission() const {
    return 0.0005;
}

inline std::string Bond::getTypeName() const {
    return "Bond";
}

#endif
