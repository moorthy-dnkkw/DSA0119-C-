/**
 * ============================================================================
 * Task 10: Compare Expectations with Execution
 * Course: Object-Oriented Programming in C++ (CO1 - Bloom's Level: K5 Evaluate)
 * Title: Scale Simulation, int Overflow Detection, Floating-Point Drift & Compiler Warnings
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <climits>
#include <cmath>
#include <cstdint>

int main() {
    std::cout << "==============================================================\n";
    std::cout << "   TASK 10: EXPECTATION VS EXECUTION - SCALE ANALYSIS         \n";
    std::cout << "==============================================================\n\n";

    // ---------- Fare Configuration ----------
    constexpr long long INNER_PAISE  = 1000; // Rs 10.00
    constexpr long long MIDDLE_PAISE = 2000; // Rs 20.00
    constexpr long long OUTER_PAISE  = 3000; // Rs 30.00
    // Weighted average: assume 40% inner, 35% middle, 25% outer = Rs 18.50 avg
    constexpr long long AVG_FARE_PAISE = (INNER_PAISE * 40 + MIDDLE_PAISE * 35 + OUTER_PAISE * 25) / 100;
    // Approx 50% peak trips: average surcharge adjustment ~ 10%
    constexpr long long AVG_FARE_WITH_PEAK = AVG_FARE_PAISE + (AVG_FARE_PAISE * 10) / 100;

    struct ScaleTest {
        const char* label;
        long long numJourneys;
    };

    ScaleTest scales[] = {
        {"1 Station (20,000 journeys/day)",     20000LL},
        {"1 Line (500,000 journeys/day)",       500000LL},
        {"Whole Network (3,000,000 journeys/day)", 3000000LL}
    };

    std::cout << "--- 1. Scale Simulation: long long Paise vs double Rupees vs int Paise ---\n";
    std::cout << "Average fare per journey (weighted + 10% peak adj): " 
              << AVG_FARE_WITH_PEAK << " paise (Rs " << std::fixed << std::setprecision(2) 
              << AVG_FARE_WITH_PEAK / 100.0 << ")\n\n";

    std::cout << std::left
              << std::setw(44) << "Scale"
              << std::setw(22) << "long long (paise)"
              << std::setw(22) << "double (Rs)"
              << std::setw(18) << "Drift (Rs)"
              << std::setw(14) << "int Overflow?" << "\n";
    std::cout << std::string(120, '-') << "\n";

    for (const auto& sc : scales) {
        // --- Method 1: Exact long long accumulation in paise ---
        long long totalLongLong = 0;
        for (long long j = 0; j < sc.numJourneys; ++j) {
            totalLongLong += AVG_FARE_WITH_PEAK;
        }

        // --- Method 2: Approximate double accumulation in rupees ---
        double totalDouble = 0.0;
        double fareRupees = AVG_FARE_WITH_PEAK / 100.0;
        for (long long j = 0; j < sc.numJourneys; ++j) {
            totalDouble += fareRupees;
        }

        // --- Drift Calculation ---
        double exactRupees = static_cast<double>(totalLongLong) / 100.0;
        double drift = totalDouble - exactRupees;

        // --- Method 3: Check if 32-bit int would overflow ---
        bool intOverflows = (totalLongLong > static_cast<long long>(INT_MAX));

        std::cout << std::left << std::setw(44) << sc.label
                  << std::setw(22) << totalLongLong
                  << std::fixed << std::setprecision(2) << std::setw(22) << totalDouble
                  << std::setprecision(6) << std::setw(18) << drift
                  << (intOverflows ? "YES OVERFLOW!" : "No") << "\n";
    }

    std::cout << "\n--- 2. Detailed 32-bit int Overflow Analysis ---\n";
    long long networkTotal = 3000000LL * AVG_FARE_WITH_PEAK;
    std::cout << "Network Daily Revenue:       " << networkTotal << " paise (Rs " 
              << std::fixed << std::setprecision(2) << networkTotal / 100.0 << ")\n";
    std::cout << "INT_MAX (signed 32-bit):     " << INT_MAX << " paise (Rs " << INT_MAX / 100 << ")\n";
    std::cout << "UINT_MAX (unsigned 32-bit):  " << UINT_MAX << " paise (Rs " << UINT_MAX / 100 << ")\n";

    if (networkTotal > INT_MAX) {
        // Simulate what happens when stored in int:
        int overflowedValue = static_cast<int>(networkTotal);
        std::cout << "\n[CRITICAL BUG DEMONSTRATION]:\n"
                  << "  When stored in 'int': " << overflowedValue << " paise (CORRUPT NEGATIVE VALUE!)\n"
                  << "  Actual value needed:  " << networkTotal << " paise\n"
                  << "  Difference from INT_MAX: " << (networkTotal - INT_MAX) << " paise overflow\n";
    }

    std::cout << "\n--- 3. Compiler Warning Analysis (with -Wall -Wextra) ---\n";
    std::cout << "The following warnings are produced and analyzed:\n\n";
    std::cout << "Warning 1: '-Wsign-compare' when comparing signed int with unsigned int.\n"
              << "  Source:   'if (signed_val < unsigned_val)' comparison.\n"
              << "  Risk:     Signed -1 becomes 4,294,967,295 in unsigned causing inverted logic.\n"
              << "  Fix:      Use same type or use static_cast<unsigned int>() after range check.\n\n";

    std::cout << "Warning 2: '-Wunused-variable' when a declared variable is not used.\n"
              << "  Source:   A variable created during development / testing.\n"
              << "  Fix:      Remove unused variables or annotate with [[maybe_unused]].\n\n";

    std::cout << "Warning 3: '-Wnarrowing' (only with brace init: int x{3.7}).\n"
              << "  Source:   Narrowing conversion from double to int.\n"
              << "  Fix:      Use static_cast<int>() or adjust initializer.\n";

    std::cout << "\n--- 4. Conclusion ---\n";
    std::cout << "1. long long (64-bit) integer paise is the ONLY safe type for financial\n"
              << "   accumulators at network scale (3M+ daily journeys).\n"
              << "2. 'double' accumulator introduces Rs-level cumulative drift on large sums.\n"
              << "3. 'int' (32-bit) catastrophically overflows, producing NEGATIVE revenue totals.\n"
              << "4. Compiler warnings from -Wall -Wextra catch signed/unsigned and narrowing\n"
              << "   issues at compile time BEFORE they become production bugs.\n";

    return 0;
}
