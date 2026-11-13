# C++ Object-Oriented Programming Assessments (CO1, CO2 & CO3)

## Student Information
| Field | Details |
|---|---|
| **Name** | NEELAKANDA MOORTHY |
| **Register Number** | 192421063 |
| **Institution** | Saveetha Institute of Medical and Technical Sciences (SIMATS Engineering) |
| **Compiler** | MinGW-W64 GCC 8.1.0 (x86_64-posix-seh-rev0) & GCC 16.2.0 (Debian/WSL AddressSanitizer) |
| **Compilation Command** | `g++ -std=c++17 -Wall -Wextra <filename>.cpp -o <filename>.exe` |
| **GitHub Repository** | [https://github.com/moorthy-dnkkw/DSA0119-C-](https://github.com/moorthy-dnkkw/DSA0119-C-) |

---

## Directory Structure

```
C++ ASSESSMENT/
├── ASSESSMENT 1/                       # CO1: OOP Constructs, Tokens & Fare System
│   ├── ASSIGNMENT_REPORT.md            # Comprehensive Markdown Report
│   ├── ASSIGNMENT_REPORT.pdf           # 13-Page Comprehensive PDF Report
│   └── Task01/ to Task12/              # Task 1 to Task 12 (Source + Output)
│
├── ASSESSMENT 2/                       # CO2: Functions, Classes & OOP Concepts
│   ├── ASSIGNMENT_REPORT.md            # Comprehensive Markdown Report (CO2)
│   ├── ASSIGNMENT_REPORT.pdf           # 9-Page Comprehensive PDF Report (CO2)
│   └── Task01/ to Task07/              # Q1 to Q7 (Source + Output)
│
└── ASSESSMENT 3/                       # CO3: Constructors, Destructors & Operator Overloading
    ├── ASSIGNMENT_REPORT.md            # Comprehensive Markdown Report (CO3)
    ├── ASSIGNMENT_REPORT.pdf           # Executive PDF Report (CO3)
    ├── Task01/                         # Q1: Constructors, Delegating Ctors & Explicit (6 Marks)
    │   ├── task01.cpp
    │   ├── output.txt
    │   ├── compiler_error_no_default.txt
    │   └── compiler_warning_reorder.txt
    ├── Task02/                         # Q2: Shallow vs Deep Copies & Rule of Three (7 Marks)
    │   ├── task02.cpp
    │   ├── output.txt
    │   ├── asan_shallow_report.txt     # AddressSanitizer double-free trace
    │   └── asan_deep_report.txt        # Clean ASan report
    ├── Task03/                         # Q3: Dynamic Ctors/Destructors & RAII (7 Marks)
    │   ├── task03.cpp
    │   ├── output.txt
    │   ├── asan_leak_report.txt        # LeakSanitizer missing delete trace
    │   └── asan_clean_report.txt       # Clean ASan report
    ├── Task04/                         # Q4: Arithmetic & Stream Operators (+, +=, <<, >>) (7 Marks)
    │   ├── task04.cpp
    │   └── output.txt                  # 1M additions IEEE 754 precision drift benchmark
    ├── Task05/                         # Q5: Increment, Comparison & Subscript Operators (6 Marks)
    │   ├── task05.cpp
    │   ├── output.txt
    │   ├── output_O0.txt               # -O0 benchmark (prefix vs postfix +38.5% copy overhead)
    │   └── output_O2.txt               # -O2 optimized benchmark
    ├── Task06/                         # Q6: Rules of Operator Overloading & Text Class (7 Marks)
    │   ├── task06.cpp
    │   ├── output.txt
    │   └── asan_report.txt             # ASan 0-leak clean verification
    └── Task07/                         # Q7: First-Principles Complex Class & AC Calculator (10 Marks)
        ├── task07.cpp
        └── output.txt                  # Series/Parallel RLC calculations & hand match
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

## Assessment 3 (CO3) Summary

| Task | Topic | Marks | Bloom's Level | Key Verification & Results |
|---|---|---|---|---|
| **Q1 (Task01)** | Constructors & Delegating Ctors | 6 | K5 – Evaluate | Delegating ctors; negative copies clamped to 0; `-Wreorder` diagnostic evaluated; `explicit` keyword |
| **Q2 (Task02)** | Copy Ctors: Shallow vs Deep | 7 | K5 – Evaluate | Rule of 3; deep string array copy; AddressSanitizer double-free trace recorded; Rule of Zero vector |
| **Q3 (Task03)** | Dynamic Ctors & Destructors | 7 | K5 – Evaluate | 2D dynamic seat map; RAII cleanup; Predict trace: `+A +B -B copyAc use -Ac +P -P end -A` (100% match); ASan leak trace |
| **Q4 (Task04)** | Arithmetic & Stream Operators | 7 | K5 – Evaluate | `+`, `-`, `+=`, `==`, `<<`; 1M additions drift: `double` drifted by `0.0000013329 Rs` vs `Money` 0 drift |
| **Q5 (Task05)** | Increment, Comparison, Subscript | 6 | K5 – Evaluate | Postfix dummy `int`; Predict trace: `5 7 7` (100% match); 10M benchmark (-O0: +38.5% postfix copy overhead; -O2: 0 ms); `std::out_of_range` safe |
| **Q6 (Task06)** | Rules of Operator Overloading | 7 | K5 – Evaluate | Non-overloadable tokens; SMS gateway `Text` class; self-assignment safe; ASan clean; 50K benchmark vs `std::string` |
| **Q7 (Task07)** | Complex Class & AC Calculator | 10 | K5 – Evaluate | 4-Stage first principles; Series ($50 - 271.1860j\,\Omega$, $\|Z\|=275.7569\,\Omega$) and Parallel ($27.5159 + 24.8731j\,\Omega$, $\|Z\|=37.0917\,\Omega$) matched hand calc; div-by-zero exception safe |
| **TOTAL** | | **50** | | **All 7 Tasks 100% Verified with zero errors** |

---

## How to Compile and Run

### Prerequisites
- MinGW-W64 GCC 8.1.0+ (with `-std=c++17`)
- AddressSanitizer supported GCC (GCC 16.2.0 in Debian/WSL or Linux)
- Windows PowerShell

### Example: Running Any Task from Assessment 3
```powershell
# Set compiler on PATH if needed
$env:PATH = "C:\Program Files\CodeBlocks\MinGW\bin;" + $env:PATH

# Navigate to desired task folder
cd "C++ ASSESSMENT\ASSESSMENT 3\Task01"

# Compile with strict warnings
g++ -std=c++17 -Wall -Wextra task01.cpp -o task01.exe

# Execute
.\task01.exe
```

### Running with AddressSanitizer (WSL)
```bash
cd "C++ ASSESSMENT/ASSESSMENT 3/Task02"
g++ -std=c++17 -fsanitize=address -g task02.cpp -o task02_asan
./task02_asan
```

---

## Verification & Deliverables
- **Markdown Reports:**
  - `C++ ASSESSMENT/ASSESSMENT 1/ASSIGNMENT_REPORT.md`
  - `C++ ASSESSMENT/ASSESSMENT 2/ASSIGNMENT_REPORT.md`
  - `C++ ASSESSMENT/ASSESSMENT 3/ASSIGNMENT_REPORT.md`
- **PDF Reports:**
  - `C++ ASSESSMENT/ASSESSMENT 1/ASSIGNMENT_REPORT.pdf` (13 Pages)
  - `C++ ASSESSMENT/ASSESSMENT 2/ASSIGNMENT_REPORT.pdf` (9 Pages)
  - `C++ ASSESSMENT/ASSESSMENT 3/ASSIGNMENT_REPORT.pdf` (Executive Report)
- **Progressive Git Commits:** Every task compiled, tested, and committed sequentially.
