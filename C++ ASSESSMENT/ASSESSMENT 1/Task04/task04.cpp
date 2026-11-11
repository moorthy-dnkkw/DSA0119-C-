/**
 * ============================================================================
 * Task 4: Control Implicit Conversions
 * Course: Object-Oriented Programming in C++ (CO1 - Bloom's Level: K5 Evaluate)
 * Title: Type Conversions, Integral Promotions, Sign Mismatches & Narrowing
 * ============================================================================
 */

#include <iostream>
#include <iomanip>

int main()
{
    std::cout << "==============================================================\n";
    std::cout << "        TASK 4: IMPLICIT TYPE CONVERSIONS ANALYSIS            \n";
    std::cout << "==============================================================\n\n";

    std::cout << "--- Standard Given Code Execution ---\n";

    int a = 7, b = 2;
    double c = a / b;

    double d = static_cast<double>(a) / b;
    std::cout << c << ' ' << d << '\n';

    char ch = 'A' + 2;
    std::cout << ch << ' ' << ch + 1 << '\n';

    bool flag = 5;
    std::cout << flag << ' ' << std::boolalpha << flag << '\n';

    unsigned int u = 1;
    int s = -1;
    // Suppress warning during execution demonstration, but analyzed in report
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-compare"
    std::cout << (s < u ? "s < u" : "s >= u") << '\n';
    #pragma GCC diagnostic pop

    std::cout << "\n--- Line-by-Line Conversion Mechanics Explanation ---\n";
    std::cout << "Line 1: 'a / b' executes integer division (7 / 2 = 3).\n"
              << "        The integer 3 is implicitly promoted to double 3.0 on assignment to 'c'.\n"
              << "        In 'd', static_cast<double>(a) converts 7 to 7.0.\n"
              << "        Then 'b' (2) undergoes arithmetic promotion to 2.0, giving exact 3.5.\n";

    std::cout << "Line 2: 'A' has ASCII value 65. 65 + 2 = 67 ('C').\n"
              << "        Printing 'ch' outputs the character 'C'.\n"
              << "        In 'ch + 1', 'ch' undergoes integral promotion to int (67).\n"
              << "        The sum evaluates to int 68, which is printed as a number.\n";

    std::cout << "Line 3: 5 is non-zero, so implicit conversion to bool yields true (1).\n"
              << "        std::cout outputs 1, and with std::boolalpha outputs 'true'.\n";

    std::cout << "Line 4: In 's < u', C++ usual arithmetic conversions dictate that signed int\n"
              << "        is promoted to unsigned int. -1 in two's-complement 32-bit unsigned\n"
              << "        becomes 4,294,967,295. Since 4294967295 < 1 is FALSE, it prints 's >= u'!\n"
              << "        This is a classic silent security vulnerability in balance / fare checks.\n";

    std::cout << "\n--- Prevention: Modern C++ Explicit Conversions and Uniform Init ---\n";
    // Safe sign comparison:
    bool safeCompare = (s < 0) ? true : (static_cast<unsigned int>(s) < u);
    std::cout << "Safe Sign-Aware Comparison (-1 < 1u): " 
              << (safeCompare ? "s < u (Correct)" : "s >= u (Bug)") << "\n";

    std::cout << "Uniform Initialization (int x{3.7};) would trigger a compile-time ERROR:\n"
              << "error: narrowing conversion of '3.7e+0' from 'double' to 'int'\n";

    return 0;
}
