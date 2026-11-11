/**
 * ============================================================================
 * Task 5: Write Correct Expressions with the Right Precedence
 * Course: Object-Oriented Programming in C++ (CO1 - Bloom's Level: K5 Evaluate)
 * Title: Operator Precedence, Associativity, Sequence Points & Lookup Tables
 * ============================================================================
 */

#include <iostream>
#include <iomanip>

int main()
{
    std::cout << "==============================================================\n";
    std::cout << "     TASK 5: OPERATOR PRECEDENCE & EXPRESSION EVALUATION      \n";
    std::cout << "==============================================================\n\n";

    std::cout << "--- Standard Given Code Execution ---\n";
    int x = 5, y = 3;
    std::cout << x + y * 2 << ' ' << (x + y) * 2 << '\n';

    int z = x++ + ++y;
    std::cout << z << ' ' << x << ' ' << y << '\n';

    std::cout << (x > y ? x : y) << ' ' << x % 4 * 2 << '\n';
    std::cout << 10 / 3 << ' ' << 10 % 3 << ' ' << -10 / 3 << ' ' << -10 % 3 << '\n';

    int k = 0;
    for (int i = 0; i < 10; i += 3) k += i;
    std::cout << k << '\n';

    std::cout << "\n--- Operator-by-Operator Precedence Breakdown ---\n";
    std::cout << "1. 'x + y * 2': '*' (Precedence 5) binds tighter than '+' (Precedence 6).\n"
              << "   Evaluates as 5 + (3 * 2) = 11. Parentheses in '(x + y) * 2' override to (5 + 3) * 2 = 16.\n";
    std::cout << "2. 'x++ + ++y': Postfix '++' has higher precedence (2) than prefix '++' (3),\n"
              << "   and '+' is level 6. '++y' increments y to 4 and evaluates to 4.\n"
              << "   'x++' evaluates to 5, then increments x to 6. z = 5 + 4 = 9.\n";
    std::cout << "3. 'x % 4 * 2': '%' and '*' have equal precedence (5) and associate Left-to-Right.\n"
              << "   Evaluates as (6 % 4) * 2 = 2 * 2 = 4.\n";
    std::cout << "4. Negative integer division/modulo in C++11 (ISO/IEC 14882:2011 5.6/4):\n"
              << "   Truncation is always toward zero. Hence -10 / 3 = -3, and -10 % 3 = -1.\n";
    std::cout << "5. 'i = i++' Hazard: Modifying a scalar object more than once without an intervening\n"
              << "   sequence point is undefined behavior (C++03) or unsequenced modification (C++11/17).\n";

    std::cout << "\n--- Alternative Concept: Peak Hour Lookup Table bool peak[24] ---\n";
    // Array lookup avoids multiple branch instructions and CPU branch mispredictions:
    constexpr bool PEAK_TABLE[24] = {
        false, false, false, false, false, false, false, false, // 0 - 7
        true,  true,                                            // 8, 9  (8:00 - 9:59)
        false, false, false, false, false, false, false,       // 10 - 16
        true,  true,  true,                                     // 17, 18, 19 (17:00 - 19:59)
        false, false, false, false                              // 20 - 23
    };

    int testHours[] = {7, 8, 9, 10, 16, 17, 19, 20, 21};
    std::cout << "Hour | Logical Expression Peak? | Lookup Table Peak?\n";
    std::cout << "-----|--------------------------|-------------------\n";
    for (int h : testHours) {
        bool logicalPeak = (h >= 8 && h < 10) || (h >= 17 && h < 20);
        bool tablePeak   = PEAK_TABLE[h];
        std::cout << std::setw(4) << h << " | "
                  << std::setw(24) << (logicalPeak ? "PEAK" : "OFF-PEAK") << " | "
                  << (tablePeak ? "PEAK" : "OFF-PEAK") << "\n";
    }

    return 0;
}
