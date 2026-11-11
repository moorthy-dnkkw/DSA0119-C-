/**
 * ============================================================================
 * Task 1: Understand the Problem and Identify the Objects
 * Course: Object-Oriented Programming in C++ (CO1 - Bloom's Level: K5 Evaluate)
 * Title: Namma Metro Smart-Card System - Domain Modeling & OO Analysis
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// ----------------------------------------------------------------------------
// 1. Zone Definition
// ----------------------------------------------------------------------------
enum class Zone {
    Inner,
    Middle,
    Outer
};

std::string zoneToString(Zone z) {
    switch (z) {
        case Zone::Inner:  return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer:  return "Outer";
        default:           return "Unknown";
    }
}

// ----------------------------------------------------------------------------
// 2. FareTable: Encapsulates fare rules, surcharges, and constants
// ----------------------------------------------------------------------------
class FareTable {
public:
    static constexpr long long INNER_BASE_PAISE  = 1000; // Rs 10.00
    static constexpr long long MIDDLE_BASE_PAISE = 2000; // Rs 20.00
    static constexpr long long OUTER_BASE_PAISE  = 3000; // Rs 30.00
    static constexpr int PEAK_SURCHARGE_PERCENT  = 20;   // 20%
    static constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50.00 required post-journey

    static bool isPeakHour(int hour) {
        // Peak hours: 8:00 to 10:00 (i.e., [8, 10)) and 17:00 to 20:00 (i.e., [17, 20))
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    static long long calculateFare(Zone zone, int hour) {
        long long baseFare = 0;
        switch (zone) {
            case Zone::Inner:  baseFare = INNER_BASE_PAISE;  break;
            case Zone::Middle: baseFare = MIDDLE_BASE_PAISE; break;
            case Zone::Outer:  baseFare = OUTER_BASE_PAISE;  break;
        }

        if (isPeakHour(hour)) {
            // Integer arithmetic: + 20%
            baseFare += (baseFare * PEAK_SURCHARGE_PERCENT) / 100;
        }
        return baseFare;
    }
};

// ----------------------------------------------------------------------------
// 3. Passenger Categories (Inheritance / Polymorphism foundation)
// ----------------------------------------------------------------------------
enum class PassengerType {
    General,
    Student,
    SeniorCitizen
};

// ----------------------------------------------------------------------------
// 4. SmartCard: Encapsulates balance and guarantees class invariants
// ----------------------------------------------------------------------------
class SmartCard {
private:
    std::string cardId;
    long long balancePaise; // All currency stored in integer paise
    PassengerType passengerType;

public:
    SmartCard(std::string id, long long initialBalancePaise, PassengerType type = PassengerType::General)
        : cardId(id), balancePaise(initialBalancePaise), passengerType(type) {}

    std::string getId() const { return cardId; }
    long long getBalance() const { return balancePaise; }
    PassengerType getPassengerType() const { return passengerType; }

    bool canTravel(long long fare) const {
        // Invariant: Balance AFTER deducting fare must remain >= MIN_BALANCE_PAISE (Rs 50.00)
        return (balancePaise - fare) >= FareTable::MIN_BALANCE_PAISE;
    }

    bool deductFare(long long fare) {
        if (!canTravel(fare)) {
            return false;
        }
        balancePaise -= fare;
        return true;
    }

    void recharge(long long amountPaise) {
        if (amountPaise > 0) {
            balancePaise += amountPaise;
        }
    }
};

// ----------------------------------------------------------------------------
// 5. Journey: Represents a single transit transaction
// ----------------------------------------------------------------------------
class Journey {
private:
    std::string journeyId;
    Zone destinationZone;
    int hourOfTravel;
    long long fareCharged;
    bool approved;

public:
    Journey(std::string id, Zone zone, int hour)
        : journeyId(id), destinationZone(zone), hourOfTravel(hour), fareCharged(0), approved(false) {}

    bool execute(SmartCard& card) {
        fareCharged = FareTable::calculateFare(destinationZone, hourOfTravel);
        approved = card.deductFare(fareCharged);
        return approved;
    }

    void printSummary(const SmartCard& card) const {
        std::cout << "[Journey " << journeyId << "] Zone: " << std::setw(6) << zoneToString(destinationZone)
                  << " | Hour: " << std::setw(2) << hourOfTravel << ":00"
                  << " | Peak: " << (FareTable::isPeakHour(hourOfTravel) ? "YES" : "NO ")
                  << " | Fare: Rs " << std::fixed << std::setprecision(2) << (fareCharged / 100.0)
                  << " | Status: " << (approved ? "APPROVED" : "DENIED  ")
                  << " | Balance Remaining: Rs " << (card.getBalance() / 100.0) << "\n";
    }
};

// ----------------------------------------------------------------------------
// Demonstration & Comparison
// ----------------------------------------------------------------------------
int main() {
    std::cout << "==============================================================\n";
    std::cout << "       TASK 1: OBJECT-ORIENTED METRO FARE SYSTEM MODEL        \n";
    std::cout << "==============================================================\n\n";

    std::cout << "--- 1. Domain Entities & Invariant Enforcement ---\n";
    SmartCard card("CARD-1001", 10000); // Initial balance: Rs 100.00 (10000 paise)

    std::cout << "Initial Card State: ID=" << card.getId() 
              << ", Balance=Rs " << (card.getBalance() / 100.0) << "\n\n";

    // Test Journey 1: Inner zone, off-peak (14:00) -> Base Rs 10, Balance becomes Rs 90
    Journey j1("J01", Zone::Inner, 14);
    j1.execute(card);
    j1.printSummary(card);

    // Test Journey 2: Outer zone, peak (8:00 AM) -> Base Rs 30 + 20% = Rs 36, Balance becomes Rs 54
    Journey j2("J02", Zone::Outer, 8);
    j2.execute(card);
    j2.printSummary(card);

    // Test Journey 3: Middle zone, off-peak (11:00 AM) -> Base Rs 20, Balance would become Rs 34 (< Rs 50)
    // Rule enforcement: Must be DENIED!
    Journey j3("J03", Zone::Middle, 11);
    j3.execute(card);
    j3.printSummary(card);

    std::cout << "\n--- 2. Procedural vs Object-Oriented Analysis Summary ---\n";
    std::cout << "Procedural Style: Global balances and free functions fail to protect state.\n"
              << "                  Any rogue function could set balance = -100 or bypass rules.\n"
              << "OOP Style:        Private balance with public methods guarantees the\n"
              << "                  Rs 50.00 minimum balance invariant at all times.\n";

    return 0;
}
