#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// ============================================================================
// ASSIGNMENT 3 - QUESTION 3: DYNAMIC CONSTRUCTORS & DESTRUCTORS
// Student: NEELAKANDA MOORTHY | Reg No: 192421063
// ============================================================================

// Logger struct for the Predict the Output question
struct Logger {
    std::string n;
    Logger(const std::string& s) : n(s) { std::cout << '+' << n << ' '; }
    Logger(const Logger& o) : n(o.n + "c") { std::cout << "copy" << n << ' '; }
    ~Logger() { std::cout << '-' << n << ' '; }
};

void use(Logger) { std::cout << "use "; }

// Dynamic Auditorium Seat Map Class
class AuditoriumEvent {
private:
    std::string eventName;
    int rows;
    int seatsPerRow;
    char** seatMap; // 2D dynamically allocated array of characters ('O' = open, 'X' = booked)
    int bookedCount;

public:
    // Dynamic Constructor
    AuditoriumEvent(std::string name, int r, int s)
        : eventName(std::move(name)), rows(r), seatsPerRow(s), bookedCount(0) {
        std::cout << "  [Dynamic Ctor] Setting up Event \"" << eventName 
                  << "\" (" << rows << " rows x " << seatsPerRow << " seats)...\n";
        
        if (rows <= 0 || seatsPerRow <= 0) {
            std::cout << "  [Dynamic Ctor Notice] 0 or negative dimensions specified. Allocated empty seat map.\n";
            rows = 0;
            seatsPerRow = 0;
            seatMap = nullptr;
            return;
        }

        // Dynamic 2D allocation: 1 pointer array + r row arrays
        seatMap = new char*[rows];
        for (int i = 0; i < rows; ++i) {
            seatMap[i] = new char[seatsPerRow];
            for (int j = 0; j < seatsPerRow; ++j) {
                seatMap[i][j] = 'O'; // 'O' indicates Available
            }
        }
        std::cout << "  [Dynamic Ctor] Successfully allocated " << (rows + 1) 
                  << " dynamic heap blocks for \"" << eventName << "\".\n";
    }

    // Disable copy for simplicity (focus on dynamic construction/destruction)
    AuditoriumEvent(const AuditoriumEvent&) = delete;
    AuditoriumEvent& operator=(const AuditoriumEvent&) = delete;

    // Destructor - RAII Guarantee: Releasing every dynamic allocation
    ~AuditoriumEvent() {
        std::cout << "  [Destructor] Closing Event \"" << eventName << "\"...\n";
        std::cout << "    -> Final Summary: " << bookedCount << " / " 
                  << (rows * seatsPerRow) << " seats occupied ("
                  << (rows * seatsPerRow > 0 ? (bookedCount * 100.0 / (rows * seatsPerRow)) : 0.0) 
                  << "% occupancy).\n";

        if (seatMap != nullptr) {
            for (int i = 0; i < rows; ++i) {
                delete[] seatMap[i]; // Delete each individual row
            }
            delete[] seatMap; // Delete row pointer array
            seatMap = nullptr;
            std::cout << "    -> Cleaned up " << (rows + 1) << " heap allocations. Zero leaks.\n";
        }
    }

    bool bookSeat(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= seatsPerRow) {
            std::cout << "    [Error] Invalid seat (" << r << ", " << c << ")\n";
            return false;
        }
        if (seatMap[r][c] == 'X') {
            std::cout << "    [Warning] Seat (" << r << ", " << c << ") is already booked.\n";
            return false;
        }
        seatMap[r][c] = 'X';
        ++bookedCount;
        return true;
    }

    bool cancelSeat(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= seatsPerRow) return false;
        if (seatMap[r][c] == 'O') return false;
        seatMap[r][c] = 'O';
        --bookedCount;
        return true;
    }

    void displayMiniMap() const {
        if (!seatMap || rows == 0) {
            std::cout << "    [Empty Map]\n";
            return;
        }
        std::cout << "    Seat Map Preview (First up to 5x5):\n";
        int dispR = std::min(rows, 5);
        int dispC = std::min(seatsPerRow, 5);
        for (int i = 0; i < dispR; ++i) {
            std::cout << "      Row " << std::setw(2) << i << ": ";
            for (int j = 0; j < dispC; ++j) {
                std::cout << seatMap[i][j] << ' ';
            }
            if (seatsPerRow > 5) std::cout << "...";
            std::cout << '\n';
        }
    }
};

// Alternative Concept: Contiguous 1D flat allocation
class AuditoriumFlat {
private:
    std::string name;
    int rows;
    int cols;
    char* data; // Single contiguous allocation: rows * cols elements

public:
    AuditoriumFlat(std::string n, int r, int c)
        : name(std::move(n)), rows(r), cols(c), data(new char[r * c]) {
        for (int i = 0; i < r * c; ++i) data[i] = 'O';
    }
    ~AuditoriumFlat() {
        delete[] data; // Single delete[] call!
    }
    char& at(int r, int c) { return data[r * cols + c]; }
};

int main() {
    std::cout << "======================================================================\n";
    std::cout << "  SIMATS ENGINEERING - CO3 ASSIGNMENT 3 - TASK 03\n";
    std::cout << "  DYNAMIC CONSTRUCTORS & DESTRUCTORS (RAII & AUDITORIUM SEATS)\n";
    std::cout << "  Student: NEELAKANDA MOORTHY (Reg No: 192421063)\n";
    std::cout << "======================================================================\n\n";

    std::cout << "[PART 1: PREDICT THE OUTPUT VERIFICATION]\n";
    std::cout << "  Executing given Logger sequence:\n  Output: ";
    {
        Logger a("A");
        {
            Logger b("B");
        }
        use(a);
        Logger* p = new Logger("P");
        delete p;
        std::cout << "end ";
    }
    std::cout << "\n  Expected: +A +B -B copyAc use -Ac +P -P end -A \n\n";

    std::cout << "[PART 2: STACK, BLOCK-SCOPE, AND HEAP OBJECT LIFETIMES]\n";
    std::cout << "--- 1. Creating Stack Object 'Annual Convocation' ---\n";
    AuditoriumEvent stackEvent("Annual Convocation", 4, 8);
    stackEvent.bookSeat(0, 0);
    stackEvent.bookSeat(0, 1);
    stackEvent.bookSeat(1, 4);

    {
        std::cout << "\n--- 2. Creating Block-Scope Object 'Guest Lecture' inside { ... } ---\n";
        AuditoriumEvent blockEvent("Guest Lecture", 3, 5);
        blockEvent.bookSeat(0, 2);
        blockEvent.displayMiniMap();
        std::cout << "--- Exiting block (blockEvent will be destroyed immediately) ---\n";
    }

    std::cout << "\n--- 3. Creating Heap Object 'Symphony Concert' via new ---\n";
    AuditoriumEvent* heapEvent = new AuditoriumEvent("Symphony Concert", 5, 10);
    heapEvent->bookSeat(2, 5);
    heapEvent->bookSeat(3, 7);
    heapEvent->displayMiniMap();
    std::cout << "--- Explicitly deleting heapEvent via delete ---\n";
    delete heapEvent;

    std::cout << "\n[PART 3: EDGE CASE - EVENT WITH ZERO ROWS]\n";
    {
        AuditoriumEvent zeroEvent("Cancelled Show", 0, 0);
        zeroEvent.displayMiniMap();
    }

    std::cout << "\n[PART 4: MEMORY MEASUREMENT ANALYSIS ACROSS SIZES]\n";
    struct SizeTest { int r; int c; };
    SizeTest sizes[] = {{10, 20}, {100, 200}, {1000, 2000}};
    for (auto& s : sizes) {
        size_t pointerArrayBytes = s.r * sizeof(char*);
        size_t rowDataBytes = (size_t)s.r * s.c * sizeof(char);
        size_t totalBytes2D = pointerArrayBytes + rowDataBytes;
        size_t flatBytes = rowDataBytes;
        size_t overheadSaved = pointerArrayBytes;

        std::cout << "  Seat Map (" << s.r << " x " << s.c << "):\n"
                  << "    - Dynamic 2D: " << (s.r + 1) << " allocations, "
                  << totalBytes2D << " bytes (" << (totalBytes2D / 1024.0) << " KB)\n"
                  << "    - Flat 1D:    1 allocation, " 
                  << flatBytes << " bytes (" << (flatBytes / 1024.0) << " KB)\n"
                  << "    - Pointer Overhead Saved by Flat: " << overheadSaved << " bytes ("
                  << (overheadSaved / 1024.0) << " KB)\n";
    }

    std::cout << "\n--- End of main() reached: stackEvent will now be destroyed ---\n";
    return 0;
}
