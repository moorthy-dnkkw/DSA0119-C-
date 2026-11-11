/**
 * ============================================================================
 * Task 2: Use Tokens, Keywords, Identifiers and Constants Correctly
 * Course: Object-Oriented Programming in C++ (CO1 - Bloom's Level: K5 Evaluate)
 * Title: Centralized Constants, Modern C++ Token Design, and Type Safety
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>

// ============================================================================
// TOKEN CATEGORY DEMONSTRATIONS:
// 1. Keywords:    enum, class, constexpr, long, bool, switch, case, return, const
// 2. Identifiers: Zone, FareConfig, INNER_BASE_PAISE, calculateFare
// 3. Constants:   1000LL, 20, 5000LL, true, "Inner"
// 4. Operators:   ::, =, +, *, /, ==, >=, &&, ||, <<
// 5. Punctuators: {, }, (, ), ;, :, ,, .
// ============================================================================

// ----------------------------------------------------------------------------
// Scoped Enumeration (enum class) vs Legacy Unscoped enum
// ----------------------------------------------------------------------------
// enum class prevents namespace pollution and forbids implicit conversion to int.
enum class Zone : int {
    Inner = 1,
    Middle = 2,
    Outer = 3
};

// ----------------------------------------------------------------------------
// Single Source of Truth: Centralized constexpr Constants
// ----------------------------------------------------------------------------
// constexpr evaluates at compile time, incurs zero runtime memory overhead,
// maintains strict C++ type checking, and respects lexical scoping rules.
namespace FareConfig {
    constexpr long long INNER_ZONE_PAISE    = 1000; // Rs 10.00
    constexpr long long MIDDLE_ZONE_PAISE   = 2000; // Rs 20.00
    constexpr long long OUTER_ZONE_PAISE    = 3000; // Rs 30.00
    
    constexpr int PEAK_SURCHARGE_PERCENT    = 20;   // 20%
    constexpr long long MIN_BALANCE_PAISE   = 5000; // Rs 50.00 required post-journey
    
    constexpr int MORNING_PEAK_START_HOUR   = 8;
    constexpr int MORNING_PEAK_END_HOUR     = 10;
    constexpr int EVENING_PEAK_START_HOUR   = 17;
    constexpr int EVENING_PEAK_END_HOUR     = 20;
}

// ----------------------------------------------------------------------------
// Demonstration of Preprocessor Macro (#define) Pitfalls vs constexpr
// ----------------------------------------------------------------------------
// Legacy Macro: No type, no scope, vulnerable to textual substitution side-effects
#define MACRO_SURCHARGE_BAD 20 / 100
#define MACRO_SURCHARGE_GOOD (20)

constexpr long long getBaseFare(Zone zone) {
    switch (zone) {
        case Zone::Inner:  return FareConfig::INNER_ZONE_PAISE;
        case Zone::Middle: return FareConfig::MIDDLE_ZONE_PAISE;
        case Zone::Outer:  return FareConfig::OUTER_ZONE_PAISE;
        default:           return 0;
    }
}

constexpr bool isPeakTime(int hour) {
    return (hour >= FareConfig::MORNING_PEAK_START_HOUR && hour < FareConfig::MORNING_PEAK_END_HOUR) ||
           (hour >= FareConfig::EVENING_PEAK_START_HOUR && hour < FareConfig::EVENING_PEAK_END_HOUR);
}

constexpr long long computeFinalFare(Zone zone, int hour) {
    long long base = getBaseFare(zone);
    if (isPeakTime(hour)) {
        return base + (base * FareConfig::PEAK_SURCHARGE_PERCENT) / 100;
    }
    return base;
}

int main() {
    std::cout << "==============================================================\n";
    std::cout << "    TASK 2: TOKENS, IDENTIFIERS, AND CONSTEXPR CONSTANTS      \n";
    std::cout << "==============================================================\n\n";

    std::cout << "--- 1. Compile-Time Evaluation of Fares (constexpr) ---\n";
    // Because computeFinalFare is constexpr, this can be evaluated at compile-time!
    constexpr long long compileTimePeakFare = computeFinalFare(Zone::Outer, 8);
    std::cout << "Outer Zone Peak Fare (Compile-Time Computed): " 
              << compileTimePeakFare << " paise (Rs " << compileTimePeakFare / 100.0 << ")\n";

    std::cout << "\n--- 2. Single Point of Truth Constants Audit ---\n";
    std::cout << "Inner Zone Base Fare:   Rs " << FareConfig::INNER_ZONE_PAISE / 100.0 << "\n";
    std::cout << "Middle Zone Base Fare:  Rs " << FareConfig::MIDDLE_ZONE_PAISE / 100.0 << "\n";
    std::cout << "Outer Zone Base Fare:   Rs " << FareConfig::OUTER_ZONE_PAISE / 100.0 << "\n";
    std::cout << "Peak Hour Surcharge:    "    << FareConfig::PEAK_SURCHARGE_PERCENT << "%\n";
    std::cout << "Required Min Balance:   Rs " << FareConfig::MIN_BALANCE_PAISE / 100.0 << "\n";

    std::cout << "\n--- 3. Type Safety Evaluation: enum class vs unscoped enum ---\n";
    Zone myZone = Zone::Middle;
    // int x = myZone; // COMPILE ERROR! enum class prevents accidental arithmetic
    std::cout << "Selected Zone is safely typed with underlying int: " 
              << static_cast<int>(myZone) << "\n";

    std::cout << "\n--- 4. Comparison: #define vs const vs constexpr ---\n";
    std::cout << "Feature            | #define           | const              | constexpr\n";
    std::cout << "-------------------|-------------------|--------------------|--------------------\n";
    std::cout << "Type Checking      | None (Text Sub)   | Yes                | Yes (Strict)\n";
    std::cout << "Scope              | Global file scope | Block/Namespace    | Block/Namespace\n";
    std::cout << "Evaluation Time    | Preprocessor      | Runtime or Compile | Guaranteed Compile\n";
    std::cout << "Memory Storage     | 0 bytes           | May allocate rodata| Zero overhead\n";
    std::cout << "Debugger Visibility| Invisible         | Visible symbol     | Visible symbol\n";

    return 0;
}
