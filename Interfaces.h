#ifndef INTERFACES_H
#define INTERFACES_H

#include <string>
#include <chrono>

// ============================================================================
// СТРУКТУРЫ ДАННЫХ (используются в нескольких модулях)
// ============================================================================

/**
 *  Позиция в портфеле (используется в Portfolio<T>)
 */
struct Position {
    double amount;        // Количество единиц актива
    double avgBuyPrice;   // Средняя цена покупки
    
    Position() : amount(0.0), avgBuyPrice(0.0) {}
    Position(double amt, double price) : amount(amt), avgBuyPrice(price) {}
};

/**
 * @brief Тип торговой операции
 */
enum class TradeType {
    BUY,
    SELL
};

/**
 * @brief Запись о совершенной сделке
 */
struct Trade {
    std::string ticker;           // Тикер актива (например, "BTCOIN")
    TradeType type;               // Тип операции (BUY/SELL)
    double amount;                // Количество
    double pricePerUnit;          // Цена за единицу на момент сделки
    std::chrono::system_clock::time_point timestamp; // Время сделки
    
    Trade() : type(TradeType::BUY), amount(0.0), pricePerUnit(0.0) {}
    Trade(const std::string& t, TradeType tp, double amt, double price)
        : ticker(t), type(tp), amount(amt), pricePerUnit(price),
          timestamp(std::chrono::system_clock::now()) {}
};

// ============================================================================
// FORWARD DECLARATIONS (чтобы избежать циклических зависимостей)
// ============================================================================

// Классы модели
class Asset;
class Stock;
class Crypto;
class Bond;

// Контейнеры
template <typename T>
class Portfolio;

// Движок и контроллеры
class Market;
class ConsoleUI;
class FileManager;
class GameManager;

// ============================================================================
// ОБЩИЕ КОНСТАНТЫ
// ============================================================================

namespace Constants {
    constexpr double INITIAL_BALANCE = 10000.0;  // Стартовый баланс
    constexpr size_t MAX_PRICE_HISTORY = 30;     // Макс. длина истории цен
    constexpr const char* SAVE_FILE = "portfolio_save.txt";
}

#endif // INTERFACES_H