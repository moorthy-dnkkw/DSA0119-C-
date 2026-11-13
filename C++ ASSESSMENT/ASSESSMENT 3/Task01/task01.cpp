#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <vector>

// ============================================================================
// ASSIGNMENT 3 - QUESTION 1: CONSTRUCTORS & CONSTRUCTOR OVERLOADING
// Student: NEELAKANDA MOORTHY | Reg No: 192421063
// ============================================================================

// Class demonstrating member initializer lists, delegating constructors,
// and invariant protection for number of copies.
class Book {
private:
    std::string title;
    std::string author;
    std::string isbn;
    int copies;

    // Centralized copy validator ensuring no Book has negative copies
    static int validateCopies(int c) {
        if (c < 0) {
            std::cout << "  [Validation Warning] Invalid copies (" << c 
                      << ") detected! Enforcing invariant copies >= 0 -> clamped to 0.\n";
            return 0;
        }
        return c;
    }

public:
    // 1. Primary Parameterised Constructor (Master Target for Delegation)
    Book(std::string t, std::string a, std::string i, int c)
        : title(std::move(t)), author(std::move(a)), isbn(std::move(i)), copies(validateCopies(c)) {
        std::cout << "  [Target Constructor] Initialised: \"" << title 
                  << "\" by " << author << " (Copies: " << copies << ")\n";
    }

    // 2. Default Constructor (Empty Record to be filled later) - DELEGATES to Master
    Book() : Book("Untitled", "Unknown", "N/A", 0) {
        std::cout << "  [Default Constructor] Empty record created via delegation.\n";
    }

    // 3. Donated Book Constructor (Title only) - Marked EXPLICIT to prevent accidental implicit conversion
    explicit Book(std::string t) : Book(std::move(t), "Anonymous", "DONATED", 1) {
        std::cout << "  [Overloaded Constructor 1] Donated book (title only) created via delegation.\n";
    }

    // 4. Donated Book Constructor (Title & Author) - DELEGATES to Master
    Book(std::string t, std::string a) : Book(std::move(t), std::move(a), "DONATED", 1) {
        std::cout << "  [Overloaded Constructor 2] Donated book (title + author) created via delegation.\n";
    }

    // Accessors
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getISBN() const { return isbn; }
    int getCopies() const { return copies; }

    void printRecord(int id) const {
        std::cout << "  Book #" << id << " | "
                  << std::left << std::setw(30) << title << " | "
                  << std::setw(22) << author << " | "
                  << std::setw(16) << isbn << " | "
                  << "Copies: " << copies << "\n";
    }
};

// Alternative Design: Single constructor with default arguments
class BookWithDefaults {
private:
    std::string title;
    std::string author;
    std::string isbn;
    int copies;

    static int validateCopies(int c) {
        return (c < 0) ? 0 : c;
    }

public:
    // Single constructor with default arguments replacing 4 overloaded constructors
    BookWithDefaults(std::string t = "Untitled", 
                     std::string a = "Unknown", 
                     std::string i = "N/A", 
                     int c = 0)
        : title(std::move(t)), author(std::move(a)), isbn(std::move(i)), copies(validateCopies(c)) {}

    void print() const {
        std::cout << "  [BookWithDefaults] \"" << title << "\" | " << author 
                  << " | " << isbn << " | Copies: " << copies << "\n";
    }
};

// Demonstration helper for explicit constructor
void processDonation(const Book& b) {
    std::cout << "  Donation accepted: \"" << b.getTitle() << "\"\n";
}

int main() {
    std::cout << "======================================================================\n";
    std::cout << "  SIMATS ENGINEERING - CO3 ASSIGNMENT 3 - TASK 01\n";
    std::cout << "  CONSTRUCTORS, CONSTRUCTOR OVERLOADING, DELEGATION & EXPLICIT\n";
    std::cout << "  Student: NEELAKANDA MOORTHY (Reg No: 192421063)\n";
    std::cout << "======================================================================\n\n";

    std::cout << "[PART 1: OBJECT CREATION WITH ALL CONSTRUCTORS]\n";
    std::cout << "--- Creating b1 (Empty record via Default Constructor) ---\n";
    Book b1;

    std::cout << "\n--- Creating b2 (Donated Book via Title-only Overloaded Constructor) ---\n";
    Book b2("Introduction to Algorithms");

    std::cout << "\n--- Creating b3 (Donated Book via Title+Author Overloaded Constructor) ---\n";
    Book b3("The C++ Programming Language", "Bjarne Stroustrup");

    std::cout << "\n--- Creating b4 (Fully Parameterised Constructor) ---\n";
    Book b4("Clean Code", "Robert C. Martin", "978-0132350884", 12);

    std::cout << "\n--- Creating b5 (Parameterised with Negative Copies - Invariant Enforcement) ---\n";
    Book b5("Corrupted Ledger", "Malicious User", "000-0000000000", -7);

    std::cout << "\n[PART 2: DISPLAYING CONSTRUCTED BOOK RECORDS]\n";
    std::cout << "  -------------------------------------------------------------------------------------------------\n";
    b1.printRecord(1);
    b2.printRecord(2);
    b3.printRecord(3);
    b4.printRecord(4);
    b5.printRecord(5);
    std::cout << "  -------------------------------------------------------------------------------------------------\n";

    std::cout << "\n[PART 3: DEMONSTRATING EXPLICIT CONSTRUCTOR]\n";
    // Direct construction is allowed:
    Book b_explicit("Design Patterns");
    processDonation(b_explicit);
    std::cout << "  -> Note: Implicit conversion 'processDonation(\"Refactoring\");' is blocked by 'explicit'.\n";
    std::cout << "  -> Direct conversion 'processDonation(Book(\"Refactoring\"));' is strictly required.\n";
    processDonation(Book("Refactoring"));

    std::cout << "\n[PART 4: ARRAY OF OBJECTS (DEPENDENCE ON DEFAULT CONSTRUCTOR)]\n";
    std::cout << "  Creating Book shelf[3] calls default constructor 3 times:\n";
    Book shelf[3];
    for (int i = 0; i < 3; ++i) {
        shelf[i].printRecord(10 + i);
    }

    std::cout << "\n[PART 5: ALTERNATIVE DESIGN - SINGLE CONSTRUCTOR WITH DEFAULT ARGUMENTS]\n";
    BookWithDefaults bd1;
    BookWithDefaults bd2("Operating System Concepts");
    BookWithDefaults bd3("Computer Networks", "Andrew Tanenbaum");
    BookWithDefaults bd4("Artificial Intelligence", "Russell & Norvig", "978-0136042594", 5);
    bd1.print();
    bd2.print();
    bd3.print();
    bd4.print();

    std::cout << "\n======================================================================\n";
    std::cout << "  TASK 01 EXECUTION COMPLETED SUCCESSFULLY\n";
    std::cout << "======================================================================\n";
    return 0;
}
