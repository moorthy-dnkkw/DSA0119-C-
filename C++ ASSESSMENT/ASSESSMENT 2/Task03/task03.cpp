/*
 * ============================================================================
 *  ASSESSMENT 2 – TASK 03 (Q3)
 *  Default Arguments and Function Overloading                  (6 Marks)
 * ============================================================================
 *
 *  SECTION A – Ambiguity Demonstration
 *  SECTION B – Redesigned Overloaded Functions
 *  SECTION C – Predict Which Function Is Called (6 Calls)
 *  SECTION D – Overload Resolution Rules
 *  SECTION E – Why Default Arguments Must Be Given from the Right
 *  SECTION F – Alternative: Single Parcel Struct vs Overloaded Functions
 *
 * ============================================================================
 *  SECTION A – AMBIGUITY DEMONSTRATION
 * ============================================================================
 *
 *  Consider these three declarations:
 *
 *      long long charge(double weightKg);                                  // (1)
 *      long long charge(double weightKg, bool express);                    // (2)
 *      long long charge(double weightKg, bool express = false,
 *                        bool insured = false);                            // (3)
 *
 *  Problem 1 – Signatures (2) and (3):
 *    charge(2.5, true) matches BOTH (2) and (3) exactly, because (3) with
 *    its default for `insured` has the same effective signature as (2).
 *    The compiler cannot decide which to call → AMBIGUOUS.
 *
 *  Problem 2 – Signatures (1) and (3):
 *    charge(2.5) matches (1) exactly AND matches (3) when both defaults
 *    are used.  Again → AMBIGUOUS.
 *
 *  Rule: Default arguments must NOT produce a parameter list that overlaps
 *  with another overload's parameter list.
 *
 *  The ambiguous code is commented out below so the program compiles:
 *
 *      // long long chargeAmbig(double w);
 *      // long long chargeAmbig(double w, bool express);
 *      // long long chargeAmbig(double w, bool express = false,
 *      //                        bool insured = false);
 *      //
 *      // chargeAmbig(2.5);        // ERROR: ambiguous (1) vs (3)
 *      // chargeAmbig(2.5, true);  // ERROR: ambiguous (2) vs (3)
 *
 * ============================================================================
 *  SECTION D – OVERLOAD RESOLUTION RULES
 * ============================================================================
 *
 *  The compiler ranks candidate functions by how well each argument matches:
 *
 *    1. EXACT MATCH        – no conversion needed, or only trivial
 *                            (e.g. array-to-pointer, top-level const)
 *    2. PROMOTION           – widening within a "family":
 *                              int → long, float → double, bool → int
 *    3. STANDARD CONVERSION – narrowing or cross-family:
 *                              int → double, double → int, int → bool
 *    4. USER-DEFINED CONV.  – constructors or conversion operators
 *
 *  If two candidates are equally good on every argument → AMBIGUOUS.
 *
 *  Examples with our redesigned overloads:
 *
 *    charge(500)        → charge(int)    EXACT (int literal → int param)
 *    charge(2.5)        → charge(double) EXACT (double literal → double param)
 *    charge(2.0f)       → charge(double) PROMOTION (float → double beats
 *                                         float → int which is a conversion)
 *    charge(2)          → charge(int)    EXACT (int literal → int param)
 *                         charge(double) would need int→double CONVERSION
 *                         ∴ charge(int) wins by exact match
 *    charge(Parcel{…})  → charge(const Parcel&) EXACT
 *    charge(2, true)    → charge(int, bool, bool) with default for insured
 *
 * ============================================================================
 *  SECTION E – WHY DEFAULT ARGUMENTS MUST BE GIVEN FROM THE RIGHT
 * ============================================================================
 *
 *  C++ requires that default arguments appear on the RIGHTMOST parameters
 *  and proceed leftward without gaps.
 *
 *  Reason: Arguments are matched LEFT to RIGHT.  If a middle parameter
 *  had a default but a later one did not, there would be no way for the
 *  caller to skip the middle argument and supply the later one:
 *
 *      void f(int a, int b = 5, int c);   // ILLEGAL
 *      f(1, , 3);                          // C++ has no "skip" syntax
 *
 *  By enforcing right-to-left defaults, the compiler can unambiguously
 *  fill in omitted trailing arguments:
 *
 *      void f(int a, int b = 5, int c = 10);  // OK
 *      f(1);        // a=1, b=5, c=10
 *      f(1, 2);     // a=1, b=2, c=10
 *      f(1, 2, 3);  // a=1, b=2, c=3
 *
 * ============================================================================
 *  SECTION F – ALTERNATIVE: Single Parcel Struct vs Overloaded Functions
 * ============================================================================
 *
 *  Approach 1 – Overloaded functions:
 *    charge(500);                      // grams
 *    charge(2.5);                      // kg
 *    charge(myParcel, true, false);    // Parcel + options
 *
 *    Pros: Clean call syntax; compiler checks types at each call site.
 *    Cons: Adding a new field (e.g. "fragile") means adding or modifying
 *          overloads; combinatorial explosion of flag parameters.
 *
 *  Approach 2 – Single Parcel struct with optional fields:
 *    Parcel p;
 *    p.weightGrams = 500;
 *    p.express  = true;
 *    p.insured  = false;
 *    charge(p);
 *
 *    Pros: Single function signature; new fields don't break existing
 *          callers (old code just leaves the new field at its default).
 *    Cons: Caller must construct the struct – slightly more verbose for
 *          simple cases; no compiler enforcement that exactly one of
 *          grams/kg is set.
 *
 *  Recommendation:
 *    For ≤ 3 parameters and stable interface → overloading is cleaner.
 *    For ≥ 4 parameters or evolving interface → the struct approach scales
 *    better and is more maintainable.
 *
 * ============================================================================
 */

#include <iostream>
#include <string>

// ── Domain: all money in long long paise ──────────────────────────────────

// ── Parcel struct for the object overload ─────────────────────────────────
struct Parcel {
    std::string description;
    int    weightGrams;        // weight in grams
    bool   express;            // express delivery?
    bool   insured;            // insurance requested?

    Parcel(const std::string& desc, int wg,
           bool exp = false, bool ins = false)
        : description(desc), weightGrams(wg), express(exp), insured(ins) {}
};

/*
 * ============================================================================
 *  SECTION B – REDESIGNED OVERLOADED FUNCTIONS
 * ============================================================================
 *  Three overloads distinguished by the FIRST parameter's type:
 *    1. charge(int grams, ...)        – weight in grams
 *    2. charge(double kg, ...)        – weight in kilograms
 *    3. charge(const Parcel&, ...)    – full Parcel object
 *  Each also accepts default-argument flags for express and insured.
 *
 *  Base rate: 50 paise per gram.
 *  Express surcharge: +50% of base.
 *  Insurance surcharge: +20% of base.
 * ============================================================================
 */

// Base rate: 50 paise per gram
constexpr long long RATE_PER_GRAM = 50;  // paise

// Overload 1: weight in grams (int)
long long charge(int grams, bool express = false, bool insured = false) {
    std::cout << "  [called] charge(int grams=" << grams
              << ", express=" << express << ", insured=" << insured << ")\n";
    long long base = static_cast<long long>(grams) * RATE_PER_GRAM;
    if (express) base += base * 50 / 100;   // +50%
    if (insured) base += base * 20 / 100;   // +20% of current
    return base;
}

// Overload 2: weight in kilograms (double)
long long charge(double kg, bool express = false, bool insured = false) {
    std::cout << "  [called] charge(double kg=" << kg
              << ", express=" << express << ", insured=" << insured << ")\n";
    int grams = static_cast<int>(kg * 1000);
    long long base = static_cast<long long>(grams) * RATE_PER_GRAM;
    if (express) base += base * 50 / 100;
    if (insured) base += base * 20 / 100;
    return base;
}

// Overload 3: Parcel object (const reference)
long long charge(const Parcel& p, bool express, bool insured) {
    std::cout << "  [called] charge(Parcel \"" << p.description
              << "\", express=" << express << ", insured=" << insured << ")\n";
    // Use the Parcel's own flags OR'd with the explicit parameters
    bool isExpress = p.express || express;
    bool isInsured = p.insured || insured;
    long long base = static_cast<long long>(p.weightGrams) * RATE_PER_GRAM;
    if (isExpress) base += base * 50 / 100;
    if (isInsured) base += base * 20 / 100;
    return base;
}

/*
 * ============================================================================
 *  SECTION C – PREDICT WHICH FUNCTION IS CALLED
 * ============================================================================
 *
 *  Call                       | Selected Overload      | Reason
 *  ---------------------------|------------------------|---------------------------
 *  charge(500)                | charge(int, ...)       | 500 is int → exact match
 *  charge(2.5)                | charge(double, ...)    | 2.5 is double → exact match
 *  charge(2.0f)               | charge(double, ...)    | 2.0f is float → PROMOTED to double
 *                             |                        |   (float→double is promotion,
 *                             |                        |    float→int is conversion;
 *                             |                        |    promotion wins)
 *  charge(2)                  | charge(int, ...)       | 2 is int → exact match
 *                             |                        |   (int→double would be conversion,
 *                             |                        |    exact match wins)
 *  charge(p, true, false)     | charge(const Parcel&,  | Parcel object → exact match
 *                             |   bool, bool)          |
 *  charge(500, true)          | charge(int, bool, bool)| 500 is int → exact; insured
 *                             |                        |   defaults to false
 *
 * ============================================================================
 */

int main() {
    std::cout << "=== SECTION A: Ambiguity Demonstration ===\n\n";
    std::cout << "The following overloads would cause AMBIGUITY (commented out):\n\n";
    std::cout << "  long long chargeAmbig(double w);                        // (1)\n";
    std::cout << "  long long chargeAmbig(double w, bool express);          // (2)\n";
    std::cout << "  long long chargeAmbig(double w, bool express = false,\n";
    std::cout << "                         bool insured = false);           // (3)\n\n";
    std::cout << "  chargeAmbig(2.5)      -> ambiguous: (1) vs (3) with both defaults\n";
    std::cout << "  chargeAmbig(2.5, true) -> ambiguous: (2) vs (3) with one default\n";
    std::cout << "  Compiler error: 'call of overloaded function is ambiguous'\n\n";

    std::cout << "=== SECTION B & C: Redesigned Overloads & Predictions ===\n\n";

    long long cost;

    // Call 1: charge(500) → charge(int) exact match
    std::cout << "Call 1: charge(500)\n";
    std::cout << "  Prediction: charge(int) — 500 is int, exact match\n";
    cost = charge(500);
    std::cout << "  Cost: " << cost << " paise (Rs "
              << cost / 100 << "." << (cost % 100 < 10 ? "0" : "") << cost % 100 << ")\n\n";

    // Call 2: charge(2.5) → charge(double) exact match
    std::cout << "Call 2: charge(2.5)\n";
    std::cout << "  Prediction: charge(double) — 2.5 is double, exact match\n";
    cost = charge(2.5);
    std::cout << "  Cost: " << cost << " paise (Rs "
              << cost / 100 << "." << (cost % 100 < 10 ? "0" : "") << cost % 100 << ")\n\n";

    // Call 3: charge(2.0f) → charge(double) by promotion (float → double)
    std::cout << "Call 3: charge(2.0f)\n";
    std::cout << "  Prediction: charge(double) — float promoted to double\n";
    cost = charge(2.0f);
    std::cout << "  Cost: " << cost << " paise (Rs "
              << cost / 100 << "." << (cost % 100 < 10 ? "0" : "") << cost % 100 << ")\n\n";

    // Call 4: charge(2) → charge(int) exact match
    std::cout << "Call 4: charge(2)\n";
    std::cout << "  Prediction: charge(int) — 2 is int, exact match\n";
    cost = charge(2);
    std::cout << "  Cost: " << cost << " paise (Rs "
              << cost / 100 << "." << (cost % 100 < 10 ? "0" : "") << cost % 100 << ")\n\n";

    // Call 5: charge(Parcel, true, false) → charge(const Parcel&, bool, bool)
    std::cout << "Call 5: charge(Parcel{...}, true, false)\n";
    std::cout << "  Prediction: charge(const Parcel&, bool, bool) — exact match\n";
    Parcel p("Textbooks", 1500, false, false);
    cost = charge(p, true, false);
    std::cout << "  Cost: " << cost << " paise (Rs "
              << cost / 100 << "." << (cost % 100 < 10 ? "0" : "") << cost % 100 << ")\n\n";

    // Call 6: charge(500, true) → charge(int, bool, bool) with insured defaulting
    std::cout << "Call 6: charge(500, true)\n";
    std::cout << "  Prediction: charge(int, bool, bool) — insured defaults to false\n";
    cost = charge(500, true);
    std::cout << "  Cost: " << cost << " paise (Rs "
              << cost / 100 << "." << (cost % 100 < 10 ? "0" : "") << cost % 100 << ")\n\n";

    // ─────────────────────────────────────────────────────────────────────
    std::cout << "=== SECTION D: Overload Resolution Rules ===\n";
    std::cout << "  Priority: exact match > promotion > standard conversion\n";
    std::cout << "  See source-code comments for detailed explanation.\n\n";

    std::cout << "=== SECTION E: Default Arguments Right-to-Left Rule ===\n";
    std::cout << "  Defaults must be on RIGHTMOST params, proceeding leftward.\n";
    std::cout << "  C++ has no syntax to 'skip' a middle argument.\n";
    std::cout << "  See source-code comments for detailed explanation.\n\n";

    std::cout << "=== SECTION F: Parcel Struct vs Overloaded Functions ===\n";
    std::cout << "  <= 3 params & stable interface  -> overloading is cleaner.\n";
    std::cout << "  >= 4 params & evolving interface -> struct scales better.\n";
    std::cout << "  See source-code comments for detailed comparison.\n";

    return 0;
}
