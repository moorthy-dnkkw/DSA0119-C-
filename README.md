# DSA01 - Assignment 1: Namma Metro Smart-Card Fare System

## Student Information
| Field | Details |
|---|---|
| **Name** | NEELAKANDA MOORTHY |
| **Register Number** | 192421063 |
| **Course** | CO1 – Object-Oriented Programming in C++ |
| **Assignment** | Design, Implement and Validate the Fare Logic of a City Metro Smart-Card System |
| **Compiler** | MinGW-W64 GCC 8.1.0 (x86_64-posix-seh-rev0) |
| **Compile Command** | `g++ -std=c++17 -Wall -Wextra <filename>.cpp -o <filename>.exe` |
| **GitHub Repository** | [https://github.com/moorthy-dnkkw/DSA0119-C-](https://github.com/moorthy-dnkkw/DSA0119-C-) |

---

## Directory Structure

```
C++ ASSESSMENT/
└── ASSESSMENT 1/
    ├── Task01/        # Object-Oriented Analysis
    │   ├── task01.cpp
    │   └── output.txt
    ├── Task02/        # Tokens, Keywords, Identifiers & Constants
    │   ├── task02.cpp
    │   └── output.txt
    ├── Task03/        # Data Types & Type Compatibility
    │   ├── task03.cpp
    │   └── output.txt
    ├── Task04/        # Implicit Conversions (Predict-the-Output)
    │   ├── task04.cpp
    │   └── output.txt
    ├── Task05/        # Operators & Precedence (Predict-the-Output)
    │   ├── task05.cpp
    │   └── output.txt
    ├── Task06/        # Control Structures (Complete-the-Code)
    │   ├── task06.cpp
    │   └── output.txt
    ├── Task07/        # Procedural vs Object-Oriented
    │   ├── task07.cpp
    │   └── output.txt
    ├── Task08/        # Safe I/O & Formatted Reporting
    │   ├── task08.cpp
    │   ├── input.txt
    │   └── output.txt
    ├── Task09/        # Fare Logic Implementation & 13 Automated Tests
    │   ├── task09.cpp
    │   └── output.txt
    ├── Task10/        # Scale Analysis: int vs long long vs double
    │   ├── task10.cpp
    │   └── output.txt
    ├── Task11/        # Final Design Recommendation
    │   ├── task11.cpp
    │   └── output.txt
    └── Task12/        # Day-4 Design Challenge (Airport, Weekend, Monthly Pass)
        ├── task12.cpp
        └── output.txt
```

---

## How to Compile and Run

### Prerequisites
- MinGW-W64 GCC 8.1.0+ (or any C++17-compatible compiler)
- Windows 10/11 (PowerShell)

### Steps for Each Task

```bash
# Navigate to the task folder
cd "C++ ASSESSMENT/ASSESSMENT 1/Task01"

# Compile with C++17 and warnings enabled
g++ -std=c++17 -Wall -Wextra task01.cpp -o task01.exe

# Run
./task01.exe
```

Repeat for Task02 through Task12 by changing the folder and filename.

---

## Task Summary

| Task | Title | Marks | Key Result |
|------|-------|-------|------------|
| 1 | Object-Oriented Analysis | 4 | Domain modeling: SmartCard, Journey, FareTable, Zone classes identified with invariant rules |
| 2 | Tokens & Constants | 3 | constexpr fare table as single source of truth; enum class Zone for type safety |
| 3 | Data Types & Compatibility | 4 | int overflows at 3M journeys (7.5B paise > INT_MAX 2.1B); long long required; double drifts by ~0.16 paise over 100K additions |
| 4 | Implicit Conversions | 4 | Predict-the-output verified: integer division, char promotion, bool narrowing, signed-unsigned comparison trap |
| 5 | Operators & Precedence | 4 | Predict-the-output verified: x++ + ++y = 9, negative division truncates toward zero |
| 6 | Control Structures | 4 | Completed switch-based fare function; boundary tests at 10:00 AM and 8:00 PM confirmed off-peak |
| 7 | Procedural vs OOP | 4 | Demonstrated: procedural allows negative balance corruption; OOP SmartCard prevents it at compile time |
| 8 | Safe I/O | 4 | Stream error recovery with cin.clear()/cin.ignore(); formatted audit report with iomanip |
| 9 | Implementation & Testing | 4 | 13/13 automated tests PASS + hand-calculated 5-journey trace matches exactly (4200 paise) |
| 10 | Expectation vs Execution | 4 | 32-bit int overflows at network scale; double accumulates Rs 0.003 drift over 3M journeys |
| 11 | Final Recommendation | 4 | Production architecture: constexpr array + SmartCard + FareEngine; fare revision = 1 line change |
| 12 | Day-4 Design Challenge | 8 | 4th zone (Airport Rs 60), weekend discount (10%), monthly pass; 16/16 tests PASS |
| **TOTAL** | | **50** | |

---

## Compiler Warnings

All tasks compile with **zero warnings** under `-Wall -Wextra -std=c++17`.
