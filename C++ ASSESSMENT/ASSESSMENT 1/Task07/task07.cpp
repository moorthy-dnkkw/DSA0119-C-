/**
 * ============================================================================
 * Task 7: From Procedural to Object-Oriented
 * Course: Object-Oriented Programming in C++ (CO1 - Bloom's Level: K5 Evaluate)
 * Title: Procedural vs Object-Oriented Architecture, Invariant Protection & Mutability
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>

// ============================================================================
// PART A: PROCEDURAL IMPLEMENTATION (Global State & Free Functions)
// ============================================================================
namespace ProceduralSystem {
    long long globalCardBalance = 6000; // Rs 60.00 in paise
    constexpr long long MIN_BAL = 5000;  // Rs 50.00

    bool canTravel(long long balance, long long fare) {
        return (balance - fare) >= MIN_BAL;
    }

    void deduct(long long& balance, long long fare) {
        // Vulnerability: In procedural style, nothing forces the caller to check canTravel!
        balance -= fare;
    }

    void runProceduralDemo() {
        std::cout << "[Procedural Run]\n";
        std::cout << "Initial Global Balance: Rs " << globalCardBalance / 100.0 << "\n";
        long long fare = 2000; // Rs 20.00 (Middle Zone)

        // Bug Scenario: Developer forgets canTravel() or bypasses check:
        std::cout << "Attempting travel with fare Rs " << fare / 100.0 << "...\n";
        if (canTravel(globalCardBalance, fare)) {
            deduct(globalCardBalance, fare);
            std::cout << "Travel permitted. Balance: Rs " << globalCardBalance / 100.0 << "\n";
        } else {
            std::cout << "Travel denied (low balance).\n";
        }

        // Direct modification vulnerability:
        std::cout << "Vulnerability: Rogue free function can directly modify state:\n";
        globalCardBalance = -2500; // Direct corruption!
        std::cout << "Corrupted Global Balance: Rs " << globalCardBalance / 100.0 << " (NEGATIVE BALANCE BUG!)\n";
    }
}

// ============================================================================
// PART B: OBJECT-ORIENTED IMPLEMENTATION (Encapsulation & Class Invariants)
// ============================================================================
namespace OOP_System {
    class SmartCard {
    private:
        std::string cardId;
        long long balancePaise; // Strict private encapsulation
        static constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50.00

    public:
        SmartCard(const std::string& id, long long initialBalance)
            : cardId(id), balancePaise(initialBalance >= 0 ? initialBalance : 0) {}

        std::string getId() const { return cardId; }
        long long getBalance() const { return balancePaise; }

        bool canTravel(long long fare) const {
            return (balancePaise - fare) >= MIN_BALANCE_PAISE;
        }

        bool deduct(long long fare) {
            // Invariant Gatekeeper: Negative balance is physically impossible!
            if (!canTravel(fare)) {
                return false;
            }
            balancePaise -= fare;
            return true;
        }

        bool recharge(long long amountPaise) {
            if (amountPaise <= 0) {
                return false; // Rejects negative or zero recharge attempts
            }
            balancePaise += amountPaise;
            return true;
        }
    };

    void runOOPDemo() {
        std::cout << "\n[Object-Oriented Run]\n";
        SmartCard card("CARD-METRO-007", 6000); // Rs 60.00
        std::cout << "Card ID: " << card.getId() 
                  << " | Initial Balance: Rs " << card.getBalance() / 100.0 << "\n";

        long long fare = 2000; // Rs 20.00
        std::cout << "Attempting to deduct Rs 20.00 (Leaves Rs 40.00, below Rs 50.00 min threshold)...\n";
        
        bool success = card.deduct(fare);
        if (success) {
            std::cout << "Deduction succeeded. Remaining: Rs " << card.getBalance() / 100.0 << "\n";
        } else {
            std::cout << "Deduction REJECTED by class invariant. Remaining: Rs " 
                      << card.getBalance() / 100.0 << "\n";
        }

        // Direct tampering attempt:
        // card.balancePaise = -2500; // COMPILE ERROR! 'long long OOP_System::SmartCard::balancePaise' is private
        std::cout << "Direct tampering (card.balance = -2500) is IMPOSSIBLE: Prevented at compile time by encapsulation!\n";
    }
}

int main() {
    std::cout << "==============================================================\n";
    std::cout << "    TASK 7: PROCEDURAL VS OBJECT-ORIENTED FARE LOGIC          \n";
    std::cout << "==============================================================\n\n";

    ProceduralSystem::runProceduralDemo();
    OOP_System::runOOPDemo();

    std::cout << "\n==============================================================\n";
    std::cout << "                  ARCHITECTURAL EVALUATION                    \n";
    std::cout << "==============================================================\n";
    std::cout << "1. Encapsulation: SmartCard restricts direct state access, making\n"
              << "   negative balances structurally impossible.\n";
    std::cout << "2. Single Point of Change: Changing the Rs 50 min balance rule\n"
              << "   requires altering exactly 1 line inside SmartCard.\n"
              << "   In procedural code, every independent check scattered across\n"
              << "   the codebase must be tracked down and updated manually.\n";
    std::cout << "3. Complexity: Time is O(1) and Space is O(1) in both designs,\n"
              << "   meaning OOP achieves complete safety at ZERO runtime cost.\n";

    return 0;
}
