/**
 * ============================================================================
 * Task 11: Recommend and Defend the Final Design
 * Course: Object-Oriented Programming in C++ (CO1 - Bloom's Level: K5 Evaluate)
 * Title: Final Recommended Production Architecture & Evidence-Based Defence
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

// ============================================================================
// RECOMMENDED PRODUCTION ARCHITECTURE
// ============================================================================

// --- 1. Zone Definition: Scoped enum class (type-safe, extensible) ---
enum class Zone : int {
    Inner  = 0,
    Middle = 1,
    Outer  = 2,
    NUM_ZONES  // Sentinel for iteration and bounds checking
};

// --- 2. Centralized Fare Table: constexpr array (Single point of revision) ---
// A fare revision changes ONLY this one array:
constexpr long long FARE_TABLE_PAISE[] = {1000, 2000, 3000}; // Rs 10, Rs 20, Rs 30
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50.00

// --- 3. FareEngine: Stateless fare computation (pure functions) ---
class FareEngine {
public:
    static bool isPeak(int hour) {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    static long long calculateFare(Zone zone, int hour) {
        int zoneIndex = static_cast<int>(zone);
        if (zoneIndex < 0 || zoneIndex >= static_cast<int>(Zone::NUM_ZONES)) {
            return -1; // Invalid zone error code
        }
        long long base = FARE_TABLE_PAISE[zoneIndex];
        if (isPeak(hour)) {
            base += (base * PEAK_SURCHARGE_PERCENT) / 100;
        }
        return base;
    }
};

// --- 4. SmartCard: Encapsulated balance with class invariants ---
class SmartCard {
private:
    std::string cardId;
    long long balancePaise;

public:
    SmartCard(const std::string& id, long long initialBalance)
        : cardId(id), balancePaise(initialBalance >= 0 ? initialBalance : 0) {}

    std::string getId() const { return cardId; }
    long long getBalance() const { return balancePaise; }

    bool canTravel(long long fare) const {
        if (fare <= 0) return false;
        return (balancePaise - fare) >= MIN_BALANCE_PAISE;
    }

    bool deduct(long long fare) {
        if (!canTravel(fare)) return false;
        balancePaise -= fare;
        return true;
    }

    bool recharge(long long amountPaise) {
        if (amountPaise <= 0) return false;
        balancePaise += amountPaise;
        return true;
    }
};

// --- 5. TransactionLogger: Audit-grade journey record output ---
class TransactionLogger {
public:
    static void logJourney(const SmartCard& card, Zone zone, int hour,
                           long long fare, bool approved) {
        static const char* zoneNames[] = {"Inner", "Middle", "Outer"};
        int zi = static_cast<int>(zone);
        std::cout << std::left
                  << std::setw(14) << card.getId()
                  << std::setw(10) << (zi >= 0 && zi < 3 ? zoneNames[zi] : "Unknown")
                  << std::setw(2)  << hour << ":00  "
                  << std::setw(10) << (FareEngine::isPeak(hour) ? "PEAK" : "OFF-PEAK")
                  << "Rs " << std::fixed << std::setprecision(2) << std::setw(8) << (fare / 100.0)
                  << std::setw(12) << (approved ? "APPROVED" : "DENIED")
                  << "Rs " << std::setw(8) << (card.getBalance() / 100.0) << "\n";
    }
};

int main() {
    std::cout << "==============================================================\n";
    std::cout << "    TASK 11: FINAL RECOMMENDED DESIGN & DEFENCE               \n";
    std::cout << "==============================================================\n\n";

    std::cout << "--- 1. Architecture Overview ---\n";
    std::cout << "Component       | Design Choice         | Justification\n";
    std::cout << "----------------|----------------------|---------------------------------------\n";
    std::cout << "Zone            | enum class : int      | Type-safe, no implicit conversion\n";
    std::cout << "Fare Table      | constexpr array       | Single revision point, zero overhead\n";
    std::cout << "Fare Engine     | Static pure functions | Stateless, testable, O(1) per journey\n";
    std::cout << "SmartCard       | Private balance + OOP | Invariant: bal >= Rs50 after deduction\n";
    std::cout << "Logger          | Static utility class  | Clean separation of concerns\n";
    std::cout << "Accumulator     | long long (64-bit)    | Prevents int overflow at 3M+ scale\n";
    std::cout << "Currency Unit   | Integer paise         | Eliminates IEEE 754 rounding drift\n\n";

    std::cout << "--- 2. Fare Revision Process ---\n";
    std::cout << "To revise the Inner Zone fare from Rs 10 to Rs 15:\n"
              << "  STEP 1: Change FARE_TABLE_PAISE[0] from 1000 to 1500.\n"
              << "  STEP 2: Recompile with g++ -std=c++17 -Wall -Wextra.\n"
              << "  STEP 3: Run automated test suite.\n"
              << "  STEP 4: Deploy updated gate executable.\n"
              << "  Total Lines Changed: 1 (single array element).\n\n";

    std::cout << "--- 3. Production Validation Run ---\n";
    SmartCard card("METRO-PROD-001", 15000); // Rs 150.00
    struct TestTrip { Zone z; int h; };
    TestTrip trips[] = {
        {Zone::Inner, 7},   // Off-peak -> Rs 10
        {Zone::Middle, 9},  // Peak -> Rs 24
        {Zone::Outer, 12},  // Off-peak -> Rs 30
        {Zone::Inner, 18},  // Peak -> Rs 12
        {Zone::Outer, 20}   // Off-peak -> Rs 30
    };

    std::cout << std::left
              << std::setw(14) << "Card ID"
              << std::setw(10) << "Zone"
              << std::setw(8)  << "Time"
              << std::setw(10) << "Peak?"
              << std::setw(12) << "Fare"
              << std::setw(12) << "Status"
              << std::setw(12) << "Balance" << "\n";
    std::cout << std::string(78, '-') << "\n";

    long long totalCollected = 0;
    for (const auto& trip : trips) {
        long long fare = FareEngine::calculateFare(trip.z, trip.h);
        bool ok = card.deduct(fare);
        if (ok) totalCollected += fare;
        TransactionLogger::logJourney(card, trip.z, trip.h, fare, ok);
    }

    std::cout << std::string(78, '=') << "\n";
    std::cout << "Total Revenue Collected: Rs " << std::fixed << std::setprecision(2) 
              << totalCollected / 100.0 << " (" << totalCollected << " paise)\n";
    std::cout << "Final Card Balance:      Rs " << card.getBalance() / 100.0 << "\n";

    std::cout << "\n--- 4. Design Limitations ---\n";
    std::cout << "1. Fares are compiled in; requires rebuild to revise (mitigation: config file).\n";
    std::cout << "2. No multi-zone journey pricing (only farthest zone currently).\n";
    std::cout << "3. No persistence layer (card balance resets on restart).\n";
    std::cout << "4. Single-threaded; concurrent gate access needs mutex protection.\n";

    std::cout << "\n--- 5. Alternative: Config File vs Compiled constexpr ---\n";
    std::cout << "Compiled constexpr: Zero startup time, verified at compile time.\n"
              << "Config file:        O(Z) startup time to read Z fares, but no recompilation needed.\n"
              << "Recommendation:     For < 10 zones, compiled constexpr is preferred.\n"
              << "                    For a dynamic multi-city network, a config file is essential.\n";

    return 0;
}
