/*
 * ASSESSMENT 2 — TASK 06 (Q6)
 * Friend Functions and Inline Functions                   (8 Marks)
 * -----------------------------------------------------------------
 *
 * Demonstrates:
 *   1. Time class with private h, m
 *   2. Friend functions minutesBetween() and earlier()
 *   3. Inline function explanation and demo
 *   4. Correctness tests across noon, same time, etc.
 *   5. Growth investigation: 10,000,000 calls (O0 vs O2 discussion)
 *   6. Alternative: member function a.minutesTo(b) vs friend vs getters
 *
 * ================================================================
 * COMPLETE THE CODE — Friend Functions
 * ================================================================
 *
 *   int minutesBetween(const Time& a, const Time& b) {
 *       // TODO 1:
 *       int totalA = a.h * 60 + a.m;
 *       int totalB = b.h * 60 + b.m;
 *       return (totalA > totalB) ? (totalA - totalB) : (totalB - totalA);
 *   }
 *   bool earlier(const Time& a, const Time& b) {
 *       // TODO 2:
 *       return (a.h < b.h) || (a.h == b.h && a.m < b.m);
 *   }
 *
 *   Expected output for Time(9,30) and Time(12,55): 205 false
 *   Explanation:
 *     totalA = 9*60 + 30 = 570
 *     totalB = 12*60 + 55 = 775
 *     minutesBetween = 775 - 570 = 205
 *     earlier(12:55, 9:30) → 12 < 9? No. 12==9? No. → false
 *
 * ================================================================
 * WHY FRIEND FUNCTION?
 * ================================================================
 *   minutesBetween(a, b) works on TWO Time objects equally — neither
 *   is the "primary" operand.  Making it a member function of Time
 *   would force `a.minutesBetween(b)`, giving `a` a privileged role
 *   it does not logically have.  A friend function treats both
 *   parameters symmetrically while still accessing private data
 *   (h, m) without public getters.
 *
 *   Same reasoning applies to earlier(a, b): it is a symmetric
 *   comparison, not something "owned by" either Time object.
 *
 * ================================================================
 * INLINE EXPLANATION
 * ================================================================
 *   The `inline` keyword *requests* (not commands) the compiler to
 *   replace the function call with the function body at the call
 *   site.  This eliminates call overhead (push/pop stack frame,
 *   jump) but increases code size if the function is large.
 *
 *   The compiler is free to ignore the request (e.g. for recursive
 *   or very large functions).  Modern compilers with optimisation
 *   enabled (-O2) often inline small functions automatically even
 *   without the keyword, and may refuse to inline functions marked
 *   `inline` if inlining would hurt performance.
 *
 *   `inline` is therefore a *hint*, not a guarantee.
 *
 * ================================================================
 * DEPLOYMENT DECISION — friend vs member vs public getters
 * ================================================================
 *   Friend functions:
 *     Pro  — symmetric interface, accesses private data directly.
 *     Con  — tightly couples the friend to the class internals;
 *            adding a friend requires modifying the class header.
 *
 *   Member functions (a.minutesTo(b)):
 *     Pro  — fully encapsulated, no extra declarations needed.
 *     Con  — asymmetric API; `a.minutesTo(b)` ≠ `b.minutesTo(a)`
 *            semantically looks different (even if result is same).
 *
 *   Public getters (getH(), getM()):
 *     Pro  — no friend or member needed; any free function works.
 *     Con  — exposes internal representation; if Time changes from
 *            (h,m) to total-minutes, every caller must be updated.
 *
 *   Best choice: friend functions for symmetric operations on two
 *   objects, member functions for asymmetric operations on one.
 *
 * ================================================================
 * ALTERNATIVE CONCEPT — a.minutesTo(b) vs friend vs getters
 * ================================================================
 *   a.minutesTo(b) — member function:
 *     Code: return abs((h*60 + m) - (b.h*60 + b.m));
 *     Encapsulation: excellent — no friend, no getter needed.
 *     Symmetry: poor — reads as "a's minutes to b", not
 *               "difference between a and b".
 *
 *   Friend function minutesBetween(a, b):
 *     Encapsulation: good — accesses privates via friendship.
 *     Symmetry: excellent — both parameters equal.
 *
 *   Free function with getters:
 *     Encapsulation: poor — class must expose h and m.
 *     Symmetry: excellent — both parameters equal.
 *
 *   Recommendation: for a symmetric binary operation, friend
 *   function provides the best balance.
 *
 * ================================================================
 * GROWTH INVESTIGATION — 10,000,000 calls with -O0 and -O2
 * ================================================================
 *   With -O0 (no optimisation):
 *     Every call to minutesBetween() goes through full function
 *     call overhead: push arguments, jump, execute, return.
 *     The `inline` keyword is ignored by -O0.
 *     Expected: measurably slower.
 *
 *   With -O2 (optimisation on):
 *     The compiler will likely inline minutesBetween() and earlier()
 *     automatically (they are small), eliminating call overhead.
 *     10,000,000 iterations run significantly faster.
 *     The `inline` keyword is redundant — the compiler decides.
 *
 *   Demonstration below measures the loop but note: -O2 may also
 *   optimise away the loop if the result is unused, so we
 *   accumulate a value and print it to prevent dead-code removal.
 * ================================================================
 */

#include <iostream>
#include <iomanip>
#include <chrono>

// ─────────────────────────────────────────────────────────
//  Time class with private h, m
// ─────────────────────────────────────────────────────────
class Time {
    int h, m;
public:
    Time() : h(0), m(0) {}
    Time(int hours, int minutes) : h(hours), m(minutes) {}

    void show() const {
        std::cout << std::setw(2) << std::setfill('0') << h
                  << ":"
                  << std::setw(2) << std::setfill('0') << m
                  << std::setfill(' ');
    }

    // ---- friend declarations ----
    friend int  minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);

    // ---- member function alternative (for comparison) ----
    int minutesTo(const Time& other) const {
        int totalA = h * 60 + m;
        int totalB = other.h * 60 + other.m;
        return (totalA > totalB) ? (totalA - totalB) : (totalB - totalA);
    }
};

// ─────────────────────────────────────────────────────────
//  Friend function implementations (TODO 1 and TODO 2 completed)
// ─────────────────────────────────────────────────────────

// TODO 1 — completed:
inline int minutesBetween(const Time& a, const Time& b) {
    int totalA = a.h * 60 + a.m;
    int totalB = b.h * 60 + b.m;
    return (totalA > totalB) ? (totalA - totalB) : (totalB - totalA);
}

// TODO 2 — completed:
inline bool earlier(const Time& a, const Time& b) {
    return (a.h < b.h) || (a.h == b.h && a.m < b.m);
}

// ─────────────────────────────────────────────────────────
//  Correctness test helper
// ─────────────────────────────────────────────────────────
void testPair(const Time& a, const Time& b,
              const char* labelA, const char* labelB) {
    std::cout << "  " << labelA << " (";
    a.show();
    std::cout << ") vs " << labelB << " (";
    b.show();
    std::cout << ")  →  diff = " << minutesBetween(a, b)
              << " min, earlier(" << labelA << "," << labelB << ") = "
              << std::boolalpha << earlier(a, b) << "\n";
}

// ─────────────────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────────────────
int main() {
    std::cout << "============================================\n";
    std::cout << "  TASK 06 — Friend & Inline Functions\n";
    std::cout << "============================================\n\n";

    // ---- Part 1: Complete the Code — expected output ----
    std::cout << "--- Part 1: Complete the Code ---\n";
    Time departure(9, 45), arrival(13, 10);
    std::cout << "  Expected: 205 false\n";
    std::cout << "  Actual  : " << minutesBetween(departure, arrival) << ' '
              << std::boolalpha << earlier(arrival, departure) << "\n\n";

    // ---- Part 2: Correctness tests ----
    std::cout << "--- Part 2: Correctness Tests ---\n";

    // Test A: times on either side of noon
    Time morning(11, 45);
    Time afternoon(13, 15);
    testPair(morning, afternoon, "11:45", "13:15");

    // Test B: same time twice
    Time same1(8, 0);
    Time same2(8, 0);
    testPair(same1, same2, "08:00", "08:00");

    // Test C: midnight and late evening
    Time midnight(0, 0);
    Time lateNight(23, 59);
    testPair(midnight, lateNight, "00:00", "23:59");

    // Test D: one minute apart
    Time t3(14, 29);
    Time t4(14, 30);
    testPair(t3, t4, "14:29", "14:30");

    // Test E: reversed arguments should give same minutesBetween
    std::cout << "  minutesBetween(morning, afternoon) = "
              << minutesBetween(morning, afternoon)
              << ", minutesBetween(afternoon, morning) = "
              << minutesBetween(afternoon, morning)
              << " (should be equal)\n\n";

    // ---- Part 3: Member function alternative comparison ----
    std::cout << "--- Part 3: Member Function Alternative ---\n";
    std::cout << "  Friend:  minutesBetween(9:45, 13:10) = "
              << minutesBetween(departure, arrival) << "\n";
    std::cout << "  Member:  departure.minutesTo(arrival) = "
              << departure.minutesTo(arrival) << "\n";
    std::cout << "  Both return the same result, but the friend\n"
              << "  function treats both parameters symmetrically.\n\n";

    // ---- Part 4: Growth Investigation — 10,000,000 calls ----
    std::cout << "--- Part 4: Growth Investigation (10M calls) ---\n";
    {
        const int N = 10'000'000;
        Time a(9, 30), b(12, 55);
        long long accumulator = 0;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; ++i) {
            accumulator += minutesBetween(a, b);
        }
        auto end = std::chrono::high_resolution_clock::now();

        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "  " << N << " calls to minutesBetween(): "
                  << std::fixed << std::setprecision(2) << ms << " ms\n";
        std::cout << "  (accumulator = " << accumulator
                  << " — used to prevent dead-code elimination)\n\n";

        std::cout << "  NOTE: Compile with -O0 vs -O2 to see the difference.\n"
                  << "  With -O0: inline hint ignored, full call overhead.\n"
                  << "  With -O2: compiler likely inlines automatically,\n"
                  << "            significantly reducing elapsed time.\n"
                  << "  The 'inline' keyword is a hint, not a guarantee.\n\n";
    }

    std::cout << "============================================\n";
    std::cout << "  End of Task 06\n";
    std::cout << "============================================\n";

    return 0;
}
