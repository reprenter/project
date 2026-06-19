#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <map>
#include <stdexcept>
#include <string>

struct Position {
    double amount{0.0};
    double avgBuyPrice{0.0};
};

template <typename T>
class Portfolio {
    std::map<std::string, T> holdings;
    double cash;

public:
    explicit Portfolio(double initialCash = 10000.0) : cash(initialCash) {}

    double getCash() const {
        return cash;
    }

    void setCash(double amount) {
        cash = amount;
    }

    bool buy(const std::string& ticker, double amount, double price, double commission) {
        if (amount <= 0.0) {
            return false;
        }

        const double cost = amount * price * (1.0 + commission);
        if (cost > cash + 1e-9) {
            return false;
        }

        cash -= cost;
        T& position = holdings[ticker];
        const double totalAmount = position.amount + amount;
        position.avgBuyPrice =
            (position.avgBuyPrice * position.amount + price * amount) / totalAmount;
        position.amount = totalAmount;
        return true;
    }

    bool sell(const std::string& ticker, double amount, double price, double commission) {
        if (amount <= 0.0) {
            return false;
        }

        const auto it = holdings.find(ticker);
        if (it == holdings.end() || it->second.amount + 1e-9 < amount) {
            return false;
        }

        const double revenue = amount * price * (1.0 - commission);
        cash += revenue;
        it->second.amount -= amount;

        if (it->second.amount < 1e-9) {
            holdings.erase(it);
        }
        return true;
    }

    const std::map<std::string, T>& getHoldings() const {
        return holdings;
    }

    std::map<std::string, T>& getHoldingsMutable() {
        return holdings;
    }

    template <typename PriceGetter>
    double calculateTotalValue(PriceGetter getPrice) const {
        double total = cash;
        for (const auto& entry : holdings) {
            total += entry.second.amount * getPrice(entry.first);
        }
        return total;
    }
};

#endif
