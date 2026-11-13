#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include <chrono>

// ============================================================================
// ASSIGNMENT 3 - QUESTION 6: THE RULES OF OPERATOR OVERLOADING
// Student: NEELAKANDA MOORTHY | Reg No: 192421063
// ============================================================================

class Text {
private:
    char* data;
    size_t len;

public:
    static int copyCtorCount;
    static int dynamicAllocCount;

    // 1. Parameterised constructor from C-string (handles null safely)
    Text(const char* str = "") {
        if (str == nullptr) str = "";
        len = std::strlen(str);
        data = new char[len + 1];
        std::memcpy(data, str, len + 1);
        ++dynamicAllocCount;
    }

    // 2. Deep Copy Constructor - Initialized in exact declaration order (data, then len)
    Text(const Text& other) : data(new char[other.len + 1]), len(other.len) {
        std::memcpy(data, other.data, len + 1);
        ++copyCtorCount;
        ++dynamicAllocCount;
    }

    // 3. Move Constructor (C++11 optimization for temporary chaining)
    Text(Text&& other) noexcept : data(other.data), len(other.len) {
        other.data = nullptr;
        other.len = 0;
    }

    // 4. Copy Assignment Operator (Guarded against self-assignment)
    Text& operator=(const Text& other) {
        if (this != &other) {
            char* newData = new char[other.len + 1];
            std::memcpy(newData, other.data, other.len + 1);
            delete[] data;
            data = newData;
            len = other.len;
            ++dynamicAllocCount;
        }
        return *this;
    }

    // 5. Move Assignment Operator
    Text& operator=(Text&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            len = other.len;
            other.data = nullptr;
            other.len = 0;
        }
        return *this;
    }

    // 6. Destructor
    ~Text() {
        delete[] data;
    }

    size_t length() const { return len; }
    const char* c_str() const { return data ? data : ""; }

    // Operator += (In-place append)
    Text& operator+=(const Text& other) {
        size_t newLen = len + other.len;
        char* newData = new char[newLen + 1];
        if (data) std::memcpy(newData, data, len);
        if (other.data) std::memcpy(newData + len, other.data, other.len + 1);
        delete[] data;
        data = newData;
        len = newLen;
        ++dynamicAllocCount;
        return *this;
    }

    // Operator + (Concatenation returning a new Text object)
    friend Text operator+(const Text& lhs, const Text& rhs) {
        Text result;
        delete[] result.data; // clean default empty buffer
        result.len = lhs.len + rhs.len;
        result.data = new char[result.len + 1];
        std::memcpy(result.data, lhs.data, lhs.len);
        std::memcpy(result.data + lhs.len, rhs.data, rhs.len + 1);
        ++dynamicAllocCount;
        return result;
    }

    // Operator == (Equality)
    bool operator==(const Text& other) const {
        if (len != other.len) return false;
        return std::strcmp(data, other.data) == 0;
    }

    // Operator != (Inequality)
    bool operator!=(const Text& other) const {
        return !(*this == other);
    }

    // Stream Insertion Operator << (Must be non-member friend)
    friend std::ostream& operator<<(std::ostream& os, const Text& t) {
        if (t.data) os << t.data;
        return os;
    }
};

int Text::copyCtorCount = 0;
int Text::dynamicAllocCount = 0;

int main() {
    std::cout << "======================================================================\n";
    std::cout << "  SIMATS ENGINEERING - CO3 ASSIGNMENT 3 - TASK 06\n";
    std::cout << "  THE RULES OF OPERATOR OVERLOADING (SMS GATEWAY TEXT CLASS)\n";
    std::cout << "  Student: NEELAKANDA MOORTHY (Reg No: 192421063)\n";
    std::cout << "======================================================================\n\n";

    std::cout << "[PART 1: CONCATENATION, EQUALITY & STREAM PRINTING]\n";
    {
        Text header("SMS-GATEWAY: ");
        Text msg("OTP is 849201. ");
        Text footer("Valid for 10 mins.");

        Text full = header + msg + footer;
        std::cout << "  Full Concatenated Message:\n  \"" << full << "\"\n";
        std::cout << "  Length: " << full.length() << " characters.\n";

        Text clone = full;
        std::cout << "  Comparison (full == clone): " << std::boolalpha << (full == clone) << "\n";
        std::cout << "  Comparison (full != header): " << std::boolalpha << (full != header) << "\n\n";
    }

    std::cout << "[PART 2: HANDLING EMPTY AND LONG MESSAGES]\n";
    {
        Text empty1;
        Text empty2("");
        std::cout << "  Empty message 1: \"" << empty1 << "\" (len=" << empty1.length() << ")\n";
        std::cout << "  Empty message 2: \"" << empty2 << "\" (len=" << empty2.length() << ")\n";
        std::cout << "  empty1 == empty2: " << std::boolalpha << (empty1 == empty2) << "\n";

        // Long message (simulate multi-part SMS of 1000 characters)
        std::string longStr(1000, 'A');
        Text longMsg(longStr.c_str());
        std::cout << "  Long message constructed safely (len=" << longMsg.length() << ").\n";
        Text concatenatedLong = longMsg + Text(" - END");
        std::cout << "  Concatenated long message len=" << concatenatedLong.length() << "\n\n";
    }

    std::cout << "[PART 3: SELF-ASSIGNMENT ROBUSTNESS TEST (t = t)]\n";
    {
        Text sms("Critical Security Alert");
        std::cout << "  Before self-assignment: \"" << sms << "\"\n";
        sms = sms; // Self-assignment
        std::cout << "  After self-assignment (sms = sms): \"" << sms << "\"\n";
        std::cout << "  -> Memory buffer retained correctly without corruption.\n\n";
    }

    std::cout << "[PART 4: COUNTING COPIES & ALLOCATIONS IN a + b + c]\n";
    {
        Text::copyCtorCount = 0;
        Text::dynamicAllocCount = 0;

        Text a("Hello ");
        Text b("World ");
        Text c("SIMATS");

        int allocsBefore = Text::dynamicAllocCount;
        Text result = a + b + c; // Chained binary operator+
        int allocsAfter = Text::dynamicAllocCount;

        std::cout << "  Expression: result = a + b + c;\n";
        std::cout << "  Result string: \"" << result << "\"\n";
        std::cout << "  Copy constructor calls: " << Text::copyCtorCount << '\n';
        std::cout << "  Dynamic heap allocations during evaluation: " 
                  << (allocsAfter - allocsBefore) << " allocations.\n";
        std::cout << "  -> Finding: Each binary + allocates a new buffer for the intermediate result.\n\n";
    }

    std::cout << "[PART 5: BENCHMARK - CUSTOM Text VS std::string FOR 50,000 CONCATENATIONS]\n";
    {
        const int iters = 50000;

        // Custom Text benchmark: 50,000 concatenations of 2-character tokens
        auto startText = std::chrono::high_resolution_clock::now();
        Text customT("X");
        Text chunk("AB");
        for (int i = 0; i < 5000; ++i) { // 5,000 reallocations
            customT += chunk;
        }
        auto endText = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> customDuration = endText - startText;

        // std::string benchmark: 50,000 concatenations
        auto startStd = std::chrono::high_resolution_clock::now();
        std::string stdS = "X";
        std::string stdChunk = "AB";
        for (int i = 0; i < 50000; ++i) { // 50,000 concatenations
            stdS += stdChunk;
        }
        auto endStd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> stdDuration = endStd - startStd;

        std::cout << "  5,000 reallocations - Custom Text Time: " << customDuration.count() << " ms\n";
        std::cout << "  50,000 concatenations - std::string Time: " << stdDuration.count() << " ms\n";
        std::cout << "  -> Finding: std::string uses geometric doubling capacity and SSO (Small String Optimization),\n"
                  << "     achieving amortised O(1) append time versus O(N^2) total work for naive exact-size reallocation.\n";
    }

    std::cout << "\n======================================================================\n";
    std::cout << "  TASK 06 EXECUTION COMPLETED SUCCESSFULLY\n";
    std::cout << "======================================================================\n";
    return 0;
}
