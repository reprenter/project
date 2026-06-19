#ifndef CRYPTO_H
#define CRYPTO_H

#include "Asset.h"

#include <algorithm>
#include <random>

class Crypto : public Asset {
public:
    Crypto(const std::string& ticker, double initialPrice);

    void updatePrice() override;
    double getCommission() const override;
    std::string getTypeName() const override;
};

inline Crypto::Crypto(const std::string& ticker, double initialPrice)
    : Asset(ticker, initialPrice) {}

inline void Crypto::updatePrice() {
    static thread_local std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<double> distribution(-0.08, 0.08);

    price = std::max(0.01, price * (1.0 + distribution(generator)));
    history.push_back(price);
    if (history.size() > 100) {
        history.pop_front();
    }
}

inline double Crypto::getCommission() const {
    return 0.0025;
}

inline std::string Crypto::getTypeName() const {
    return "Crypto";
}

#endif
