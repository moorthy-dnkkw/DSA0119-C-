/*
 * ============================================================================
 *  ASSESSMENT 2 – TASK 01 (Q1)
 *  Call by Value, Call by Reference, and Call by Pointer        (6 Marks)
 *  Bloom's Level: K5 – Evaluate
 * ============================================================================
 *
 *  SECTION A – PREDICT THE OUTPUT
 * ──────────────────────────────────────────────────────────────────────────────
 *  Given Program:
 *
 *      #include <iostream>
 *
 *      void change(int a, int& b, int* c)
 *      {
 *          a += 10;
 *          b += 10;
 *          *c += 10;
 *      }
 *
 *      int& bigger(int& p, int& q) { return p > q ? p : q; }
 *
 *      void show(int x, int y = 5, int z = 10) { std::cout << x + y + z << ' '; }
 *
 *      int main()
 *      {
 *          int a = 1, b = 2, c = 3;
 *          change(a, b, &c);
 *          std::cout << a << ' ' << b << ' ' << c << '\n';
 *
 *          bigger(a, c) = 100;
 *          std::cout << a << ' ' << c << '\n';
 *
 *          show(1);
 *          show(1, 2);
 *          show(1, 2, 3);
 *          std::cout << '\n';
 *      }
 *
 *  PREDICTED OUTPUT (Written before running):
 *      1 12 13
 *      1 100
 *      16 13 6 
 *
 *  DETAILED LINE-BY-LINE EXPLANATION OF PREDICTION:
 *
 *  1. change(a, b, &c);
 *     - Parameter `a` (int) is passed by value: a copy of `a` is created on the stack frame of change().
 *       Inside change(), local `a` becomes 1 + 10 = 11. When change() returns, local `a` is destroyed.
 *       The caller's variable `a` remains 1.
 *     - Parameter `b` (int&) is passed by reference: `b` is an alias / reference to the caller's `b`.
 *       b += 10 directly mutates caller's `b` from 2 to 12.
 *     - Parameter `c` (int*) is passed by pointer: `c` holds the memory address of caller's `c`.
 *       *c += 10 dereferences the pointer and modifies caller's `c` from 3 to 13.
 *     - Output: "1 12 13\n"
 *
 *  2. bigger(a, c) = 100;
 *     - Here, a = 1, c = 13.
 *     - `bigger(int& p, int& q)` returns an lvalue reference `int&`.
 *     - The ternary condition `p > q` evaluates `1 > 13`, which is false.
 *     - Hence, `bigger` returns a reference to `q` (which is caller's `c`).
 *     - The assignment `bigger(a, c) = 100;` assigns 100 to the returned reference (`c`).
 *     - Caller's `c` becomes 100; `a` remains 1.
 *     - Output: "1 100\n"
 *
 *  3. show(1); show(1, 2); show(1, 2, 3);
 *     - Prototype: `void show(int x, int y = 5, int z = 10)`
 *     - `show(1)`: x = 1, y uses default 5, z uses default 10 → 1 + 5 + 10 = 16. Prints "16 ".
 *     - `show(1, 2)`: x = 1, y = 2, z uses default 10 → 1 + 2 + 10 = 13. Prints "13 ".
 *     - `show(1, 2, 3)`: x = 1, y = 2, z = 3 (no defaults used) → 1 + 2 + 3 = 6. Prints "6 ".
 *     - Output: "16 13 6 \n"
 *
 * ============================================================================
 *  SECTION B – INQUIRY: Storage & Parameter Passing Mechanics
 * ============================================================================
 *  1. Where each version stores its parameter:
 *     - Call by value: A new object of the parameter type is allocated on the function's
 *       activation record (stack frame) and initialized via copy constructor / bitwise copy.
 *     - Call by reference: Under the hood (in assembly), the compiler typically passes an
 *       address (pointer) on the stack or in a machine register (e.g., %rcx or %rdi). Syntactically,
 *       the parameter is an alias with no independent memory address.
 *     - Call by pointer: An explicit pointer variable (8 bytes on 64-bit platforms) is allocated
 *       on the stack frame holding the caller's memory address.
 *
 *  2. Why only reference and pointer change caller's fee:
 *     - Call by value mutates only the stack-local copy; on return, the stack unwinds and the
 *       modification is lost.
 *     - Call by reference and pointer both operate on the caller's memory location directly.
 *
 *  3. When to use `const Type&`:
 *     - Use `const Type&` when passing large read-only objects (e.g. std::string, std::vector,
 *       large structs). It avoids the expensive copy cost of call-by-value while providing
 *       a compile-time guarantee that the function will not mutate the caller's object.
 *
 * ============================================================================
 *  SECTION C – DESIGN DECISION: In-place Mutation vs Return New Fee
 * ============================================================================
 *  Problem Context: Multiple concessions may apply to one student (e.g. Merit 10%, Sports 15%).
 *  Decision: Return the new fee (or return an updated object) rather than in-place mutation.
 *
 *  Defence:
 *    1. Immutability & Traceability: Preserves auditability. The original base fee is retained
 *       for invoicing records, audit logging, and fee rollback.
 *    2. Function Chaining & Composability:
 *          long long finalFee = applyConcession(applyConcession(baseFee, 10), 15);
 *       Pure functions without side effects compose cleanly.
 *    3. Thread Safety: Eliminates data races when multiple threads inspect a student's record.
 *
 * ============================================================================
 *  SECTION D – ALTERNATIVE CONCEPT AND COMPLEXITY ANALYSIS
 * ============================================================================
 *  Alternative: `long long applyConcession(long long fee, int percent)` (return new value)
 *
 *  Complexity Analysis:
 *    - Time Complexity: O(1) for all three passing mechanisms (arithmetic: subtract, multiply, divide).
 *    - Space Complexity:
 *      * For `long long` (8 bytes):
 *        - By value: copies 8 bytes onto stack (O(1)).
 *        - By reference / pointer: passes 8-byte pointer (O(1)).
 *        - Overhead difference is 0 bytes!
 *      * For a large `Student` object (e.g. 256 bytes with name, course, history):
 *        - By value: copies 256 bytes + heap allocations for dynamic members (O(S) space, O(S) time).
 *        - By reference (`const Student&`): passes 8-byte address (O(1) space, O(1) time).
 *        - Space copied: 256 bytes vs 8 bytes (32x savings!).
 * ============================================================================
 */

#include <iostream>
#include <iomanip>

// ── Predict-the-output functions ────────────────────────────────────────────
void change(int a, int& b, int* c)
{
    a += 10;
    b += 10;
    *c += 10;
}

int& bigger(int& p, int& q) { return p > q ? p : q; }

void show(int x, int y = 5, int z = 10) { std::cout << x + y + z << ' '; }

// ── Complete the Code: Three Concession Passing Styles ───────────────────────

// 1. Call by Value: modifies only local copy; caller's variable remains unchanged
void applyByValue(long long fee, int percent)
{
    fee -= fee * percent / 100; // changes only the local copy
}

// 2. Call by Reference: modifies caller's variable directly (TODO 1)
void applyConcession(long long& fee, int percent)
{
    // TODO 1: Reduce the caller's fee by percent using integer arithmetic (call by reference)
    fee -= fee * percent / 100;
}

// 3. Call by Pointer: modifies caller's variable via pointer dereference
void applyByPointer(long long* fee, int percent)
{
    if (fee != nullptr) {
        *fee -= *fee * percent / 100;
    }
}

// 4. Functional Style: returns new discounted fee without mutating caller's variable
long long applyConcessionFunctional(long long fee, int percent)
{
    return fee - (fee * percent / 100);
}

int main()
{
    std::cout << "==============================================================\n";
    std::cout << "  TASK 01: CALL BY VALUE, REFERENCE AND POINTER (Q1)          \n";
    std::cout << "==============================================================\n\n";

    // --- PART 1: Predict-the-Output Verification ---
    std::cout << "--- 1. PREDICT THE OUTPUT VERIFICATION ---\n";
    std::cout << "Predicted Output:\n";
    std::cout << "1 12 13\n";
    std::cout << "1 100\n";
    std::cout << "16 13 6 \n\n";

    std::cout << "Actual Output:\n";
    int a = 1, b = 2, c = 3;
    change(a, b, &c);
    std::cout << a << ' ' << b << ' ' << c << '\n';

    bigger(a, c) = 100;
    std::cout << a << ' ' << c << '\n';

    show(1);
    show(1, 2);
    show(1, 2, 3);
    std::cout << "\n\n";

    // --- PART 2: Complete the Code Verification ---
    std::cout << "--- 2. COMPLETE THE CODE: HOSTEL FEE CONCESSION ---\n";
    long long hostelFee = 45000;
    applyByValue(hostelFee, 10);
    std::cout << "after call by value : " << hostelFee << '\n';
    applyConcession(hostelFee, 10);
    std::cout << "after call by reference: " << hostelFee << '\n';

    // --- PART 3: Three Implementations Demonstration ---
    std::cout << "\n--- 3. THREE IMPLEMENTATIONS COMPARISON ---\n";
    long long feeVal = 45000;
    long long feeRef = 45000;
    long long feePtr = 45000;

    applyByValue(feeVal, 10);
    applyConcession(feeRef, 10);
    applyByPointer(&feePtr, 10);

    std::cout << "Initial Fee: 45000 paise (Rs 450.00)\n";
    std::cout << "After applyByValue(fee, 10)    : " << feeVal << " paise (Original unchanged)\n";
    std::cout << "After applyConcession(fee, 10)  : " << feeRef << " paise (Modified in-place)\n";
    std::cout << "After applyByPointer(&fee, 10) : " << feePtr << " paise (Modified in-place)\n";

    // --- PART 4: Functional Alternative (Chaining multiple concessions) ---
    std::cout << "\n--- 4. FUNCTIONAL ALTERNATIVE (CHAINING CONCESSIONS) ---\n";
    long long baseFee = 45000;
    // Apply 10% Merit Concession, then 15% Sports Concession on remaining fee
    long long afterMerit = applyConcessionFunctional(baseFee, 10);
    long long finalFee = applyConcessionFunctional(afterMerit, 15);
    std::cout << "Base Fee: " << baseFee << " paise\n";
    std::cout << "After 10% Merit Concession : " << afterMerit << " paise\n";
    std::cout << "After 15% Sports Concession: " << finalFee << " paise\n";

    std::cout << "\n==============================================================\n";
    std::cout << "  ALL TASK 01 CHECKS COMPLETED SUCCESSFULLY                   \n";
    std::cout << "==============================================================\n";

    return 0;
}
