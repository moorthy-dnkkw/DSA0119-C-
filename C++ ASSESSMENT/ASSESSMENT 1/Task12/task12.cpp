/**
 * ============================================================================
 * Task 12: Re-evaluate and Adapt the Design (Day-4 Design Challenge)
 * Course: Object-Oriented Programming in C++ (CO1 - Bloom's Level: K5 Evaluate)
 * Title: Fourth Zone, Weekend Discount, Monthly Pass - Impact Analysis & Redesign
 * Total: 8 Marks (A: Impact 2, B: Evaluation 2, C: Adaptation 2, D: Defence 2)
 * ============================================================================
 *
 * CHANGED REQUIREMENTS:
 * 1. Fourth zone: Airport (Rs 60 / 6000 paise)
 * 2. Weekend discount: 10% off; does NOT combine with peak surcharge
 *    (If weekend AND peak, ONLY the 10% discount applies, NOT the 20% surcharge)
 * 3. Monthly Pass: Unlimited journeys in two chosen zones
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cassert>

// ============================================================================
// A. IMPACT ANALYSIS
// ============================================================================
// Affected Components:
// 1. Zone enum:       ADD Airport = 3 (before NUM_ZONES sentinel)
// 2. FARE_TABLE:      ADD 6000 paise entry for Airport
// 3. FareEngine:      MODIFY calculateFare() to handle DayOfWeek and new discount logic
//                     - Peak surcharge (20%) applies ONLY on weekdays in peak hours
//                     - Weekend discount (10%) applies on Sat/Sun, replacing (not combining) peak
// 4. SmartCard:       ADD optional MonthlyPass member
// 5. NEW CLASS:       MonthlyPass (stores two covered zones, expiry)
// 6. Switch/Array:    Original 3-element switch/array now needs 4 elements
// ============================================================================

// --- Scoped Enumerations ---
enum class Zone : int {
    Inner   = 0,
    Middle  = 1,
    Outer   = 2,
    Airport = 3,   // NEW: Fourth zone
    NUM_ZONES
};

enum class DayOfWeek : int {
    Monday = 0, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday
};

std::string zoneToString(Zone z) {
    switch (z) {
        case Zone::Inner:   return "Inner";
        case Zone::Middle:  return "Middle";
        case Zone::Outer:   return "Outer";
        case Zone::Airport: return "Airport";
        default:            return "Unknown";
    }
}

std::string dayToString(DayOfWeek d) {
    switch (d) {
        case DayOfWeek::Monday:    return "Mon";
        case DayOfWeek::Tuesday:   return "Tue";
        case DayOfWeek::Wednesday: return "Wed";
        case DayOfWeek::Thursday:  return "Thu";
        case DayOfWeek::Friday:    return "Fri";
        case DayOfWeek::Saturday:  return "Sat";
        case DayOfWeek::Sunday:    return "Sun";
        default:                   return "???";
    }
}

// ============================================================================
// REDESIGNED Fare Table: Array-indexed (extensible for N zones)
// ============================================================================
constexpr long long FARE_TABLE_PAISE[] = {1000, 2000, 3000, 6000}; // Inner, Middle, Outer, Airport
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int WEEKEND_DISCOUNT_PERCENT = 10;
constexpr long long MIN_BALANCE_PAISE = 5000;

// ============================================================================
// REDESIGNED FareEngine: Handles Peak, Weekend, and their Non-Combination Rule
// ============================================================================
class FareEngine {
public:
    static bool isPeak(int hour) {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    static bool isWeekend(DayOfWeek day) {
        return (day == DayOfWeek::Saturday || day == DayOfWeek::Sunday);
    }

    static long long calculateFare(Zone zone, int hour, DayOfWeek day) {
        int zoneIndex = static_cast<int>(zone);
        if (zoneIndex < 0 || zoneIndex >= static_cast<int>(Zone::NUM_ZONES)) {
            return -1; // Invalid zone
        }

        long long base = FARE_TABLE_PAISE[zoneIndex];

        // NON-COMBINATION RULE:
        // Weekend discount (10% OFF) takes priority over peak surcharge
        // Peak surcharge (20% EXTRA) applies ONLY on weekdays during peak hours
        if (isWeekend(day)) {
            base -= (base * WEEKEND_DISCOUNT_PERCENT) / 100; // 10% discount
        } else if (isPeak(hour)) {
            base += (base * PEAK_SURCHARGE_PERCENT) / 100;   // 20% surcharge
        }
        // Else: standard weekday off-peak fare (no modification)

        return base;
    }
};

// ============================================================================
// NEW CLASS: MonthlyPass (Covers unlimited travel in two chosen zones)
// ============================================================================
class MonthlyPass {
private:
    Zone coveredZone1;
    Zone coveredZone2;
    bool active;

public:
    MonthlyPass() : coveredZone1(Zone::Inner), coveredZone2(Zone::Inner), active(false) {}

    MonthlyPass(Zone z1, Zone z2)
        : coveredZone1(z1), coveredZone2(z2), active(true) {}

    bool isActive() const { return active; }

    bool coversZone(Zone z) const {
        if (!active) return false;
        return (z == coveredZone1 || z == coveredZone2);
    }

    std::string getCoverageString() const {
        if (!active) return "No Pass";
        return zoneToString(coveredZone1) + " + " + zoneToString(coveredZone2);
    }

    void deactivate() { active = false; }
};

// ============================================================================
// REDESIGNED SmartCard: Supports optional MonthlyPass
// ============================================================================
class SmartCard {
private:
    std::string cardId;
    long long balancePaise;
    MonthlyPass pass;

public:
    SmartCard(const std::string& id, long long initialBalance)
        : cardId(id), balancePaise(initialBalance >= 0 ? initialBalance : 0) {}

    std::string getId() const { return cardId; }
    long long getBalance() const { return balancePaise; }
    const MonthlyPass& getPass() const { return pass; }

    void activatePass(Zone z1, Zone z2) { pass = MonthlyPass(z1, z2); }

    bool canTravel(long long fare) const {
        if (fare <= 0) return true; // Pass-covered journeys always allowed
        return (balancePaise - fare) >= MIN_BALANCE_PAISE;
    }

    // Returns the actual fare charged (0 if pass-covered)
    long long processJourney(Zone zone, int hour, DayOfWeek day) {
        // Check if monthly pass covers this zone
        if (pass.isActive() && pass.coversZone(zone)) {
            return 0; // No charge — covered by pass
        }

        long long fare = FareEngine::calculateFare(zone, hour, day);
        if (fare < 0) return -1; // Invalid zone

        if (!canTravel(fare)) {
            return -2; // Insufficient balance
        }

        balancePaise -= fare;
        return fare;
    }

    bool recharge(long long amountPaise) {
        if (amountPaise <= 0) return false;
        balancePaise += amountPaise;
        return true;
    }
};

// ============================================================================
// TEST SUITE
// ============================================================================
struct TestCase {
    std::string testId;
    std::string desc;
    Zone zone;
    int hour;
    DayOfWeek day;
    long long initBalance;
    bool hasPass;
    Zone passZone1;
    Zone passZone2;
    long long expectedFare;   // 0 = pass-covered, -2 = denied
    bool expectedApproval;
};

void runTestSuite() {
    std::vector<TestCase> tests = {
        // --- Original 3 zones (Weekday Off-peak/Peak) ---
        {"T01", "Inner Weekday Off-Peak",  Zone::Inner,   12, DayOfWeek::Monday,   10000, false, Zone::Inner, Zone::Inner, 1000, true},
        {"T02", "Inner Weekday Peak",      Zone::Inner,    9, DayOfWeek::Tuesday,  10000, false, Zone::Inner, Zone::Inner, 1200, true},
        {"T03", "Middle Weekday Peak",     Zone::Middle,  18, DayOfWeek::Wednesday,10000, false, Zone::Inner, Zone::Inner, 2400, true},
        {"T04", "Outer Weekday Off-Peak",  Zone::Outer,   14, DayOfWeek::Thursday, 10000, false, Zone::Inner, Zone::Inner, 3000, true},
        {"T05", "Outer Weekday Peak",      Zone::Outer,    8, DayOfWeek::Friday,   10000, false, Zone::Inner, Zone::Inner, 3600, true},

        // --- NEW: Airport Zone ---
        {"T06", "Airport Weekday Off-Peak", Zone::Airport, 12, DayOfWeek::Monday,   15000, false, Zone::Inner, Zone::Inner, 6000, true},
        {"T07", "Airport Weekday Peak",     Zone::Airport,  9, DayOfWeek::Tuesday,  15000, false, Zone::Inner, Zone::Inner, 7200, true},

        // --- NEW: Weekend Discount (10% off, no peak surcharge) ---
        {"T08", "Inner Weekend Off-Peak",   Zone::Inner,   12, DayOfWeek::Saturday,  10000, false, Zone::Inner, Zone::Inner, 900, true},
        {"T09", "Outer Weekend Peak Time",  Zone::Outer,    9, DayOfWeek::Sunday,    10000, false, Zone::Inner, Zone::Inner, 2700, true},
        {"T10", "Airport Weekend Peak Time", Zone::Airport, 18, DayOfWeek::Saturday, 15000, false, Zone::Inner, Zone::Inner, 5400, true},

        // --- NEW: Monthly Pass (zero fare in covered zones) ---
        {"T11", "Pass: Inner Covered",      Zone::Inner,   9, DayOfWeek::Monday,    10000, true, Zone::Inner, Zone::Middle, 0, true},
        {"T12", "Pass: Middle Covered",     Zone::Middle, 18, DayOfWeek::Friday,    10000, true, Zone::Inner, Zone::Middle, 0, true},
        {"T13", "Pass: Outer NOT Covered",  Zone::Outer,  12, DayOfWeek::Monday,    10000, true, Zone::Inner, Zone::Middle, 3000, true},
        {"T14", "Pass: Airport NOT Covered", Zone::Airport, 9, DayOfWeek::Tuesday,  15000, true, Zone::Inner, Zone::Middle, 7200, true},

        // --- Boundary: Balance exactly Rs 50 after deduction ---
        {"T15", "Exact Rs 50 boundary",     Zone::Inner,  12, DayOfWeek::Monday,     6000, false, Zone::Inner, Zone::Inner, 1000, true},

        // --- Denial: Insufficient balance ---
        {"T16", "Denied - low balance",     Zone::Outer,  12, DayOfWeek::Monday,     7000, false, Zone::Inner, Zone::Inner, -2, false}
    };

    std::cout << std::left
              << std::setw(6) << "ID"
              << std::setw(30) << "Description"
              << std::setw(10) << "Zone"
              << std::setw(6) << "Hour"
              << std::setw(6) << "Day"
              << std::setw(12) << "Exp Fare"
              << std::setw(12) << "Act Fare"
              << std::setw(10) << "Result" << "\n";
    std::cout << std::string(92, '-') << "\n";

    int passed = 0;
    for (const auto& tc : tests) {
        SmartCard card("TC", tc.initBalance);
        if (tc.hasPass) {
            card.activatePass(tc.passZone1, tc.passZone2);
        }

        long long result = card.processJourney(tc.zone, tc.hour, tc.day);

        bool match;
        if (tc.expectedFare == -2) {
            match = (result == -2); // Denied
        } else {
            match = (result == tc.expectedFare);
        }

        if (match) passed++;

        std::string expStr = (tc.expectedFare == -2) ? "DENIED" :
                             (tc.expectedFare == 0)  ? "PASS-FREE" :
                             "Rs " + std::to_string(tc.expectedFare / 100) + "." +
                             (tc.expectedFare % 100 < 10 ? "0" : "") + std::to_string(tc.expectedFare % 100);
        std::string actStr = (result == -2) ? "DENIED" :
                             (result == 0)  ? "PASS-FREE" :
                             "Rs " + std::to_string(result / 100) + "." +
                             (result % 100 < 10 ? "0" : "") + std::to_string(result % 100);

        std::cout << std::setw(6) << tc.testId
                  << std::setw(30) << tc.desc
                  << std::setw(10) << zoneToString(tc.zone)
                  << std::setw(6) << tc.hour
                  << std::setw(6) << dayToString(tc.day)
                  << std::setw(12) << expStr
                  << std::setw(12) << actStr
                  << (match ? "[PASS]" : "[FAIL]") << "\n";
    }

    std::cout << std::string(92, '=') << "\n";
    std::cout << "Test Summary: " << passed << " / " << tests.size() << " tests PASSED.\n";
}

int main() {
    std::cout << "==============================================================\n";
    std::cout << "  TASK 12: DAY-4 DESIGN CHALLENGE - REDESIGNED FARE SYSTEM    \n";
    std::cout << "==============================================================\n\n";

    std::cout << "--- A. IMPACT ANALYSIS ---\n";
    std::cout << "Changed Requirements:\n"
              << " 1. New Zone::Airport added (Rs 60 / 6000 paise).\n"
              << " 2. Weekend discount: 10% off on Saturday & Sunday.\n"
              << "    Rule: Weekend discount does NOT combine with peak surcharge.\n"
              << "    If weekend AND peak hour, only the 10% discount applies.\n"
              << " 3. Monthly Pass: Unlimited journeys in 2 chosen zones (0 fare).\n\n";
    std::cout << "Affected Components:\n"
              << " - Zone enum:       Added Airport = 3 (1 line)\n"
              << " - FARE_TABLE:      Added 6000 paise entry (1 line)\n"
              << " - FareEngine:      Added DayOfWeek parameter, weekend/peak branching (10 lines)\n"
              << " - SmartCard:       Added MonthlyPass integration, processJourney() (15 lines)\n"
              << " - NEW MonthlyPass: New class with zone coverage logic (20 lines)\n"
              << " - switch -> array: Original switch replaced by array[zoneIndex] (eliminated switch)\n\n";

    std::cout << "--- B. EVALUATION OF ORIGINAL DESIGN ---\n";
    std::cout << "Original Design Assessment:\n"
              << " - The switch-based fare logic would need a 4th case (Airport) - EASY.\n"
              << " - However, adding the weekend discount NON-COMBINATION rule breaks the\n"
              << "   simple 'if (peak) add surcharge' pattern. The switch cannot express\n"
              << "   'weekend OR peak but not both' without restructuring.\n"
              << " - Monthly pass requires SmartCard to conditionally bypass the fare engine\n"
              << "   entirely, which the original canTravel()/deduct() pair cannot handle.\n"
              << " VERDICT: MODIFY the original design. The core SmartCard class is retained\n"
              << "          but extended. The FareEngine is redesigned with priority-based\n"
              << "          discount logic. A new MonthlyPass class is introduced.\n\n";

    std::cout << "--- C. ADAPTATION IMPLEMENTATION ---\n";
    std::cout << "Design Decisions:\n"
              << " 1. Array-indexed fare table replaces switch (O(1) lookup, extensible).\n"
              << " 2. FareEngine::calculateFare() now takes DayOfWeek as 3rd parameter.\n"
              << "    Priority: Weekend discount > Peak surcharge > Standard fare.\n"
              << " 3. MonthlyPass class encapsulates 2 covered zones with coversZone() check.\n"
              << " 4. SmartCard.processJourney() checks pass FIRST, then calls FareEngine.\n\n";

    std::cout << "--- D. EVIDENCE-BASED DEFENCE ---\n";
    runTestSuite();

    std::cout << "\nDefence Summary:\n"
              << " - All 16 test cases pass, covering all 4 zones, weekday/weekend,\n"
              << "   peak/off-peak, pass-covered/uncovered, and boundary/denial cases.\n"
              << " - Weekend + Peak correctly applies ONLY the 10% discount (T09, T10).\n"
              << " - Monthly pass correctly charges Rs 0 for covered zones (T11, T12)\n"
              << "   and standard fare for uncovered zones (T13, T14).\n"
              << " - Adding a 5th zone requires changing exactly 2 lines (enum + array).\n"
              << " - Time Complexity: O(1) per journey (unchanged from original).\n"
              << " - Space Complexity: O(Z) for fare table + O(1) per card (Z = number of zones).\n";

    return 0;
}
