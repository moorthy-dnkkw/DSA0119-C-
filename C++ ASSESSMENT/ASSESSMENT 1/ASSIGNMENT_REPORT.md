# Assignment 1 Report: Namma Metro Smart-Card Fare System

**Student:** NEELAKANDA MOORTHY | **Register No:** 192421063 | **Compiler:** MinGW-W64 GCC 8.1.0  
**Compile Command:** `g++ -std=c++17 -Wall -Wextra`  
**GitHub:** [https://github.com/moorthy-dnkkw/DSA0119-C-](https://github.com/moorthy-dnkkw/DSA0119-C-)

---

## Task 1 – Understand the Problem and Identify the Objects (4 Marks)

### Objects, Data, and Behaviour

| Object | Data (Attributes) | Behaviour (Methods) |
|--------|-------------------|---------------------|
| **SmartCard** | cardId (string), balancePaise (long long), passengerType (enum) | canTravel(), deduct(), recharge(), getBalance() |
| **Journey** | journeyId, destinationZone, hourOfTravel, fareCharged, approved | execute(SmartCard&), printSummary() |
| **FareTable** | BASE_PAISE[] (constexpr array), PEAK_SURCHARGE_PERCENT, MIN_BALANCE_PAISE | isPeakHour(), calculateFare() |
| **Passenger** | name, age, type (General/Student/Senior) | getConcession() |
| **Zone** | enum values: Inner, Middle, Outer | N/A (type-safe enumeration) |

### OOP Principles in the Metro System

| Principle | Application |
|-----------|-------------|
| **Encapsulation** | SmartCard keeps `balancePaise` private; only `deduct()` and `recharge()` can modify it, preventing negative balances |
| **Abstraction** | FareTable exposes `calculateFare()` hiding surcharge formula internals |
| **Inheritance** | Student/SeniorCitizen passenger types can inherit from base Passenger class |
| **Polymorphism** | Different passenger types override `getConcession()` to return different discount rates |

### Invariant Rules
1. Balance after any journey must remain ≥ Rs 50.00 (5000 paise)
2. Zone values must be within valid range (Inner/Middle/Outer)
3. Recharge amount must be positive (> 0)
4. Peak hours are strictly [8, 10) and [17, 20) — 10:00 is off-peak, 20:00 is off-peak

### Alternative Concept: Procedural vs OOP

| Aspect | Procedural | Object-Oriented |
|--------|-----------|-----------------|
| Time per journey | O(1) | O(1) |
| Memory per card | O(1) | O(1) |
| Lines changed for min-balance update | Every `if` check across all files | 1 line in SmartCard class |
| Risk of balance corruption | HIGH (global variables) | ZERO (private access) |

---

## Task 2 – Use Tokens, Keywords, Identifiers and Constants Correctly (3 Marks)

### Five Token Categories with Examples

| Token Type | Examples from Code |
|------------|-------------------|
| **Keywords** | `enum`, `class`, `constexpr`, `long`, `bool`, `switch`, `case`, `return`, `const`, `static` |
| **Identifiers** | `Zone`, `FareConfig`, `INNER_ZONE_PAISE`, `calculateFare`, `SmartCard` |
| **Constants** | `1000LL`, `20`, `5000LL`, `true`, `"Inner"` |
| **Operators** | `::`, `=`, `+`, `*`, `/`, `==`, `>=`, `&&`, `||`, `<<` |
| **Punctuators** | `{`, `}`, `(`, `)`, `;`, `:`, `,`, `.` |

### Constant Mechanism Comparison

| Feature | `#define` | `const` | `constexpr` |
|---------|-----------|---------|-------------|
| Type Checking | None (text substitution) | Yes | Yes (strict) |
| Scope | Global file scope | Block/Namespace | Block/Namespace |
| Evaluation Time | Preprocessor | Runtime or compile | Guaranteed compile-time |
| Memory | 0 bytes | May allocate .rodata | Zero overhead |
| Debugger Visibility | Invisible | Visible | Visible |

**Verdict:** `constexpr` is preferred — it provides type safety, scope control, compile-time evaluation, and zero runtime overhead.

### enum class vs enum
- `enum class Zone` prevents implicit conversion to `int`, preventing accidental arithmetic
- Scoped: `Zone::Inner` instead of just `Inner`, avoiding namespace pollution

---

## Task 3 – Choose Data Types and Check Type Compatibility (4 Marks)

### Data Type Selection Table

| Value | Type Chosen | sizeof | Justification |
|-------|------------|--------|---------------|
| Fares (paise) | `long long` | 8 bytes | Network-scale accumulation exceeds INT_MAX |
| Balances (paise) | `long long` | 8 bytes | Same as fares |
| Hour of travel | `int` | 4 bytes | Range 0–23, well within int range |
| Zone number | `enum class : int` | 4 bytes | Type-safe scoped enumeration |
| Journey count | `long long` | 8 bytes | Up to 3,000,000 per day |
| Peak flag | `bool` | 1 byte | Binary true/false |

### Why Integer Paise, Not Double Rupees
- `double` cannot exactly represent Rs 10.10 in binary (IEEE 754)
- Over 100,000 additions of Rs 10.10: double drifts by ~0.00016 Rupees
- `long long` integer paise: EXACT arithmetic, zero cumulative error

### Network Scale Overflow Proof
- 3,000,000 journeys × Rs 25 avg = Rs 75,000,000 = 7,500,000,000 paise
- `INT_MAX` = 2,147,483,647 paise → **OVERFLOW by 5.35 billion paise!**
- `long long` (`LLONG_MAX` = 9.2 × 10^18) handles years of network data

---

## Task 4 – Control Implicit Conversions (4 Marks)

### Predicted Output (Written Before Running)

```
3 3.5
C 68
1 true
s >= u
```

### Actual Output (Matches Prediction)

```
3 3.5
C 68
1 true
s >= u
```

### Line-by-Line Conversion Analysis

| Line | Code | Conversion | Explanation |
|------|------|-----------|-------------|
| 1 | `a / b` → `c` | Integer division, int→double | 7/2 = 3 (truncated), then widened to 3.0 |
| 1 | `static_cast<double>(a) / b` | Explicit cast, int→double promotion | 7.0 / 2.0 = 3.5 (exact) |
| 2 | `'A' + 2` → `ch` | Integral promotion, int→char narrowing | 65 + 2 = 67 ('C'), narrowed back to char |
| 2 | `ch + 1` | Integral promotion char→int | 'C' (67) + 1 = 68 (printed as integer) |
| 3 | `bool flag = 5` | int→bool | 5 ≠ 0, so flag = true (1) |
| 4 | `s < u` | signed→unsigned conversion | -1 becomes 4,294,967,295u; 4294967295 < 1 is FALSE |

### Prevention Techniques
- **Brace initialization:** `int x{3.7};` → compile-time ERROR (narrowing detected)
- **static_cast:** Makes intent explicit, aids code review
- **Same-type comparisons:** Avoid mixing signed and unsigned

---

## Task 5 – Write Correct Expressions with the Right Precedence (4 Marks)

### Predicted Output (Written Before Running)

```
11 16
9 6 4
6 4
3 1 -3 -1
18
```

### Actual Output (Matches Prediction)

```
11 16
9 6 4
6 4
3 1 -3 -1
18
```

### Operator-by-Operator Breakdown

| Expression | Operators | Precedence Rule | Result |
|-----------|-----------|----------------|--------|
| `x + y * 2` | `*` (P5) > `+` (P6) | Multiplication first | 5 + 6 = 11 |
| `(x + y) * 2` | Parentheses override | Addition first | 8 × 2 = 16 |
| `x++ + ++y` | Postfix `++` (P2), Prefix `++` (P3), `+` (P6) | x→5 then 6, y→4 | 5 + 4 = 9 |
| `x % 4 * 2` | `%` and `*` same precedence (P5), L→R | (6 % 4) × 2 | 2 × 2 = 4 |
| `-10 / 3` | Truncation toward zero (C++11) | −10 ÷ 3 = −3.33… → −3 | −3 |
| `-10 % 3` | Identity: `a == (a/b)*b + a%b` | −10 − (−3 × 3) = −1 | −1 |
| `for` loop | `i = 0, 3, 6, 9` | k = 0 + 3 + 6 + 9 | 18 |

### Alternative: Lookup Table vs Boolean Expression
- `bool peak[24]` array: O(1) with single array access (no branches)
- `(h >= 8 && h < 10) || ...`: O(1) with 2–4 comparisons (potential branch mispredictions)
- Extra space: 24 bytes (negligible)

---

## Task 6 – Design the Control Structures (4 Marks)

### SIMATS Pseudocode

```
ALGORITHM CalculateFare(zone, isPeak)
INPUT:  zone of type Zone, isPeak of type Boolean
OUTPUT: finalFare in integer paise
BEGIN
  SWITCH zone DO
    CASE Zone.Inner:  baseFare ← 1000
    CASE Zone.Middle: baseFare ← 2000
    CASE Zone.Outer:  baseFare ← 3000
    DEFAULT:          baseFare ← 0
  END SWITCH
  IF isPeak = TRUE THEN
    finalFare ← baseFare + (baseFare × 20) / 100
  ELSE
    finalFare ← baseFare
  END IF
  RETURN finalFare
END
```

### Completed Code Output (Matches Expected)

```
Fare Rs 10.00 balance Rs 90
Fare Rs 36.00 balance Rs 54
Denied (low balance) balance Rs 54
```

### Boundary Tests
- **10:00 AM:** OFF-PEAK (standard Rs 30 fare) ✓
- **8:00 PM (20:00):** OFF-PEAK (standard Rs 30 fare) ✓

### Alternative: Array Lookup vs Switch
- Array: `FARE[static_cast<int>(z)]` — O(1), direct memory read, trivial to extend
- Switch: O(1) (compiler may optimize to jump table), requires new `case` for each zone
- **For extensibility, array lookup is superior**

---

## Task 7 – From Procedural to Object-Oriented (4 Marks)

### Comparison

| Aspect | Procedural | Object-Oriented |
|--------|-----------|-----------------|
| Balance storage | Global `long long` variable | Private member in SmartCard class |
| Invariant protection | None — any function can set balance to -100 | Enforced by `deduct()` gatekeeper |
| Min balance change | Update every scattered `if` check | Change 1 line: `MIN_BALANCE_PAISE` |
| Time complexity | O(1) | O(1) |
| Space complexity | O(1) | O(1) |

### Key Finding
OOP achieves **complete state protection at zero runtime cost**. The procedural version was demonstrated to allow direct corruption (`globalCardBalance = -2500`), while the OOP version prevents this at compile time.

---

## Task 8 – Read and Print Data Safely (4 Marks)

### Stream Error Recovery Mechanism
1. `cin >> value` fails → `cin.fail()` returns true
2. `cin.clear()` clears the error flags
3. `cin.ignore(numeric_limits<streamsize>::max(), '\n')` discards the corrupt buffer
4. Loop re-prompts the user

### Formatted Output
- `std::setw(N)` — field width
- `std::setfill('0')` — zero-padding for paise
- `std::fixed << std::setprecision(2)` — 2 decimal places for rupees

### Alternative: getline + stoi vs cin >>
| Aspect | `cin >>` | `getline` + `stoi` |
|--------|----------|---------------------|
| Time | O(1) per number | O(L) per line |
| Space | O(1) | Heap-allocated string |
| Error handling | Failbit + clear/ignore | try/catch exception |

---

## Task 9 – Implement and Test the Fare Logic (4 Marks)

### Test Results: 13/13 PASS

| Test | Description | Expected | Actual | Status |
|------|-------------|----------|--------|--------|
| TC-01 | Inner Off-Peak | Rs 10.00 | Rs 10.00 | ✅ PASS |
| TC-02 | Inner Peak | Rs 12.00 | Rs 12.00 | ✅ PASS |
| TC-03 | Middle Off-Peak | Rs 20.00 | Rs 20.00 | ✅ PASS |
| TC-04 | Middle Peak | Rs 24.00 | Rs 24.00 | ✅ PASS |
| TC-05 | Outer Off-Peak | Rs 30.00 | Rs 30.00 | ✅ PASS |
| TC-06 | Outer Peak | Rs 36.00 | Rs 36.00 | ✅ PASS |
| TC-07 | 08:00 Boundary (Peak) | Rs 36.00 | Rs 36.00 | ✅ PASS |
| TC-08 | 10:00 Boundary (Off-Peak) | Rs 30.00 | Rs 30.00 | ✅ PASS |
| TC-09 | 17:00 Boundary (Peak) | Rs 36.00 | Rs 36.00 | ✅ PASS |
| TC-10 | 20:00 Boundary (Off-Peak) | Rs 30.00 | Rs 30.00 | ✅ PASS |
| TC-11 | Leaves exactly Rs 50 | APPROVED | APPROVED | ✅ PASS |
| TC-12 | Leaves Rs 49.99 | DENIED | DENIED | ✅ PASS |
| TC-13 | Invalid Zone | DENIED | DENIED | ✅ PASS |

### Hand-Calculated 5-Journey Trace
- Total approved fares: Rs 12 + Rs 20 + Rs 10 = **Rs 42.00 (4200 paise)**
- Program accumulated: **4200 paise — EXACT MATCH** ✅

---

## Task 10 – Compare Expectations with Execution (4 Marks)

### Scale Simulation Results

| Scale | long long (paise) | double (Rs) | Drift (Rs) | int Overflow? |
|-------|-------------------|-------------|------------|---------------|
| 1 Station (20K) | 40,700,000 | 407,000.00 | ~0.000000 | No |
| 1 Line (500K) | 1,017,500,000 | 10,175,000.00 | ~0.000096 | No |
| Network (3M) | 6,105,000,000 | 61,050,000.00 | ~0.003025 | **YES** |

### Key Findings
1. **32-bit `int` catastrophically overflows** at network scale (6.1B > INT_MAX 2.1B)
2. **`double` accumulates Rs 0.003 drift** over 3M journeys (unacceptable for financial audit)
3. **`long long` is exact** — zero error at any scale

---

## Task 11 – Recommend and Defend the Final Design (4 Marks)

### Recommended Production Architecture

| Component | Choice | Justification |
|-----------|--------|---------------|
| Zone | `enum class : int` | Type-safe, no implicit conversion |
| Fare Table | `constexpr` array | Single revision point, zero overhead |
| Fare Engine | Static pure functions | Stateless, testable, O(1) |
| SmartCard | Private balance + OOP | Invariant: balance ≥ Rs 50 after deduction |
| Accumulator | `long long` (64-bit) | Prevents int overflow at 3M+ scale |
| Currency | Integer paise | Eliminates IEEE 754 rounding |

### Fare Revision: 1 Line Change
To update Inner Zone from Rs 10 to Rs 15: change `FARE_TABLE_PAISE[0]` from `1000` to `1500`.

---

## Task 12 – Day-4 Design Challenge (8 Marks)

### A. Impact Analysis
**Changed Requirements:**
1. Airport zone (Rs 60 / 6000 paise) — add 1 enum value + 1 array element
2. Weekend discount (10%) — add DayOfWeek parameter to FareEngine
3. Monthly pass — new MonthlyPass class + SmartCard integration

**Affected Code:** Zone enum (1 line), fare array (1 line), FareEngine (10 lines), SmartCard (15 lines), new MonthlyPass class (20 lines)

### B. Evaluation of Original Design
- Switch-based fare: easily absorbs 4th case (Airport) ✓
- Weekend discount: breaks simple `if (peak) surcharge` pattern ✗ → needs restructuring
- Monthly pass: requires SmartCard to bypass fare engine entirely ✗ → needs extension
- **Verdict: MODIFY** (retain SmartCard core, redesign FareEngine, add MonthlyPass)

### C. Adaptation
1. **Array-indexed fare table** replaces switch (O(1), extensible)
2. **Priority-based discount logic:** Weekend > Peak > Standard
3. **MonthlyPass class** with `coversZone()` check
4. **SmartCard.processJourney()** checks pass FIRST, then calls FareEngine

### D. Evidence-Based Defence: 16/16 Tests PASS

| Test | Scenario | Expected | Actual | Status |
|------|----------|----------|--------|--------|
| T01–T05 | Original 3 zones (weekday) | Standard/Peak fares | Match | ✅ |
| T06–T07 | Airport zone (weekday) | Rs 60/72 | Match | ✅ |
| T08–T10 | Weekend discount (10% off) | Rs 9/27/54 | Match | ✅ |
| T09 | Weekend + Peak hour | 10% discount ONLY (no surcharge) | Rs 27 | ✅ |
| T11–T12 | Pass-covered zones | Rs 0 (PASS-FREE) | Match | ✅ |
| T13–T14 | Pass uncovered zones | Standard/Peak fare | Match | ✅ |
| T15 | Exact Rs 50 boundary | APPROVED | APPROVED | ✅ |
| T16 | Insufficient balance | DENIED | DENIED | ✅ |

**Complexity:** O(1) per journey, O(Z) space for fare table, O(1) per card.

---

*Report generated for CO1 Assignment 1 — All 12 tasks verified and committed progressively.*
