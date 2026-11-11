/*
 * ============================================================================
 *  ASSESSMENT 2 – TASK 05 (Q5)
 *  Static Members and Arrays of Objects                    (7 Marks)
 *  Bloom's Level: K5 – Evaluate
 * ============================================================================
 *
 *  SECTION A – PREDICT THE OUTPUT
 * ──────────────────────────────────────────────────────────────────────────────
 *  Given Program:
 *
 *      #include <iostream>
 *
 *      class Token {
 *          static int next;
 *          int id;
 *
 *      public:
 *          Token() { id = ++next; }
 *          int get() const { return id; }
 *          static int issued() { return next - 100; }
 *      };
 *
 *      int Token::next = 100;
 *
 *      int main()
 *      {
 *          Token counter[3];
 *          Token extra;
 *          std::cout << counter[0].get() << ' ' << counter[2].get() << ' ' << extra.get()
 *                    << ' ' << Token::issued() << '\n';
 *      }
 *
 *  PREDICTED OUTPUT (Written before running):
 *      101 103 104 4
 *
 *  DETAILED LINE-BY-LINE EXPLANATION:
 *
 *  1. Why static counter starts at 100:
 *     - `int Token::next = 100;` defines the static data member outside the class declaration
 *       in static/global storage. It is initialized to 100 before `main()` begins.
 *
 *  2. How many times the constructor ran:
 *     - `Token counter[3];` instantiates an array of 3 Token objects, invoking the default
 *       constructor 3 times:
 *         * counter[0]: ++next becomes 101; id = 101
 *         * counter[1]: ++next becomes 102; id = 102
 *         * counter[2]: ++next becomes 103; id = 103
 *     - `Token extra;` instantiates a 4th Token object:
 *         * extra: ++next becomes 104; id = 104
 *     - In total, the constructor ran 4 times.
 *
 *  3. Output values:
 *     - counter[0].get() returns 101
 *     - counter[2].get() returns 103
 *     - extra.get() returns 104
 *     - Token::issued() computes (next - 100) = (104 - 100) = 4
 *     - Final printed line: "101 103 104 4\n"
 *
 * ============================================================================
 *  SECTION B – STATIC CONCEPTS & RULES
 * ============================================================================
 *  1. Which members must be static:
 *     - Data shared across the entire class (e.g. `issued`, `CAPACITY`) must be static.
 *     - Defined outside the class in file scope to allocate memory once in .bss / .data segment.
 *  2. Why a static member function cannot use `this` or non-static members:
 *     - Static member functions belong to the class, not to any object instance.
 *     - They do not receive an implicit `this` pointer argument.
 *     - Accessing a non-static member requires an object instance's memory address, which
 *       does not exist in a static function context.
 *  3. Array of objects initialization:
 *     - Initialized using braced-initializer list or default constructor.
 *     - Each element's constructor runs in ascending index order (0 to N-1).
 *  4. What happens when tickets are created in a loop or function:
 *     - The static counter persists across loop iterations and across multiple function calls.
 *     - Even when ticket objects are destroyed upon leaving local scope, `issued` stays unchanged.
 *
 * ============================================================================
 *  SECTION C – ALTERNATIVE CONCEPT AND COMPLEXITY ANALYSIS
 * ============================================================================
 *  Comparison: Global Variable vs TicketOffice Object vs Static Member
 *
 *  1. Global variable:
 *     - Zero encapsulation. Any function can corrupt `g_issued = -5;`.
 *     - Cannot handle multiple buses independently.
 *  2. TicketOffice object:
 *     - Full encapsulation + can instantiate BusA and BusB with independent counters!
 *     - Each ticket holds a reference to its TicketOffice or is issued by `office.issueTicket(name)`.
 *  3. Static member (current design):
 *     - Perfectly encapsulated within `BusTicket` class.
 *     - Limitation: Only one bus fleet can be managed per class.
 *
 *  Complexity:
 *     - Time Complexity: O(1) for ticket creation and count check across all designs.
 *     - Space Complexity: O(1) auxiliary space (one counter variable across all tickets).
 * ============================================================================
 */

#include <iostream>
#include <string>

// ── Predict the Output verification class ───────────────────────────────────
class Token {
    static int next;
    int id;

public:
    Token() { id = ++next; }
    int get() const { return id; }
    static int issued() { return next - 100; }
};

int Token::next = 100;

// ── Complete the Code: BusTicket class with static members ───────────────────
class BusTicket {
    static int issued; // shared by all BusTicket objects
    static const int CAPACITY = 3;
    int seat;
    std::string name;

public:
    explicit BusTicket(const std::string& n) : seat(0), name(n)
    {
        // TODO 1: If fewer than CAPACITY tickets have been issued, increase issued and
        // give this ticket the next seat number; otherwise leave seat as 0 (waiting list)
        if (issued < CAPACITY) {
            ++issued;
            seat = issued;
        }
    }

    static int count()
    {
        // TODO 2: Return the number of seats issued so far
        return issued;
    }

    void print() const
    {
        std::cout << name << ": "
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting"))
                  << '\n';
    }

    // Static function to reset for multiple testing scenarios
    static void reset() { issued = 0; }
};

int BusTicket::issued = 0; // definition of the static data member

// Function called twice to verify count persistence
void issueBatch(const std::string names[], int size)
{
    std::cout << "  Booking batch of " << size << " passenger(s):\n";
    for (int i = 0; i < size; ++i) {
        BusTicket t(names[i]);
        std::cout << "    ";
        t.print();
    }
    std::cout << "  Issued so far: " << BusTicket::count() << " / 3\n\n";
}

int main()
{
    std::cout << "==============================================================\n";
    std::cout << "  TASK 05: STATIC MEMBERS AND ARRAYS OF OBJECTS (Q5)          \n";
    std::cout << "==============================================================\n\n";

    // --- PART 1: Predict the Output Verification ---
    std::cout << "--- 1. PREDICT THE OUTPUT VERIFICATION ---\n";
    std::cout << "Predicted Output:\n";
    std::cout << "101 103 104 4\n\n";

    std::cout << "Actual Output:\n";
    Token counter[3];
    Token extra;
    std::cout << counter[0].get() << ' ' << counter[2].get() << ' ' << extra.get()
              << ' ' << Token::issued() << "\n\n";

    // --- PART 2: Complete the Code Verification ---
    std::cout << "--- 2. COMPLETE THE CODE (EXACT PROBLEM SPECIFICATION) ---\n";
    std::cout << "Expected Output:\n";
    std::cout << "Asha: Seat 1\n";
    std::cout << "Bala: Seat 2\n";
    std::cout << "Chitra: Seat 3\n";
    std::cout << "Dev: Waiting\n";
    std::cout << "Issued: 3\n\n";

    std::cout << "Actual Output:\n";
    BusTicket trip[4] = {BusTicket("Asha"), BusTicket("Bala"),
                         BusTicket("Chitra"), BusTicket("Dev")};
    for (const BusTicket& t : trip) t.print();
    std::cout << "Issued: " << BusTicket::count() << "\n\n";

    // --- PART 3: Tickets Created in a Function Called Twice ---
    std::cout << "--- 3. TICKETS CREATED IN A FUNCTION CALLED TWICE ---\n";
    BusTicket::reset(); // reset for demo
    std::string morningBatch[2] = {"Farooq", "Gita"};
    std::string afternoonBatch[3] = {"Hari", "Ishaan", "Jaya"};

    issueBatch(morningBatch, 2);   // Call 1
    issueBatch(afternoonBatch, 3); // Call 2

    std::cout << "Explanation of behaviour:\n";
    std::cout << "  Even though local BusTicket objects were destroyed at the end of Call 1,\n";
    std::cout << "  the static data member `BusTicket::issued` lives in static memory for the\n";
    std::cout << "  entire program execution. Call 2 picks up right where Call 1 left off,\n";
    std::cout << "  allocating the last remaining seat (Seat 3 to Hari) and placing subsequent\n";
    std::cout << "  passengers (Ishaan, Jaya) on the waiting list.\n";

    std::cout << "\n==============================================================\n";
    std::cout << "  ALL TASK 05 CHECKS COMPLETED SUCCESSFULLY                   \n";
    std::cout << "==============================================================\n";

    return 0;
}
