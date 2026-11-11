/**
 * ============================================================================
 * Task 6: Design the Control Structures
 * Course: Object-Oriented Programming in C++ (CO1 - Bloom's Level: K5 Evaluate)
 * Title: Metro Fare Control Logic, Switch/Loops & SIMATS Standard Pseudocode
 * ============================================================================
 *
 * SIMATS Engineering Standard Algorithmic Pseudocode:
 * ----------------------------------------------------
 * ALGORITHM CalculateFare(zone, isPeak)
 * INPUT:  zone of type Zone, isPeak of type Boolean
 * OUTPUT: finalFare in integer paise
 *
 * BEGIN
 *   SWITCH zone DO
 *     CASE Zone.Inner:
 *       baseFare <- BASE_PAISE[0]
 *     CASE Zone.Middle:
 *       baseFare <- BASE_PAISE[1]
 *     CASE Zone.Outer:
 *       baseFare <- BASE_PAISE[2]
 *     DEFAULT:
 *       baseFare <- 0
 *   END SWITCH
 *
 *   IF isPeak = TRUE THEN
 *     surcharge <- (baseFare * PEAK_SURCHARGE_PERCENT) / 100
 *     finalFare <- baseFare + surcharge
 *   ELSE
 *     finalFare <- baseFare
 *   END IF
 *
 *   RETURN finalFare
 * END
 *
 * ALGORITHM CanTravel(currentBalance, tripFare)
 * INPUT:  currentBalance in paise, tripFare in paise
 * OUTPUT: Boolean (TRUE if travel permitted, FALSE otherwise)
 *
 * BEGIN
 *   remainingBalance <- currentBalance - tripFare
 *   IF remainingBalance >= MIN_BALANCE_PAISE THEN
 *     RETURN TRUE
 *   ELSE
 *     RETURN FALSE
 *   END IF
 * END
 * ============================================================================
 */

#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };

constexpr int BASE_PAISE[] = {1000, 2000, 3000}; // Rs 10, Rs 20, Rs 30
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50 must remain on the card

long long fare(Zone z, bool peak)
{
    // TODO 1: Use a switch on z to pick the base fare from BASE_PAISE, then add
    // PEAK_SURCHARGE_PERCENT % in peak hours using integer arithmetic only
    long long base = 0;
    switch (z) {
        case Zone::Inner:
            base = BASE_PAISE[0];
            break;
        case Zone::Middle:
            base = BASE_PAISE[1];
            break;
        case Zone::Outer:
            base = BASE_PAISE[2];
            break;
        default:
            base = 0;
            break;
    }

    if (peak) {
        base += (base * PEAK_SURCHARGE_PERCENT) / 100;
    }
    return base;
}

bool canTravel(long long balance, long long f)
{
    // TODO 2: Return true only if the balance left after paying the fare is at least
    // MIN_BALANCE_PAISE
    return (balance - f) >= MIN_BALANCE_PAISE;
}

// Function to resolve peak status based on exact hour (8-10 AM and 5-8 PM)
bool isPeakHour(int hour) {
    // Peak hours are strictly [8, 10) and [17, 20)
    // 10:00 AM is 10 (Off-peak)
    // 8:00 PM is 20 (Off-peak)
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

int main()
{
    std::cout << "--- 1. Mandatory Test Cases (Expected Output Check) ---\n";
    long long balance = 10000; // Rs 100.00 in paise
    struct Trip { Zone zone; bool peak; };
    Trip trips[] = {{Zone::Inner, false}, {Zone::Outer, true}, {Zone::Middle, false}};
    for (const Trip& t : trips) {
        long long f = fare(t.zone, t.peak);
        if (canTravel(balance, f)) {
            balance -= f;
            std::cout << "Fare Rs " << f / 100 << '.' << std::setw(2)
                      << std::setfill('0') << f % 100 << std::setfill(' ');
        } else {
            std::cout << "Denied (low balance)";
        }
        std::cout << " balance Rs " << balance / 100 << '\n';
    }

    std::cout << "\n--- 2. Boundary Test Cases: Exactly 10 AM and Exactly 8 PM (20:00) ---\n";
    long long freshBalance = 10000; // Rs 100.00
    struct BoundaryTrip { std::string label; Zone zone; int hour; };
    BoundaryTrip boundaryTrips[] = {
        {"Trip at 10:00 AM (Boundary)", Zone::Outer, 10},
        {"Trip at 08:00 PM (Boundary)", Zone::Outer, 20}
    };

    for (const auto& bt : boundaryTrips) {
        bool peak = isPeakHour(bt.hour);
        long long f = fare(bt.zone, peak);
        std::cout << bt.label << " -> Peak Status: " << (peak ? "PEAK (20% surcharge)" : "OFF-PEAK (Standard fare)")
                  << " | Fare: Rs " << f / 100 << '.' << std::setw(2) << std::setfill('0') << f % 100 << std::setfill(' ');
        if (canTravel(freshBalance, f)) {
            freshBalance -= f;
            std::cout << " | Status: APPROVED | Balance Rs " << freshBalance / 100 << '\n';
        } else {
            std::cout << " | Status: DENIED (Low Balance) | Balance Rs " << freshBalance / 100 << '\n';
        }
    }

    return 0;
}
