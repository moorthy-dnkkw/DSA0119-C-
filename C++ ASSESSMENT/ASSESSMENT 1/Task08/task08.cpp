/**
 * ============================================================================
 * Task 8: Read and Print Data Safely
 * Course: Object-Oriented Programming in C++ (CO1 - Bloom's Level: K5 Evaluate)
 * Title: Safe Stream I/O, Error State Recovery, and <iomanip> Report Formatting
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <sstream>
#include <vector>

struct JourneyRecord {
    std::string cardId;
    std::string destinationZone;
    int hour;
    bool isPeak;
    long long farePaise;
    long long balanceAfterPaise;
    bool approved;
};

// Function demonstrating safe integer reading with stream error recovery
int readSafeInt(std::istream& in, const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        if (in >> value) {
            if (value >= minVal && value <= maxVal) {
                return value;
            }
            std::cout << "  [INPUT ERROR]: Value out of range (" << minVal << " to " << maxVal << "). Please re-enter.\n";
        } else {
            // Stream entered fail state (e.g. non-numeric character entered)
            in.clear(); // Clear stream failbits
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Purge corrupt buffer
            std::cout << "  [STREAM ERROR]: Non-numeric characters detected and purged. Please enter a valid integer.\n";
        }
    }
}

void printFormattedReport(const std::vector<JourneyRecord>& records) {
    std::cout << "\n====================================================================================\n";
    std::cout << "                       NAMMA METRO GATE AUDIT REPORT                                \n";
    std::cout << "====================================================================================\n";
    std::cout << std::left 
              << std::setw(14) << "Card ID"
              << std::setw(12) << "Dest Zone"
              << std::setw(10) << "Time"
              << std::setw(10) << "Peak?"
              << std::setw(14) << "Fare (Rs)"
              << std::setw(14) << "Status"
              << std::setw(14) << "Balance (Rs)" << "\n";
    std::cout << std::string(84, '-') << "\n";

    long long totalRevenuePaise = 0;
    int totalApproved = 0;

    for (const auto& rec : records) {
        std::cout << std::left
                  << std::setw(14) << rec.cardId
                  << std::setw(12) << rec.destinationZone
                  << std::setw(2)  << rec.hour << ":00     "
                  << std::setw(10) << (rec.isPeak ? "PEAK" : "OFF-PEAK")
                  << "Rs " << std::fixed << std::setprecision(2) << std::setw(11) << (rec.farePaise / 100.0)
                  << std::setw(14) << (rec.approved ? "APPROVED" : "DENIED")
                  << "Rs " << std::setw(11) << (rec.balanceAfterPaise / 100.0) << "\n";

        if (rec.approved) {
            totalRevenuePaise += rec.farePaise;
            totalApproved++;
        }
    }

    std::cout << std::string(84, '=') << "\n";
    std::cout << "Journeys Processed: " << records.size() 
              << " | Approved: " << totalApproved 
              << " | Total Fare Collected: Rs " << std::fixed << std::setprecision(2) 
              << (totalRevenuePaise / 100.0) << "\n";
    std::cout << "====================================================================================\n";
}

int main() {
    std::cout << "==============================================================\n";
    std::cout << "         TASK 8: SAFE STREAM I/O & REPORT FORMATTING          \n";
    std::cout << "==============================================================\n\n";

    std::cout << "--- 1. Testing Stream Error Recovery with Simulated Bad Input ---\n";
    // Simulated input: operator types "abc", then 99 (out of range), then valid 8
    std::string testInput = "abc\n99\n8\nOuterZone\n1\n";
    std::istringstream mockStream(testInput);

    int validHour = readSafeInt(mockStream, "Enter journey hour [0-23]: ", 0, 23);
    std::cout << "Successfully recovered! Accepted Hour: " << validHour << ":00\n";

    std::cout << "\n--- 2. Formatting Money and Generating Detailed Audit Table ---\n";
    std::vector<JourneyRecord> sampleRecords = {
        {"CARD-001", "Inner",  8,  true,  1200, 8800, true},
        {"CARD-002", "Middle", 14, false, 2000, 6000, true},
        {"CARD-003", "Outer",  18, true,  3600, 4400, false}, // Leaves Rs 44 (< Rs 50) -> Denied
        {"CARD-004", "Outer",  20, false, 3000, 7000, true},
        {"CARD-005", "Inner",  10, false, 1000, 9000, true}
    };

    printFormattedReport(sampleRecords);

    std::cout << "\n--- 3. Alternative Concept Analysis: cin vs getline + stoi ---\n";
    std::cout << "Direct 'cin >>' Approach:\n"
              << " - Time Complexity: O(1) per numeric extraction\n"
              << " - Space Complexity: O(1) in-place extraction with zero heap allocation\n"
              << " - Error Recovery: Uses failbits; requires cin.clear() and cin.ignore()\n";
    std::cout << "getline + stoi + try/catch Approach:\n"
              << " - Time Complexity: O(L) where L is line length\n"
              << " - Space Complexity: Allocates dynamic string buffer on the heap\n"
              << " - Error Recovery: Clean exception catching (std::invalid_argument, std::out_of_range)\n"
              << "   but introduces substantial C++ exception frame runtime overhead.\n";

    return 0;
}
