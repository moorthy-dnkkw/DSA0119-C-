/*
 * ASSESSMENT 2 — TASK 04 (Q4)
 * Specifying a Class with an Array Inside It              (6 Marks)
 * -----------------------------------------------------------------
 *
 * Demonstrates:
 *   1. SavingsAccount class with a fixed-size transactions array
 *   2. Member functions defined INSIDE and OUTSIDE the class
 *   3. Handling the 11th transaction — shift approach vs circular buffer
 *   4. Mini-statement showing only the last 10 transactions
 *
 * ================================================================
 * INQUIRY — Why must the transactions array be private?
 * ================================================================
 *   If the transactions array were public, any outside code could
 *   modify, reorder, or corrupt the recorded transactions without
 *   going through deposit()/withdraw().  Keeping it private means
 *   the class alone decides how transactions are stored, how old
 *   entries are evicted, and what the mini-statement shows.
 *   Encapsulation protects the *invariant* that every entry in the
 *   array was produced by a validated deposit or withdrawal.
 *
 * ================================================================
 * INQUIRY — Shift O(k) vs Circular O(1)
 * ================================================================
 *   Shift approach : when the array is full, move elements 1..9
 *     into positions 0..8, then write the new transaction at
 *     position 9.  Cost: O(k) where k = array capacity (here 10).
 *
 *   Circular buffer : maintain a head index.  When the array is
 *     full, overwrite the oldest slot (head) and advance head by 1
 *     (mod capacity).  Cost: O(1) — no element copying needed.
 *
 * ================================================================
 * DESIGN DECISION — Last 50 transactions
 * ================================================================
 *   If the requirement changed from 10 to 50, the circular buffer
 *   changes the least:
 *     - Only the array size constant changes (10 → 50).
 *     - The O(1) insertion remains O(1).
 *   The shift approach would now copy up to 49 elements per
 *   transaction — increasingly wasteful as capacity grows.
 *
 * ================================================================
 *  ALTERNATIVE — std::deque vs circular buffer vs shifting array
 * ================================================================
 *   std::deque  : push_back() + pop_front() when size > N.
 *       Pro  — idiomatic STL, automatic memory management.
 *       Con  — heap allocation, slightly higher constant overhead.
 *
 *   Circular buffer (raw array + head index):
 *       Pro  — O(1) insert, zero heap allocation, minimal memory.
 *       Con  — iteration order requires modular arithmetic.
 *
 *   Shifting array:
 *       Pro  — simplest to understand; iteration is in natural order.
 *       Con  — O(k) per insertion once full; poor scalability.
 *
 *   For a fixed, small capacity (10) and no dynamic resizing,
 *   the circular buffer gives the best balance of speed and
 *   simplicity inside the class.
 * ================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>  // for memmove in shift approach

// ─────────────────────────────────────────────────────────
//  SavingsAccount class — SHIFT approach for overflow
// ─────────────────────────────────────────────────────────
class SavingsAccount {
private:
    std::string accountNumber;
    long long   balancePaise;           // balance in paise (not rupees)
    long long   transactions[10];       // last-10 ledger (paise, +/-)
    int         txnCount;               // how many stored (max 10)

public:
    // ---------- constructor defined INSIDE the class ----------
    SavingsAccount(const std::string& accNo, long long openingPaise)
        : accountNumber(accNo),
          balancePaise(openingPaise),
          txnCount(0)
    {
        for (int i = 0; i < 10; ++i) transactions[i] = 0;
    }

    // ---------- getBalance defined INSIDE ----------
    long long getBalance() const { return balancePaise; }

    // ---------- getAccountNumber defined INSIDE ----------
    std::string getAccountNumber() const { return accountNumber; }

    // deposit  — declared here, defined OUTSIDE using scope resolution
    void deposit(long long amountPaise);

    // withdraw — declared here, defined OUTSIDE
    bool withdraw(long long amountPaise);

    // miniStatement — declared here, defined OUTSIDE
    void miniStatement() const;

private:
    // ---- helper: record a transaction using the SHIFT approach ----
    // Defined INSIDE the class
    void recordTransaction(long long amountPaise) {
        if (txnCount < 10) {
            transactions[txnCount++] = amountPaise;
        } else {
            // SHIFT: move elements 1..9 → 0..8, write at position 9
            std::memmove(transactions, transactions + 1,
                         9 * sizeof(long long));
            transactions[9] = amountPaise;
            // txnCount stays 10
        }
    }
};

// ============================================================
//  Member functions defined OUTSIDE using SavingsAccount::
// ============================================================

void SavingsAccount::deposit(long long amountPaise) {
    if (amountPaise <= 0) {
        std::cout << "  [!] Deposit amount must be positive.\n";
        return;
    }
    balancePaise += amountPaise;
    recordTransaction(amountPaise);        // +ve = deposit
    std::cout << "  Deposited Rs "
              << amountPaise / 100 << "."
              << std::setw(2) << std::setfill('0') << amountPaise % 100
              << std::setfill(' ') << "\n";
}

bool SavingsAccount::withdraw(long long amountPaise) {
    if (amountPaise <= 0) {
        std::cout << "  [!] Withdrawal amount must be positive.\n";
        return false;
    }
    if (amountPaise > balancePaise) {
        std::cout << "  [!] Insufficient balance for Rs "
                  << amountPaise / 100 << "."
                  << std::setw(2) << std::setfill('0') << amountPaise % 100
                  << std::setfill(' ') << "\n";
        return false;
    }
    balancePaise -= amountPaise;
    recordTransaction(-amountPaise);       // -ve = withdrawal
    std::cout << "  Withdrew Rs "
              << amountPaise / 100 << "."
              << std::setw(2) << std::setfill('0') << amountPaise % 100
              << std::setfill(' ') << "\n";
    return true;
}

void SavingsAccount::miniStatement() const {
    std::cout << "\n  ── Mini-Statement for A/c " << accountNumber
              << " ──\n";
    for (int i = 0; i < txnCount; ++i) {
        long long t = transactions[i];
        const char* tag = (t >= 0) ? "  CR" : "  DR";
        long long abs = (t >= 0) ? t : -t;
        std::cout << "   " << std::setw(2) << (i + 1) << ". "
                  << tag << "  Rs "
                  << abs / 100 << "."
                  << std::setw(2) << std::setfill('0') << abs % 100
                  << std::setfill(' ') << "\n";
    }
    std::cout << "  Balance: Rs "
              << balancePaise / 100 << "."
              << std::setw(2) << std::setfill('0') << balancePaise % 100
              << std::setfill(' ') << "\n\n";
}

// ─────────────────────────────────────────────────────────
//  SavingsAccountCB — CIRCULAR BUFFER approach for overflow
// ─────────────────────────────────────────────────────────
class SavingsAccountCB {
private:
    std::string accountNumber;
    long long   balancePaise;
    long long   transactions[10];
    int         head;       // index of the oldest entry
    int         count;      // number of valid entries (max 10)

public:
    SavingsAccountCB(const std::string& accNo, long long openingPaise)
        : accountNumber(accNo),
          balancePaise(openingPaise),
          head(0), count(0)
    {
        for (int i = 0; i < 10; ++i) transactions[i] = 0;
    }

    long long getBalance() const { return balancePaise; }

    // ---- deposit defined INSIDE ----
    void deposit(long long amountPaise) {
        if (amountPaise <= 0) return;
        balancePaise += amountPaise;
        recordTxn(amountPaise);
        std::cout << "  [CB] Deposited Rs "
                  << amountPaise / 100 << "."
                  << std::setw(2) << std::setfill('0') << amountPaise % 100
                  << std::setfill(' ') << "\n";
    }

    // ---- withdraw defined OUTSIDE ----
    bool withdraw(long long amountPaise);

    // ---- miniStatement defined OUTSIDE ----
    void miniStatement() const;

private:
    // Circular buffer insertion — O(1) always
    void recordTxn(long long amountPaise) {
        if (count < 10) {
            transactions[(head + count) % 10] = amountPaise;
            ++count;
        } else {
            // Overwrite the oldest slot, advance head
            transactions[head] = amountPaise;
            head = (head + 1) % 10;
        }
    }
};

bool SavingsAccountCB::withdraw(long long amountPaise) {
    if (amountPaise <= 0) return false;
    if (amountPaise > balancePaise) {
        std::cout << "  [CB] Insufficient balance.\n";
        return false;
    }
    balancePaise -= amountPaise;
    recordTxn(-amountPaise);
    std::cout << "  [CB] Withdrew Rs "
              << amountPaise / 100 << "."
              << std::setw(2) << std::setfill('0') << amountPaise % 100
              << std::setfill(' ') << "\n";
    return true;
}

void SavingsAccountCB::miniStatement() const {
    std::cout << "\n  ── Mini-Statement (Circular Buffer) for A/c "
              << accountNumber << " ──\n";
    for (int i = 0; i < count; ++i) {
        int idx = (head + i) % 10;
        long long t = transactions[idx];
        const char* tag = (t >= 0) ? "  CR" : "  DR";
        long long abs = (t >= 0) ? t : -t;
        std::cout << "   " << std::setw(2) << (i + 1) << ". "
                  << tag << "  Rs "
                  << abs / 100 << "."
                  << std::setw(2) << std::setfill('0') << abs % 100
                  << std::setfill(' ') << "\n";
    }
    std::cout << "  Balance: Rs "
              << balancePaise / 100 << "."
              << std::setw(2) << std::setfill('0') << balancePaise % 100
              << std::setfill(' ') << "\n\n";
}

// ─────────────────────────────────────────────────────────
//  main — demo with >10 transactions
// ─────────────────────────────────────────────────────────
int main() {
    std::cout << "============================================\n";
    std::cout << "  TASK 04 — Class with an Array Inside It\n";
    std::cout << "============================================\n\n";

    // ---------- Part A: Shift Approach ----------
    std::cout << "--- Part A: Shift Approach ---\n";
    SavingsAccount sa("SB-1001", 10000'00);  // Rs 10000.00

    // Perform 12 transactions (more than 10 — triggers shift)
    sa.deposit(500'00);       // 1
    sa.deposit(1500'00);      // 2
    sa.withdraw(200'00);      // 3
    sa.deposit(300'00);       // 4
    sa.withdraw(100'00);      // 5
    sa.deposit(750'00);       // 6
    sa.withdraw(50'00);       // 7
    sa.deposit(1200'00);      // 8
    sa.withdraw(600'00);      // 9
    sa.deposit(400'00);       // 10  (array now full)
    sa.deposit(250'00);       // 11  → shift kicks in
    sa.withdraw(350'00);      // 12  → shift again

    std::cout << "\n  After 12 transactions, mini-statement shows last 10:\n";
    sa.miniStatement();

    // ---------- Part B: Circular Buffer Approach ----------
    std::cout << "--- Part B: Circular Buffer Approach ---\n";
    SavingsAccountCB cb("SB-2002", 10000'00);

    cb.deposit(500'00);
    cb.deposit(1500'00);
    cb.withdraw(200'00);
    cb.deposit(300'00);
    cb.withdraw(100'00);
    cb.deposit(750'00);
    cb.withdraw(50'00);
    cb.deposit(1200'00);
    cb.withdraw(600'00);
    cb.deposit(400'00);
    cb.deposit(250'00);       // 11th — circular overwrite
    cb.withdraw(350'00);      // 12th

    std::cout << "\n  After 12 transactions, mini-statement shows last 10:\n";
    cb.miniStatement();

    // ---------- Comparison ----------
    std::cout << "--- Comparison: Shift vs Circular Buffer ---\n";
    std::cout << "  Shift approach:\n"
              << "    - O(k) per insert when full (copies k-1 elements)\n"
              << "    - Natural iteration order (index 0 = oldest)\n"
              << "    - Simpler to understand\n\n";
    std::cout << "  Circular buffer approach:\n"
              << "    - O(1) per insert always (overwrite + advance head)\n"
              << "    - Iteration uses modular arithmetic\n"
              << "    - Scales better as capacity grows\n\n";

    std::cout << "  Both produce the same last-10 transaction list.\n";
    std::cout << "  For 50 transactions, only the constant (10 → 50) changes\n"
              << "  in the circular buffer; the shift approach copies up to\n"
              << "  49 elements per transaction — increasingly wasteful.\n\n";

    std::cout << "============================================\n";
    std::cout << "  End of Task 04\n";
    std::cout << "============================================\n";

    return 0;
}
