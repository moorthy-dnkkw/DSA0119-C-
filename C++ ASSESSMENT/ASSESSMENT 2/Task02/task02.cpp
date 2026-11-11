/*
 * ============================================================================
 *  ASSESSMENT 2 – TASK 02 (Q2)
 *  Return by Reference and Its Dangers                         (4 Marks)
 * ============================================================================
 *
 *  SECTION A – Return by Reference: Highest Mark in Array
 *  SECTION B – Dangerous Function: Returning Reference to Local Variable
 *  SECTION C – Using Return-by-Reference on LHS of = (Moderation)
 *  SECTION D – Explanation: Dangling Reference, const& vs copy
 *  SECTION E – Changed Condition: std::vector Growing Invalidates References
 *  SECTION F – Alternative: Returning Index vs Pointer vs Reference
 *
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <climits>

/*
 * ============================================================================
 *  SECTION A – Return reference to highest mark in a fixed-size array
 * ============================================================================
 *  The function returns a NON-CONST reference, so the caller can both
 *  READ the highest mark and WRITE through it (e.g. for moderation).
 */
int& highestMark(int marks[], int size) {
    int maxIdx = 0;
    for (int i = 1; i < size; ++i) {
        if (marks[i] > marks[maxIdx]) {
            maxIdx = i;
        }
    }
    return marks[maxIdx];   // safe: array outlives the function
}

/*
 * ============================================================================
 *  SECTION B – DANGEROUS: Returning reference to a local variable
 * ============================================================================
 *  This function compiles but produces a WARNING:
 *      "warning: reference to local variable 'local' returned"
 *  The returned reference is DANGLING – the local variable is destroyed
 *  when the function returns, so any use of the reference is undefined
 *  behaviour.
 *
 *  NOTE: This is intentionally included to demonstrate the danger.
 *        GCC will emit a warning (-Wall catches it).
 */
int& dangerousLocal() {
    int local = 42;
    return local;       // WARNING: dangling reference!
}

/*
 * ============================================================================
 *  SECTION D – EXPLANATION (in comments)
 * ============================================================================
 *
 *  1. DANGLING REFERENCE
 *     A dangling reference refers to memory that has been deallocated (e.g.
 *     a local variable after the function returns, or a vector element after
 *     reallocation).  Accessing it is undefined behaviour – the program may
 *     crash, return garbage, or appear to work but corrupt other data.
 *
 *  2. const REFERENCE vs COPY
 *
 *     | Aspect            | const int&              | int (copy)            |
 *     |--------------------|-------------------------|-----------------------|
 *     | Memory             | No copy; aliases orig.  | Copies the value      |
 *     | Can modify orig?   | No (const)              | No (independent copy) |
 *     | Lifetime danger    | Dangling if orig dies   | Safe (owns its copy)  |
 *     | Use case           | Read large objects      | Small/primitive types  |
 *
 *     For primitives (int, long long) the cost of copying is negligible, so
 *     returning by value is usually preferred for safety.  For large objects
 *     (std::string, Student), const& avoids the copy but the caller must
 *     ensure the referred-to object outlives the reference.
 *
 * ============================================================================
 *  SECTION E – CHANGED CONDITION: std::vector growing invalidates references
 * ============================================================================
 *
 *  When a std::vector exceeds its capacity, it allocates a new, larger block
 *  and MOVES all elements there.  Any reference (or pointer / iterator)
 *  obtained before the reallocation now points to freed memory – a dangling
 *  reference, just like returning a reference to a local.
 *
 *  Example:
 *      std::vector<int> v = {10, 20, 30};
 *      int& ref = v[0];       // ref → first element
 *      v.push_back(40);       // may reallocate → ref is now DANGLING
 *      std::cout << ref;      // UNDEFINED BEHAVIOUR
 *
 *  Mitigation strategies:
 *    a) reserve() enough capacity up front so push_back never reallocates.
 *    b) Use indices instead of references – indices survive reallocation.
 *    c) Re-obtain the reference after any operation that might grow the
 *       vector.
 *
 * ============================================================================
 *  SECTION F – ALTERNATIVE: Returning Index vs Pointer vs Reference
 * ============================================================================
 *
 *  | Approach       | Pros                                 | Cons                                    |
 *  |----------------|--------------------------------------|-----------------------------------------|
 *  | Return index   | Survives reallocation; simple int    | Requires container + index to access    |
 *  | Return pointer | Can be nullptr (no valid element);   | Raw pointer semantics; can be dangling  |
 *  |                | works with C APIs                    |                                         |
 *  | Return ref     | Natural syntax (ref = 100); no null  | Dangling if container reallocates or    |
 *  |                |                                      | element is removed                      |
 *
 *  Recommendation: For containers that may grow, return an INDEX.
 *                  For fixed-size arrays, return a REFERENCE (clean syntax).
 *                  Use POINTERS when nullable semantics are needed (e.g.
 *                  "element not found" → return nullptr).
 *
 * ============================================================================
 */

int main() {
    // ─────────────────────────────────────────────────────────────────────
    //  SECTION A: Return reference to highest mark
    // ─────────────────────────────────────────────────────────────────────
    std::cout << "=== SECTION A: Return Reference to Highest Mark ===\n\n";

    int marks[] = {72, 85, 63, 91, 78};
    int n = sizeof(marks) / sizeof(marks[0]);

    std::cout << "Original marks: ";
    for (int i = 0; i < n; ++i) std::cout << marks[i] << ' ';
    std::cout << '\n';

    int& highest = highestMark(marks, n);
    std::cout << "Highest mark  : " << highest << '\n';

    // ─────────────────────────────────────────────────────────────────────
    //  SECTION C: Using return-by-reference on LHS of = (Moderation)
    //  Scenario: Exam board moderates the highest mark by adding 2
    // ─────────────────────────────────────────────────────────────────────
    std::cout << "\n=== SECTION C: Moderation via Return-by-Reference ===\n\n";

    std::cout << "Moderating: highestMark(marks, n) += 2\n";
    highestMark(marks, n) += 2;     // modifies marks[3] directly

    std::cout << "Marks after moderation: ";
    for (int i = 0; i < n; ++i) std::cout << marks[i] << ' ';
    std::cout << '\n';
    std::cout << "Highest is now: " << highestMark(marks, n) << '\n';

    // ─────────────────────────────────────────────────────────────────────
    //  SECTION B: Dangerous – returning reference to local variable
    // ─────────────────────────────────────────────────────────────────────
    std::cout << "\n=== SECTION B: Dangerous Local Reference Demonstration ===\n\n";

    std::cout << "Function definition:\n";
    std::cout << "  int& dangerousLocal() {\n";
    std::cout << "      int local = 42;\n";
    std::cout << "      return local;  // WARNING: reference to local variable returned\n";
    std::cout << "  }\n\n";
    std::cout << "Compiler Diagnostic from GCC (MinGW-W64 8.1.0):\n";
    std::cout << "  warning: reference to local variable 'local' returned [-Wreturn-local-addr]\n\n";
    std::cout << "Why using it is Undefined Behaviour:\n";
    std::cout << "  1. The variable 'local' is allocated on the activation record (stack frame)\n";
    std::cout << "     of dangerousLocal().\n";
    std::cout << "  2. When the function returns, its stack frame is popped/deallocated.\n";
    std::cout << "  3. The returned reference points to memory that is no longer reserved for that object.\n";
    std::cout << "  4. Any subsequent function call, interrupt, or stack manipulation overwrites this memory.\n";
    std::cout << "  5. Dereferencing this dangling reference causes memory corruption, garbage reads, or SIGSEGV.\n";

    // ─────────────────────────────────────────────────────────────────────
    //  SECTION E: std::vector growing invalidates references
    // ─────────────────────────────────────────────────────────────────────
    std::cout << "\n=== SECTION E: Vector Growth Invalidating References ===\n\n";

    std::vector<int> v = {10, 20, 30};
    v.shrink_to_fit();  // ensure capacity == size for demo

    std::cout << "Vector: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << "\nCapacity before push: " << v.capacity() << '\n';

    int* ptrBefore = &v[0];
    std::cout << "Address of v[0] before push_back: " << ptrBefore << '\n';

    // Force reallocation by pushing beyond capacity
    for (int i = 0; i < 100; ++i) v.push_back(i);

    int* ptrAfter = &v[0];
    std::cout << "Address of v[0] after  push_back: " << ptrAfter << '\n';

    if (ptrBefore != ptrAfter) {
        std::cout << "  -> Addresses DIFFER: reallocation occurred!\n";
        std::cout << "  -> Any reference/pointer obtained before push_back is DANGLING.\n";
    } else {
        std::cout << "  -> Addresses same (no reallocation this time, but NOT guaranteed).\n";
    }

    // Safe approach: use indices
    std::cout << "\nSafe approach using INDEX instead of reference:\n";
    std::vector<int> scores = {55, 88, 72};
    int bestIdx = 0;
    for (int i = 1; i < static_cast<int>(scores.size()); ++i) {
        if (scores[i] > scores[bestIdx]) bestIdx = i;
    }
    std::cout << "  Best score at index " << bestIdx << " = " << scores[bestIdx] << '\n';
    scores.push_back(99);  // may reallocate
    std::cout << "  After push_back(99), index " << bestIdx
              << " still valid = " << scores[bestIdx] << '\n';

    // ─────────────────────────────────────────────────────────────────────
    //  SECTIONS D & F: See detailed comments in source above
    // ─────────────────────────────────────────────────────────────────────
    std::cout << "\n=== SECTIONS D & F: See source-code comments for ===\n";
    std::cout << "    Dangling Ref explanation, const& vs copy,\n";
    std::cout << "    and Index vs Pointer vs Reference comparison.\n";

    return 0;
}
