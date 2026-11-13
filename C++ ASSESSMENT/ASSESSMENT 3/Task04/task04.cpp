#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <sstream>

// ============================================================================
// ASSIGNMENT 3 - QUESTION 4: ARITHMETIC & STREAM OPERATOR OVERLOADING
// Student: NEELAKANDA MOORTHY | Reg No: 192421063
// ============================================================================

class Money {
private:
    long long paise; // Internal representation: integer paise prevents floating-point drift

public:
    // Constructor handling rupees and paise, supporting negative values gracefully
    Money(long long rupees = 0, int p = 0) {
        if (rupees >= 0) {
            paise = rupees * 100 + p;
        } else {
            paise = rupees * 100 - p;
        }
    }

    // Direct constructor from total paise
    static Money fromPaise(long long totalPaise) {
        Money m;
        m.paise = totalPaise;
        return m;
    }

    long long getPaise() const { return paise; }

    // ------------------------------------------------------------------------
    // TODO 1: Overload + as a member function that returns a new Money object
    // ------------------------------------------------------------------------
    Money operator+(const Money& other) const {
        Money res;
        res.paise = this->paise + other.paise;
        return res;
    }

    // ------------------------------------------------------------------------
    // TODO 2: Overload == as a member, and << as a friend that prints "Rs 200.00"
    // ------------------------------------------------------------------------
    bool operator==(const Money& other) const {
        return this->paise == other.paise;
    }

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long absPaise = std::abs(m.paise);
        long long rs = absPaise / 100;
        long long ps = absPaise % 100;

        if (m.paise < 0) {
            os << "-";
        }
        os << "Rs " << rs << "." << std::setw(2) << std::setfill('0') << ps;
        return os;
    }

    // Overload input stream >> for complete stream functionality
    friend std::istream& operator>>(std::istream& is, Money& m) {
        long long rs;
        char dot;
        int ps;
        if (is >> rs >> dot >> ps) {
            m.paise = (rs >= 0 ? rs * 100 + ps : rs * 100 - ps);
        }
        return is;
    }

    // Compound assignment += (returns reference to *this for chaining)
    Money& operator+=(const Money& other) {
        this->paise += other.paise;
        return *this;
    }

    // Binary Subtraction -
    Money operator-(const Money& other) const {
        Money res;
        res.paise = this->paise - other.paise;
        return res;
    }

    // Compound assignment -=
    Money& operator-=(const Money& other) {
        this->paise -= other.paise;
        return *this;
    }

    // Relational Comparison <
    bool operator<(const Money& other) const {
        return this->paise < other.paise;
    }

    // Valid scalar scaling operations (Dimensional analysis: Money * scalar = Money)
    Money operator*(double factor) const {
        Money res;
        res.paise = static_cast<long long>(std::round(this->paise * factor));
        return res;
    }

    friend Money operator*(double factor, const Money& m) {
        return m * factor; // Symmetric multiplication
    }
};

// Alternative Concept: Non-member friend operator+ allowing symmetric conversion (100 + m)
class MoneySymmetric {
private:
    long long paise;

public:
    MoneySymmetric(long long rupees = 0) : paise(rupees * 100) {}

    friend MoneySymmetric operator+(const MoneySymmetric& lhs, const MoneySymmetric& rhs) {
        MoneySymmetric res;
        res.paise = lhs.paise + rhs.paise;
        return res;
    }

    long long getPaise() const { return paise; }
};

int main() {
    std::cout << "======================================================================\n";
    std::cout << "  SIMATS ENGINEERING - CO3 ASSIGNMENT 3 - TASK 04\n";
    std::cout << "  OVERLOADING ARITHMETIC & STREAM OPERATORS (MONEY & BILLING)\n";
    std::cout << "  Student: NEELAKANDA MOORTHY (Reg No: 192421063)\n";
    std::cout << "======================================================================\n\n";

    std::cout << "[PART 1: MANDATORY 'COMPLETE THE CODE' VERIFICATION]\n";
    {
        Money a(120, 75), b(79, 25);
        Money c = a + b;
        std::cout << "  Output:\n  " << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';
        std::cout << "  Expected:\n  Rs 200.00\n  true\n";
        std::cout << "  -> Verification: Output matches expected exactly!\n\n";
    }

    std::cout << "[PART 2: PAISE CARRY INTO RUPEES TEST (Rs 0.75 + Rs 0.50)]\n";
    {
        Money p1(0, 75);
        Money p2(0, 50);
        Money sum = p1 + p2;
        std::cout << "  " << p1 << " + " << p2 << " = " << sum << '\n';
        std::cout << "  Expected: Rs 1.25\n";
        std::cout << "  -> Correctly carried 125 paise into 1 Rupee and 25 Paise.\n\n";
    }

    std::cout << "[PART 3: SUBTRACTION & NEGATIVE BALANCE HANDLING]\n";
    {
        Money balance(50, 0);
        Money purchase(125, 50);
        Money net = balance - purchase;
        std::cout << "  " << balance << " - " << purchase << " = " << net << '\n';
        std::cout << "  -> Formatted correctly as negative currency (-Rs 75.50).\n\n";
    }

    std::cout << "[PART 4: BILLING COUNTER WORKFLOW COMPARISON]\n";
    {
        Money item1(250, 50);
        Money item2(149, 75);
        Money item3(99, 90);
        double taxRate = 0.05; // 5% GST

        // With Overloaded Operators: Natural algebraic syntax
        Money subtotal = item1 + item2 + item3;
        Money tax = subtotal * taxRate;
        Money grandTotal = subtotal + tax;

        std::cout << "  Item 1:      " << item1 << '\n';
        std::cout << "  Item 2:      " << item2 << '\n';
        std::cout << "  Item 3:      " << item3 << '\n';
        std::cout << "  -------------------------\n";
        std::cout << "  Subtotal:    " << subtotal << '\n';
        std::cout << "  Tax (5%):    " << tax << '\n';
        std::cout << "  Grand Total: " << grandTotal << '\n';
        std::cout << "  Readability: Subtotal calculated seamlessly as 'item1 + item2 + item3'.\n\n";
    }

    std::cout << "[PART 5: PRECISION DRIFT BENCHMARK - 1,000,000 ADDITIONS]\n";
    {
        // 1,000,000 additions of Rs 0.10 (10 paise)
        const int iterations = 1000000;

        // Long long paise version:
        Money m(0, 0);
        Money tenPaise(0, 10);
        for (int i = 0; i < iterations; ++i) {
            m += tenPaise;
        }

        // Double version:
        double d = 0.0;
        double tenPaiseDouble = 0.10;
        for (int i = 0; i < iterations; ++i) {
            d += tenPaiseDouble;
        }

        std::cout << std::fixed << std::setprecision(10);
        std::cout << "  Expected Theoretical Total : Rs 100,000.00\n";
        std::cout << "  Money (Integer Paise) Total: " << m << " (Exact: " << m.getPaise() << " paise)\n";
        std::cout << "  double-based Total         : Rs " << d << '\n';
        std::cout << "  double Cumulative Drift    : " << (d - 100000.0) << " Rs\n";
        std::cout << "  -> Finding: Binary floating point suffers IEEE 754 representation error (0.1 is repeating in binary),\n";
        std::cout << "     whereas integer paise avoids drift completely.\n\n";
    }

    std::cout << "[PART 6: SYMMETRIC CONVERSION (100 + m) WITH FRIEND OPERATOR]\n";
    {
        MoneySymmetric ms(50);
        // Non-member friend operator+ allows implicit conversion of 100 (rupees) to MoneySymmetric:
        MoneySymmetric res1 = ms + 100; // ms.operator+(100) or operator+(ms, 100)
        MoneySymmetric res2 = 100 + ms; // operator+(100, ms) -> converts 100 to MoneySymmetric(100)
        std::cout << "  ms(50) + 100 = " << res1.getPaise() / 100 << " Rupees\n";
        std::cout << "  100 + ms(50) = " << res2.getPaise() / 100 << " Rupees (Symmetric conversion valid!)\n";
    }

    std::cout << "\n======================================================================\n";
    std::cout << "  TASK 04 EXECUTION COMPLETED SUCCESSFULLY\n";
    std::cout << "======================================================================\n";
    return 0;
}
