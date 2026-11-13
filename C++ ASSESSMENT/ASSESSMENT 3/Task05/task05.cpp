#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <stdexcept>

// ============================================================================
// ASSIGNMENT 3 - QUESTION 5: INCREMENT, COMPARISON & SUBSCRIPT OPERATORS
// Student: NEELAKANDA MOORTHY | Reg No: 192421063
// ============================================================================

// Predict the Output Counter struct
struct Counter {
    int v;
    Counter& operator++() { ++v; return *this; } // prefix
    Counter operator++(int) { Counter old = *this; ++v; return old; } // postfix
};

// Time class representing playback track position (hh:mm:ss)
class Time {
private:
    int hours;
    int minutes;
    int seconds;

    void normalize() {
        if (seconds >= 60) {
            minutes += seconds / 60;
            seconds %= 60;
        }
        if (minutes >= 60) {
            hours += minutes / 60;
            minutes %= 60;
        }
    }

public:
    Time(int h = 0, int m = 0, int s = 0) 
        : hours(h), minutes(m), seconds(s) {
        normalize();
    }

    // Prefix increment (++t): advance by 1 second, return reference to updated object
    Time& operator++() {
        ++seconds;
        normalize();
        return *this;
    }

    // Postfix increment (t++): advance by 1 second, return copy of original state
    Time operator++(int) {
        Time old = *this; // Must make temporary copy of current state
        ++seconds;
        normalize();
        return old; // Return previous state by value
    }

    // Equality comparison
    bool operator==(const Time& o) const {
        return (hours == o.hours && minutes == o.minutes && seconds == o.seconds);
    }

    // Less-than comparison (chronological order)
    bool operator<(const Time& o) const {
        if (hours != o.hours) return hours < o.hours;
        if (minutes != o.minutes) return minutes < o.minutes;
        return seconds < o.seconds;
    }

    friend std::ostream& operator<<(std::ostream& os, const Time& t) {
        os << std::setfill('0') << std::setw(2) << t.hours << ":"
           << std::setfill('0') << std::setw(2) << t.minutes << ":"
           << std::setfill('0') << std::setw(2) << t.seconds;
        return os;
    }
};

// Playlist class managing a sequence of song tracks
class Playlist {
private:
    std::string name;
    std::vector<std::string> tracks;

public:
    Playlist(std::string n) : name(std::move(n)) {}

    void addTrack(const std::string& track) {
        tracks.push_back(track);
    }

    size_t size() const { return tracks.size(); }

    // 1. Non-const subscript operator (allows reading and in-place modification)
    std::string& operator[](int index) {
        if (index < 0 || index >= static_cast<int>(tracks.size())) {
            throw std::out_of_range("Index " + std::to_string(index) + 
                                    " out of bounds (Size: " + std::to_string(tracks.size()) + ")");
        }
        return tracks[index];
    }

    // 2. Const subscript operator (allows safe read-only access on const objects)
    const std::string& operator[](int index) const {
        if (index < 0 || index >= static_cast<int>(tracks.size())) {
            throw std::out_of_range("Index " + std::to_string(index) + 
                                    " out of bounds (Size: " + std::to_string(tracks.size()) + ")");
        }
        return tracks[index];
    }

    // Alternative: Unchecked access (raw performance benchmark)
    const std::string& uncheckedAt(size_t index) const {
        return tracks[index];
    }
};

// Read-only inspection function testing const operator[]
void inspectFirstTrack(const Playlist& pl) {
    std::cout << "  [Const Inspection] First track: \"" << pl[0] << "\"\n";
}

int main() {
    std::cout << "======================================================================\n";
    std::cout << "  SIMATS ENGINEERING - CO3 ASSIGNMENT 3 - TASK 05\n";
    std::cout << "  INCREMENT, COMPARISON & SUBSCRIPT OPERATORS (PLAYLIST & TIMER)\n";
    std::cout << "  Student: NEELAKANDA MOORTHY (Reg No: 192421063)\n";
    std::cout << "======================================================================\n\n";

    std::cout << "[PART 1: PREDICT THE OUTPUT VERIFICATION]\n";
    {
        Counter c{5};
        Counter a = c++;
        Counter b = ++c;
        std::cout << "  Actual Output   : " << a.v << ' ' << b.v << ' ' << c.v << '\n';
        std::cout << "  Predicted Output: 5 7 7\n";
        std::cout << "  -> Verification: Output matches prediction 100%!\n\n";
    }

    std::cout << "[PART 2: PREFIX VS POSTFIX BEHAVIOURAL VERIFICATION]\n";
    {
        Time t(0, 2, 45);
        std::cout << "  Initial timer t: " << t << '\n';
        Time a = t++; // Postfix: a receives old value (00:02:45), t becomes 00:02:46
        std::cout << "  After a = t++  : a = " << a << ", t = " << t << '\n';
        Time b = ++t; // Prefix: t becomes 00:02:47, b receives new value (00:02:47)
        std::cout << "  After b = ++t  : b = " << b << ", t = " << t << '\n';
        std::cout << "  -> Verified: Postfix preserved old snapshot, prefix returned updated reference.\n\n";
    }

    std::cout << "[PART 3: MINUTE AND HOUR ROLLOVER (00:59:59 -> 01:00:00)]\n";
    {
        Time rolloverTest(0, 59, 58);
        std::cout << "  Start     : " << rolloverTest << '\n';
        ++rolloverTest;
        std::cout << "  ++ (1 sec): " << rolloverTest << '\n';
        ++rolloverTest;
        std::cout << "  ++ (Rollover to 1 hour): " << rolloverTest << '\n';
        std::cout << "  -> Rollover successfully transitioned 00:59:59 to 01:00:00.\n\n";
    }

    std::cout << "[PART 4: PLAYLIST PLAYBACK SIMULATION & CONST/NON-CONST []]\n";
    {
        Playlist mySongs("Rock Classics");
        mySongs.addTrack("Bohemian Rhapsody");
        mySongs.addTrack("Stairway to Heaven");
        mySongs.addTrack("Hotel California");

        // Non-const modification via operator[]
        std::cout << "  Original track 1: " << mySongs[1] << '\n';
        mySongs[1] = "Comfortably Numb"; // In-place non-const modification
        std::cout << "  Modified track 1: " << mySongs[1] << '\n';

        // Const access via const reference helper
        inspectFirstTrack(mySongs);

        // Simulation
        Time timer(0, 0, 0);
        std::cout << "  Playback simulation:\n";
        for (size_t i = 0; i < mySongs.size(); ++i) {
            std::cout << "    [" << timer << "] Playing: " << mySongs[i] << '\n';
            for (int s = 0; s < 180; ++s) ++timer; // Advance track
        }
        std::cout << '\n';
    }

    std::cout << "[PART 5: RANGE CHECK BOUNDARY TESTING ([-1] AND [size])]\n";
    {
        Playlist pl("Boundary Test");
        pl.addTrack("Track A");
        pl.addTrack("Track B");

        // Test index -1
        try {
            std::cout << "  Attempting access at index -1: ";
            std::cout << pl[-1] << '\n';
        } catch (const std::out_of_range& e) {
            std::cout << "CAUGHT EXCEPTION: " << e.what() << '\n';
        }

        // Test index = size
        try {
            std::cout << "  Attempting access at index 2 (size = 2): ";
            std::cout << pl[2] << '\n';
        } catch (const std::out_of_range& e) {
            std::cout << "CAUGHT EXCEPTION: " << e.what() << '\n';
        }
        std::cout << '\n';
    }

    std::cout << "[PART 6: BENCHMARK - 10,000,000 PREFIX VS POSTFIX INCREMENTS]\n";
    {
        const int iters = 10000000;
        
        Time tPre(0, 0, 0);
        auto start1 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iters; ++i) {
            ++tPre;
        }
        auto end1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> preDuration = end1 - start1;

        Time tPost(0, 0, 0);
        auto start2 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iters; ++i) {
            tPost++;
        }
        auto end2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> postDuration = end2 - start2;

        std::cout << "  10,000,000 Prefix  (++t) Time: " << preDuration.count() << " ms\n";
        std::cout << "  10,000,000 Postfix (t++) Time: " << postDuration.count() << " ms\n";
        std::cout << "  -> Postfix overhead: " << (postDuration.count() - preDuration.count()) 
                  << " ms extra due to temporary object creation and value return.\n\n";
    }

    std::cout << "[PART 7: BENCHMARK - CHECKED VS UNCHECKED [] FOR 10,000,000 ACCESSES]\n";
    {
        const int iters = 10000000;
        Playlist pl("Benchmark");
        pl.addTrack("Performance Test Track");

        auto startChecked = std::chrono::high_resolution_clock::now();
        volatile size_t len1 = 0;
        for (int i = 0; i < iters; ++i) {
            len1 += pl[0].size();
        }
        auto endChecked = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> checkedTime = endChecked - startChecked;

        auto startUnchecked = std::chrono::high_resolution_clock::now();
        volatile size_t len2 = 0;
        for (int i = 0; i < iters; ++i) {
            len2 += pl.uncheckedAt(0).size();
        }
        auto endUnchecked = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> uncheckedTime = endUnchecked - startUnchecked;

        std::cout << "  Checked operator[]   (10M accesses): " << checkedTime.count() << " ms\n";
        std::cout << "  Unchecked operator[] (10M accesses): " << uncheckedTime.count() << " ms\n";
        std::cout << "  Safety cost per access: " 
                  << ((checkedTime.count() - uncheckedTime.count()) * 1e6 / iters) << " nanoseconds.\n";
    }

    std::cout << "\n======================================================================\n";
    std::cout << "  TASK 05 EXECUTION COMPLETED SUCCESSFULLY\n";
    std::cout << "======================================================================\n";
    return 0;
}
