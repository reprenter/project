#ifndef ASSET_H
#define ASSET_H

#include <deque>
#include <ostream>
#include <string>

class Asset {
protected:
    std::string ticker;
    double price;
    std::deque<double> history;

public:
    Asset(const std::string& ticker, double initialPrice);
    virtual ~Asset();

    const std::string& getTicker() const;
    double getPrice() const;
    const std::deque<double>& getHistory() const;

    virtual void updatePrice() = 0;
    virtual double getCommission() const = 0;
    virtual std::string getTypeName() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Asset& asset);
};

#endif
