# C++ Object-Oriented Programming Assessments (CO1 & CO2)

## Student Information
| Field | Details |
|---|---|
| **Name** | NEELAKANDA MOORTHY |
| **Register Number** | 192421063 |
| **Institution** | Saveetha Institute of Medical and Technical Sciences (SIMATS Engineering) |
| **Compiler** | MinGW-W64 GCC 8.1.0 (x86_64-posix-seh-rev0) |
| **Compilation Command** | `g++ -std=c++17 -Wall -Wextra <filename>.cpp -o <filename>.exe` |
| **GitHub Repository** | [https://github.com/moorthy-dnkkw/DSA0119-C-](https://github.com/moorthy-dnkkw/DSA0119-C-) |

---

## Directory Structure

```
C++ ASSESSMENT/
├── ASSESSMENT 1/                       # CO1: OOP Constructs, Tokens & Fare System
│   ├── ASSIGNMENT_REPORT.md            # Comprehensive Markdown Report
│   ├── ASSIGNMENT_REPORT.pdf           # 13-Page Comprehensive PDF Report
│   ├── Task01/ to Task12/              # Task 1 to Task 12 (Source + Output)
│   │   ├── task01.cpp ... task12.cpp
│   │   └── output.txt ...
│
└── ASSESSMENT 2/                       # CO2: Functions, Classes & OOP Concepts
    ├── ASSIGNMENT_REPORT.md            # Comprehensive Markdown Report (CO2)
    ├── ASSIGNMENT_REPORT.pdf           # 9-Page Comprehensive PDF Report (CO2)
    ├── Task01/                         # Q1: Call by Value, Reference, Pointer (6 Marks)
    │   ├── task01.cpp
    │   └── output.txt
    ├── Task02/                         # Q2: Return by Reference & Dangers (4 Marks)
    │   ├── task02.cpp
    │   └── output.txt
    ├── Task03/                         # Q3: Default Arguments & Overloading (6 Marks)
    │   ├── task03.cpp
    │   └── output.txt
    ├── Task04/                         # Q4: Specifying Class with Internal Array (6 Marks)
    │   ├── task04.cpp
    │   └── output.txt
    ├── Task05/                         # Q5: Static Members & Arrays of Objects (7 Marks)
    │   ├── task05.cpp
    │   └── output.txt
    ├── Task06/                         # Q6: Friend Functions & Inline Functions (8 Marks)
    │   ├── task06.cpp
    │   └── output.txt
    └── Task07/                         # Q7: Day-4 Design Challenge (13 Marks)
        ├── task07.cpp
        └── output.txt
```

---

## Assessment 2 (CO2) Summary

| Task | Topic | Marks | Bloom's Level | Key Verification & Results |
|---|---|---|---|---|
| **Q1 (Task01)** | Call by Value, Ref, Pointer | 6 | K5 – Evaluate | Predict: `1 12 13`, `1 100`, `16 13 6` (Exact match). Concession: `45000` -> `40500` |
| **Q2 (Task02)** | Return by Ref & Dangers | 4 | K5 – Evaluate | LHS Moderation `+= 2` updates 91->93; GCC warning `-Wreturn-local-addr` verified; vector realloc hazard |
| **Q3 (Task03)** | Default Args & Overloading | 6 | K5 – Evaluate | Ambiguity explained & resolved; 6 prediction tests match compiler resolution (`2` vs `2.0f`) |
| **Q4 (Task04)** | Class with Internal Array | 6 | K5 – Evaluate | SavingsAccount ATM mini-statement; scope resolution `::`; Shift $O(k)$ vs Circular $O(1)$ |
| **Q5 (Task05)** | Static Members & Arrays | 7 | K5 – Evaluate | Predict: `101 103 104 4` (Exact match). BusTicket `trip[4]` allocation and static lifetime persistence |
| **Q6 (Task06)** | Friend & Inline Functions | 8 | K5 – Evaluate | Symmetric binary ops; `departure(9,45)` to `arrival(13,10)` = `205 false`; 10M call benchmark: 15.52 ms |
| **Q7 (Task07)** | Day-4 Design Challenge | 13 | K5 – Evaluate | Diagnose $O(K)$ array shift; redesign $O(1)$ ring buffer; 1M txns benchmark: **2.66x speedup** |
| **TOTAL** | | **50** | | **All 7 Tasks 100% Verified with zero errors** |

---

## How to Compile and Run

### Prerequisites
- MinGW-W64 GCC 8.1.0+ (with `-std=c++17`)
- Windows PowerShell

### Example: Running Any Task from Assessment 2
```powershell
# Set compiler on PATH if needed
$env:PATH = "C:\Program Files\CodeBlocks\MinGW\bin;" + $env:PATH

# Navigate to desired task folder
cd "C++ ASSESSMENT\ASSESSMENT 2\Task01"

# Compile with strict warnings
g++ -std=c++17 -Wall -Wextra task01.cpp -o task01.exe

# Execute
.\task01.exe
```

---

## Verification & Deliverables
- **Markdown Reports:**
  - `C++ ASSESSMENT/ASSESSMENT 1/ASSIGNMENT_REPORT.md`
  - `C++ ASSESSMENT/ASSESSMENT 2/ASSIGNMENT_REPORT.md`
- **PDF Reports:**
  - `C++ ASSESSMENT/ASSESSMENT 1/ASSIGNMENT_REPORT.pdf` (13 Pages)
  - `C++ ASSESSMENT/ASSESSMENT 2/ASSIGNMENT_REPORT.pdf` (9 Pages)
- **Progressive Git Commits:** Every task compiled, tested, and committed sequentially.
