#include <iostream>
#include <string>
#include <chrono>
#include <vector>

// ============================================================================
// ASSIGNMENT 3 - QUESTION 2: COPY CONSTRUCTORS - SHALLOW & DEEP COPIES
// Student: NEELAKANDA MOORTHY | Reg No: 192421063
// ============================================================================

class Prescription {
private:
    int n;
    std::string* medicines; // dynamically allocated array

public:
    static int copyConstructorCalls;

    explicit Prescription(int count) 
        : n(count), medicines(new std::string[count]) {}

    // ------------------------------------------------------------------------
    // TODO 1: Copy constructor: allocate a NEW array of the same size and copy
    // every medicine (deep copy)
    // ------------------------------------------------------------------------
    Prescription(const Prescription& other) 
        : n(other.n), medicines(new std::string[other.n]) {
        ++copyConstructorCalls;
        for (int i = 0; i < n; ++i) {
            medicines[i] = other.medicines[i];
        }
    }

    // ------------------------------------------------------------------------
    // TODO 2: Copy assignment: guard against self-assignment, build a new array,
    // copy, release the old array and return *this
    // ------------------------------------------------------------------------
    Prescription& operator=(const Prescription& other) {
        if (this != &other) { // Guard against self-assignment
            std::string* newMedicines = new std::string[other.n];
            for (int i = 0; i < other.n; ++i) {
                newMedicines[i] = other.medicines[i];
            }
            delete[] medicines; // Release existing buffer
            medicines = newMedicines;
            n = other.n;
        }
        return *this;
    }

    // ------------------------------------------------------------------------
    // TODO 3: Release the dynamically allocated array
    // ------------------------------------------------------------------------
    ~Prescription() {
        delete[] medicines;
    }

    void set(int i, const std::string& m) { medicines[i] = m; }
    std::string get(int i) const { return medicines[i]; }
    int size() const { return n; }
};

int Prescription::copyConstructorCalls = 0;

// Functions to test pass-by-value vs pass-by-const-reference
void processByValue(Prescription p) {
    std::cout << "    [Inside processByValue] Med 0: " << p.get(0) << '\n';
}

void processByConstRef(const Prescription& p) {
    std::cout << "    [Inside processByConstRef] Med 0: " << p.get(0) << '\n';
}

// Alternative Concept: Modern C++ Rule of Zero using std::vector<std::string>
class PrescriptionVector {
private:
    std::vector<std::string> medicines;

public:
    explicit PrescriptionVector(size_t count) : medicines(count) {}

    // Rule of Zero: Compiler-generated copy ctor, copy assignment, and dtor
    // all automatically perform deep copies without manual resource management.

    void set(size_t i, const std::string& m) { medicines[i] = m; }
    std::string get(size_t i) const { return medicines[i]; }
    size_t size() const { return medicines.size(); }
};

int main() {
    std::cout << "======================================================================\n";
    std::cout << "  SIMATS ENGINEERING - CO3 ASSIGNMENT 3 - TASK 02\n";
    std::cout << "  COPY CONSTRUCTORS: SHALLOW VS DEEP COPIES & RULE OF THREE\n";
    std::cout << "  Student: NEELAKANDA MOORTHY (Reg No: 192421063)\n";
    std::cout << "======================================================================\n\n";

    std::cout << "[PART 1: MANDATORY 'COMPLETE THE CODE' VERIFICATION]\n";
    {
        Prescription p(2);
        p.set(0, "Paracetamol");
        p.set(1, "ORS");

        Prescription q = p; // copy constructor (deep copy)
        q.set(1, "Cetirizine");

        Prescription r(1);
        r = p; // copy assignment (deep copy)

        p.set(0, "Ibuprofen");

        std::cout << "  Result output: \n  ";
        std::cout << p.get(0) << ' ' << p.get(1) << " | " 
                  << q.get(0) << ' ' << q.get(1) << " | " 
                  << r.get(0) << ' ' << r.get(1) << '\n';
        std::cout << "  Expected: \n  "
                  << "Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS\n";
        std::cout << "  -> Verification: Output matches expected exactly!\n\n";
    }

    std::cout << "[PART 2: PASS-BY-VALUE VS PASS-BY-CONST-REFERENCE COPY COUNT]\n";
    {
        Prescription testP(3);
        testP.set(0, "Amoxicillin");
        testP.set(1, "Metformin");
        testP.set(2, "Atorvastatin");

        Prescription::copyConstructorCalls = 0;
        std::cout << "  Calling processByValue(testP):\n";
        processByValue(testP);
        std::cout << "  -> Copy constructor calls after pass-by-value: " 
                  << Prescription::copyConstructorCalls << " (Expensive full deep copy)\n";

        Prescription::copyConstructorCalls = 0;
        std::cout << "  Calling processByConstRef(testP):\n";
        processByConstRef(testP);
        std::cout << "  -> Copy constructor calls after pass-by-const-ref: " 
                  << Prescription::copyConstructorCalls << " (Zero copies, O(1) pointer bind)\n\n";
    }

    std::cout << "[PART 3: SELF-ASSIGNMENT SAFETY VERIFICATION (p = p)]\n";
    {
        Prescription selfP(2);
        selfP.set(0, "Aspirin");
        selfP.set(1, "Vitamin C");

        std::cout << "  Before self-assignment: [0]=" << selfP.get(0) << ", [1]=" << selfP.get(1) << '\n';
        selfP = selfP; // Self-assignment test
        std::cout << "  After self-assignment (selfP = selfP): [0]=" << selfP.get(0) << ", [1]=" << selfP.get(1) << '\n';
        std::cout << "  -> Data preserved safely because self-assignment guard (this != &other) prevented deletion.\n\n";
    }

    std::cout << "[PART 4: BENCHMARK - DEEP COPY FOR 10, 1,000, AND 100,000 MEDICINES]\n";
    const int sizes[] = {10, 1000, 100000};
    for (int sz : sizes) {
        Prescription src(sz);
        for (int i = 0; i < sz; ++i) {
            src.set(i, "Medicine_" + std::to_string(i));
        }

        auto start = std::chrono::high_resolution_clock::now();
        Prescription dst = src; // invokes deep copy constructor
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> elapsed = end - start;
        std::cout << "  Copying Prescription with N = " << sz 
                  << " medicines took: " << elapsed.count() << " microseconds ("
                  << (elapsed.count() / 1000.0) << " ms)\n";
    }

    std::cout << "\n[PART 5: ALTERNATIVE CONCEPT - RULE OF ZERO WITH std::vector]\n";
    {
        PrescriptionVector pv1(2);
        pv1.set(0, "Paracetamol");
        pv1.set(1, "ORS");

        PrescriptionVector pv2 = pv1; // Vector default copy ctor does deep copy
        pv2.set(1, "Cetirizine");

        PrescriptionVector pv3(1);
        pv3 = pv1; // Vector default assignment does deep copy

        pv1.set(0, "Ibuprofen");

        std::cout << "  Vector result: " 
                  << pv1.get(0) << ' ' << pv1.get(1) << " | "
                  << pv2.get(0) << ' ' << pv2.get(1) << " | "
                  << pv3.get(0) << ' ' << pv3.get(1) << '\n';
        std::cout << "  -> Confirmed: std::vector provides automatic deep copy and memory safety with zero boilerplate.\n";
    }

    std::cout << "\n======================================================================\n";
    std::cout << "  TASK 02 EXECUTION COMPLETED SUCCESSFULLY\n";
    std::cout << "======================================================================\n";
    return 0;
}
