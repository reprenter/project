#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <numeric>
#include <ostream>

template <typename T>
double calculateProfit(T buyPrice, T currentPrice, T amount) {
    return static_cast<double>(currentPrice - buyPrice) * static_cast<double>(amount);
}

template <typename Container>
void printContainer(const Container& container, std::ostream& os) {
    os << "[ ";
    for (const auto& item : container) {
        os << item << ' ';
    }
    os << ']';
}

template <typename Container, typename Predicate>
typename Container::value_type findInContainer(const Container& container, Predicate predicate) {
    const auto it = std::find_if(container.begin(), container.end(), predicate);
    if (it == container.end()) {
        throw std::runtime_error("Element not found in container");
    }
    return *it;
}

template <typename Container>
double sumContainerValues(const Container& container) {
    return std::accumulate(container.begin(), container.end(), 0.0);
}

#endif
