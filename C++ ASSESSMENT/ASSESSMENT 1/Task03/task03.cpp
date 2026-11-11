/**
 * ============================================================================
 * Task 3: Choose Data Types and Check Type Compatibility
 * Course: Object-Oriented Programming in C++ (CO1 - Bloom's Level: K5 Evaluate)
 * Title: Data Type Selection, Hardware Sizes, climits, and Precision Analysis
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <climits>
#include <cstdint>

int main() {
    std::cout << "==============================================================\n";
    std::cout << "    TASK 3: DATA TYPES, SIZES, COMPATIBILITY & PRECISION      \n";
    std::cout << "==============================================================\n\n";

    std::cout << "--- 1. Measured Data Type Sizes and System Ranges ---\n";
    std::cout << std::left << std::setw(16) << "Type" 
              << std::setw(10) << "Size (B)" 
              << std::setw(25) << "Min Value" 
              << std::setw(25) << "Max Value" << "\n";
    std::cout << std::string(76, '-') << "\n";

    std::cout << std::setw(16) << "bool" 
              << std::setw(10) << sizeof(bool) 
              << std::setw(25) << 0 
              << std::setw(25) << 1 << "\n";

    std::cout << std::setw(16) << "char" 
              << std::setw(10) << sizeof(char) 
              << std::setw(25) << CHAR_MIN 
              << std::setw(25) << CHAR_MAX << "\n";

    std::cout << std::setw(16) << "int" 
              << std::setw(10) << sizeof(int) 
              << std::setw(25) << INT_MIN 
              << std::setw(25) << INT_MAX << "\n";

    std::cout << std::setw(16) << "unsigned int" 
              << std::setw(10) << sizeof(unsigned int) 
              << std::setw(25) << 0 
              << std::setw(25) << UINT_MAX << "\n";

    std::cout << std::setw(16) << "long long" 
              << std::setw(10) << sizeof(long long) 
              << std::setw(25) << LLONG_MIN 
              << std::setw(25) << LLONG_MAX << "\n";

    std::cout << std::setw(16) << "float" 
              << std::setw(10) << sizeof(float) 
              << std::setw(25) << "Approx 10^-38" 
              << std::setw(25) << "Approx 10^38" << "\n";

    std::cout << std::setw(16) << "double" 
              << std::setw(10) << sizeof(double) 
              << std::setw(25) << "Approx 10^-308" 
              << std::setw(25) << "Approx 10^308" << "\n";

    std::cout << "\n--- 2. Network Scale Analysis: Is 32-bit int Sufficient? ---\n";
    long long networkJourneys = 3000000LL; // 3 million journeys/day
    long long avgFarePaise = 2500LL;       // Rs 25.00 average fare
    long long totalRevenuePaise = networkJourneys * avgFarePaise;

    std::cout << "Network daily journeys: " << networkJourneys << "\n";
    std::cout << "Average fare:           " << avgFarePaise << " paise (Rs 25.00)\n";
    std::cout << "Expected daily revenue: " << totalRevenuePaise << " paise (Rs " << totalRevenuePaise / 100 << ")\n";
    std::cout << "Signed 32-bit INT_MAX:  " << INT_MAX << " paise (Rs " << INT_MAX / 100 << ")\n";

    if (totalRevenuePaise > INT_MAX) {
        std::cout << "[EVALUATION RESULT]: 32-bit signed 'int' OVERFLOWS by " 
                  << (totalRevenuePaise - INT_MAX) << " paise!\n"
                  << "If accumulated in 'int', the total wraps to a negative value.\n"
                  << "Verdict: 'long long' (64-bit) is strictly required for financial accumulators.\n";
    }

    std::cout << "\n--- 3. IEEE 754 Floating-Point Drift: double Rupees vs long long Paise ---\n";
    // Adding 10.10 rupees (which cannot be represented exactly in binary floating point)
    double doubleBalanceRupees = 0.0;
    long long intBalancePaise = 0;
    const int iterations = 100000;
    const double stepRupees = 10.10;
    const long long stepPaise = 1010; // Rs 10.10 in paise

    for (int i = 0; i < iterations; ++i) {
        doubleBalanceRupees += stepRupees;
        intBalancePaise += stepPaise;
    }

    double convertedIntRupees = static_cast<double>(intBalancePaise) / 100.0;
    double difference = doubleBalanceRupees - convertedIntRupees;

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Iterations:                     " << iterations << "\n";
    std::cout << "Accumulated with double (Rs):   " << doubleBalanceRupees << "\n";
    std::cout << "Accumulated with integer paise: " << convertedIntRupees << " (exact: " << intBalancePaise << " paise)\n";
    std::cout << "Cumulative Drift / Error:       " << difference << " Rupees\n";
    std::cout << "Lost / Discrepant Paise:        " << std::setprecision(4) << difference * 100.0 << " paise\n";

    std::cout << "\n--- 4. Type Compatibility & Implicit Promotion Rules ---\n";
    int iVal = 10;
    double dVal = 2.5;
    bool bVal = true;
    // In expression: iVal + dVal * bVal:
    // 1. bVal (true) promoted to int (1)
    // 2. dVal * 1 is double (2.5)
    // 3. iVal is promoted to double (10.0) -> result 12.5
    auto mixedResult = iVal + dVal * bVal;
    std::cout << "Expression: int(10) + double(2.5) * bool(true)\n";
    std::cout << "Result Value: " << mixedResult 
              << " | Result Type Size: " << sizeof(mixedResult) << " bytes (double)\n";

    return 0;
}
