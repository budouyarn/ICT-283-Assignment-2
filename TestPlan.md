Student: Lim Wen Xin Sherine

## 1. Design chosen (Design A, OLAP-style cells)

`Map<int cell, Bst<MetRecType>>` where `cell = year * 100 + month` (March 2015 -> 201503)

Each (year, month) is one cell of a small "cube" and holds that month's readings in a tree.

* **Map** (own class, `Map.h`): key -> value, no duplicate keys. `Insert`, `Find` (returns a pointer, null if absent),
  `Delete`, `Keys` (returns a `Vector<K>`, so the client never sees `std::map`). That is 4 public methods (Find has a
  const and a non-const form), within the brief's "aim for no more than 4". `Size` was removed because `Keys().Size()` gives the same answer,
  so it was a duplicate of something the Map already offers.
  It wraps `std::map` inside; a second version over `Vector` (`MapVectorVersion.h`) passes the same 31 unit checks (it also has no `Size`),
  which shows the client does not depend on the representation.
* **Bst** (exercise d, `Bst.h`): one tree per (year, month) cell, holding that month's readings ordered by date then time
  (`operator<` on `MetRecType`, `Date`, `Time`).
* **MetStore** loads a `Metlog` into the map, then answers month queries (count, mean, standard deviation for speed and
  temperature) with one `Find` of the cell, and the three sample Pearson correlations (S_T, S_R, T_R) for a month across
  all loaded years by visiting the cells whose key ends in that month.
* **Stats** holds `Mean`, `StdDev`, `Mad` (mean absolute deviation) and `sPCC` (two-pass formula; returns NaN when undefined).
* **Menu**: Option 3 asks for a month only and prints S_T, S_R and T_R. Option 4 asks for a year and writes
  `WindTempSolar.csv` (year, then one line per month: average wind speed (stdev, mad), average temperature (stdev, mad), total solar
  radiation in kWh/m2; N/A when a month has no valid readings). The old whole-file report is now Option 5.
  The stats routines take a `Vector<double>`, so none of them is coupled to the weather data type.

## 2. Why this data structure (advantages, disadvantages, issues)

| | Advantages | Disadvantages / issues |
|---|---|---|
| Map of (year, month) cells | A month query goes straight to one cell (one `Find`, one null check) and reads about 4,000 readings instead of a whole year. A new year file only adds cells. Option 3 is one loop over the keys with one `if`. | The key packs two numbers into one integer (valid for years 1 to 9999 and months 1 to 12), so it must always be built the same way: it is made in one function, `MakeKey`. Option 3 still scans all keys (a few dozen, so cheap). |
| Bst per cell | Readings kept in date/time order, so an in-order traversal gives them chronologically; search is O(log n) when balanced. | A BST cannot hold two equal keys, so a repeated timestamp is dropped (the first is kept). It needs `operator<` on `MetRecType`. **Chronological data builds a chain** (section 3), though a much shorter one per month than per year. |
| Own Map class | The client is independent of `std::map`; the representation can change without touching `MetStore`. | Extra code to write and test; one more layer. |
| Function-pointer traversal | Matches the textbook; the tree stays generic. | A function pointer cannot carry state, so `MetStore` collects through a static `Gatherer` class. That is **not thread-safe** and is a design decision to be able to defend. |

Compared with the earlier layout `Map<year, Bst>` (kept in `bench/MetStoreByYear/` for the comparison), on the synthetic files (3 years, 157,824 readings, `-O0`):

| Measure | By year | By year and month |
|---|---|---|
| `MetStore::Load` | 0.43 s | 0.46 s |
| 12 month queries for one year | 0.029 s | 0.008 s |
| 12 Option 3 queries | 0.075 s | 0.032 s |
| Stress x10 (1,578,240 readings): Load | 4.25 s | 3.75 s |
| Stress x10: month queries | 0.016 s | 0.008 s |
| Stress x10: peak memory | 118 MB | 118 MB |

The by-year layout was no faster than scanning the whole log for a month query, because it walked the entire year's tree. The cell layout is about 3 times faster, for the same memory.

Other decisions to defend:

* sPCC is **NaN** (shown as "not available") when the two lists differ in length, there are fewer than 2 pairs, or a list has zero variance.
* A pair is used only when **both** values in that reading are valid, so missing values never shift the pairing.
* Bst keeps `T &` in the visit function (textbook form). The client could therefore change a value and break the ordering;
  `isValidBst()` detects it. A `const T &` version exists (`Bst_const_version.h`) if you decide to switch.

## 3. Recursive or iterative? Decision supported by testing

I loaded real-looking readings into the tree in three orders and timed a recursive tree (`Bst.h`) against an iterative
one (`bench/BstIter.h`, written only for comparison). 52,560 readings = one year of 10-minute data.
Times are insert time in seconds, built at `-O0` (like a Code::Blocks debug build) unless stated.

| Insert order | Recursive | Iterative |
|---|---|---|
| Balanced (middle first) | 0.038 | 0.034 |
| Shuffled | 0.052 | 0.047 |
| Sorted (chronological), 10,000 | 3.50 | 2.48 |
| Sorted (chronological), 52,560 | **89.19** | **67.35** |
| Sorted (chronological), 52,560, `-O2` | 34.23 | 16.05 |

What this shows:

1. The shape of the tree matters far more than recursion versus iteration. Inserting in date order makes a chain, so
   each insert walks the whole chain (O(n^2)): about 89 s instead of 0.04 s.
2. For a balanced tree the two versions are the same speed (0.038 s against 0.034 s), so speed is not a reason to choose iteration.
3. Recursion depth equals the tree height. On a chain that is the number of readings. With a 1 MB stack (Windows default),
   the recursive tree on sorted data **crashed (stack overflow) at 20,000 and 30,000 readings** and survived 10,000.
   The iterative tree handled 30,000 sorted readings in the same 1 MB stack (21.8 s: slow, but no crash).
4. **Decision:** keep the recursive `Bst` (it matches the textbook and is easier to read) **and** insert each year's
   readings in balanced order: `MetStore::Load` sorts the readings, then inserts the middle one of each month's run first. The height is then
   about log2(n) (about 12 for a month), so there is no stack risk. The iterative tree stays as a tested alternative.
   Say honestly that this decision depends on `MetStore::Load` doing the balancing; if a client inserts sorted data directly, the iterative tree is the safer one.

## 4. Test plan (expected values calculated independently)

The expected values were calculated by a separate Python script (`bench/testplan.py`) that reads the CSV files directly and does not use any of my C++ code. 
You may refer to test plan excel here: ->Test Plan<-

Result on the synthetic files:

| # | Test | Expected (calculated independently) | Actual | Result |
|---|---|---|---|---|
| 1 | Option 1, month 1, year 2014 | Valid readings (N): 4373 / Average wind speed (km/h): 11.17 / Sample standard deviation (km/h): 5.04 | all expected lines present | PASS |
| 2 | Option 1, month 2, year 2015 | Valid readings (N): 3954 / Average wind speed (km/h): 11.28 / Sample standard deviation (km/h): 4.98 | all expected lines present | PASS |
| 3 | Option 1, month 7, year 2015 | Valid readings (N): 4380 / Average wind speed (km/h): 14.05 / Sample standard deviation (km/h): 5.16 | all expected lines present | PASS |
| 4 | Option 1, month 12, year 2016 | Valid readings (N): 4356 / Average wind speed (km/h): 11.29 / Sample standard deviation (km/h): 5.13 | all expected lines present | PASS |
| 5 | Option 1, month 2, year 2016 | Valid readings (N): 4090 / Average wind speed (km/h): 11.43 / Sample standard deviation (km/h): 5.01 | all expected lines present | PASS |
| 6 | Option 1, month 6, year 2014 | Valid readings (N): 4232 / Average wind speed (km/h): 13.74 / Sample standard deviation (km/h): 5.10 | all expected lines present | PASS |
| 7 | Option 1, month 1, year 2017 | No valid wind speed readings for that month. | all expected lines present | PASS |
| 8 | Option 2, month 1, year 2014 | Valid readings (N): 4427 / Average temperature (degrees C): 30.16 / Sample standard deviation (degrees C): 2.95 | all expected lines present | PASS |
| 9 | Option 2, month 2, year 2015 | Valid readings (N): 3994 / Average temperature (degrees C): 29.39 / Sample standard deviation (degrees C): 2.98 | all expected lines present | PASS |
| 10 | Option 2, month 7, year 2015 | Valid readings (N): 4427 / Average temperature (degrees C): 18.32 / Sample standard deviation (degrees C): 2.94 | all expected lines present | PASS |
| 11 | Option 2, month 12, year 2016 | Valid readings (N): 4425 / Average temperature (degrees C): 29.42 / Sample standard deviation (degrees C): 2.96 | all expected lines present | PASS |
| 12 | Option 2, month 2, year 2016 | Valid readings (N): 4127 / Average temperature (degrees C): 29.34 / Sample standard deviation (degrees C): 3.00 | all expected lines present | PASS |
| 13 | Option 2, month 6, year 2014 | Valid readings (N): 4278 / Average temperature (degrees C): 19.15 / Sample standard deviation (degrees C): 3.03 | all expected lines present | PASS |
| 14 | Option 2, month 1, year 2017 | No valid temperature readings for that month. | all expected lines present | PASS |
| 15 | Option 3, month 1 | Sample Pearson Correlation Coefficient for January / S_T: 0.38 / S_R: 0.39 / T_R: 0.87 | all expected lines present | PASS |
| 16 | Option 3, month 2 | Sample Pearson Correlation Coefficient for February / S_T: 0.37 / S_R: 0.38 / T_R: 0.86 | all expected lines present | PASS |
| 17 | Option 3, month 6 | Sample Pearson Correlation Coefficient for June / S_T: 0.37 / S_R: 0.40 / T_R: 0.86 | all expected lines present | PASS |
| 18 | Option 3, month 12 | Sample Pearson Correlation Coefficient for December / S_T: 0.37 / S_R: 0.39 / T_R: 0.86 | all expected lines present | PASS |
| 23 | Option 5 (whole file) | Readings with a missing wind speed: 3168 / The number of valid wind speed readings(N): 154656 / The mean wind speed (km/h): 12.53 / The sample standard deviation(S) (km/h): 5.18 | all expected lines present | PASS |
| 24 | Bad input: choice 9, then 'abc', then exit | Please enter a whole number from 0 to 5. | all expected lines present | PASS |
| 25 | Bad input: Option 3 with month 13, then month 3 | Please enter a whole number from 1 to 12. / Sample Pearson Correlation Coefficient for March | all expected lines present | PASS |
| 26 | Option 1 with year 0, then year 2014 | Please enter a whole number from 1 to 9999. / Valid readings (N): 4373 | all expected lines present | PASS |
| 27 | Input ends without choosing Exit |  | all expected lines present | PASS |
| 19 | Option 4, year 2014: WindTempSolar.csv | 2014 / January,11.17(5.04, 4.05),30.16(2.95, 2.54),150.78 / February,11.32(5.15, 4.13),29.38(3.01, 2.58),132.42 / March,11.80(5.00, 4.01),27.25(3.07, 2.60),132.99 / April,12.47(5.07, 4.05),24.26(3.08, 2.60),109.24 / May,13.12(5.17, 4.16),21.33(3.05, 2.59),93.45 / June,13.74(5.10, 4.07),19.15(3.03, 2.59),75.83 / July,13.92(5.02, 4.05),18.29(2.97, 2.55),73.27 / August,13.85(5.08, 4.06),19.12(2.98, 2.56),78.25 / September,13.30(5.06, 4.05),21.27(3.04, 2.58),90.54 / October,12.55(5.08, 4.06),24.25(3.12, 2.62),113.40 / November,11.82(5.03, 4.03),27.20(3.03, 2.56),128.20 / December,11.39(5.08, 4.07),29.39(2.98, 2.55),148.32 | all expected lines present | PASS |
| 20 | Option 4, year 2015: WindTempSolar.csv | 2015 / January,11.22(5.13, 4.13),30.15(2.93, 2.52),152.61 / February,11.28(4.98, 4.02),29.39(2.98, 2.56),131.73 / March,11.85(5.13, 4.12),27.26(3.06, 2.58),132.84 / April,12.42(5.09, 4.07),24.32(3.09, 2.61),107.60 / May,13.18(5.10, 4.12),21.31(3.06, 2.58),93.64 / June,13.76(5.14, 4.10),19.10(2.99, 2.56),75.39 / July,14.05(5.16, 4.15),18.32(2.94, 2.54),73.02 / August,13.61(5.14, 4.12),19.10(2.99, 2.56),78.61 / September,13.21(5.00, 4.00),21.27(3.06, 2.60),88.33 / October,12.49(5.15, 4.12),24.22(3.07, 2.60),112.94 / November,11.79(5.06, 4.05),27.23(3.06, 2.59),128.63 / December,11.26(5.03, 4.04),29.37(2.96, 2.51),146.34 | all expected lines present | PASS |
| 21 | Option 4, year 2016: WindTempSolar.csv | 2016 / January,11.11(5.03, 4.05),30.17(2.96, 2.55),151.99 / February,11.43(5.01, 4.03),29.34(3.00, 2.57),137.62 / March,11.89(5.13, 4.12),27.16(3.08, 2.61),133.11 / April,12.39(5.07, 4.05),24.24(3.10, 2.62),108.70 / May,13.26(5.04, 4.02),21.24(3.04, 2.57),92.13 / June,13.63(5.07, 4.08),19.09(2.98, 2.55),75.47 / July,13.96(5.19, 4.15),18.28(2.94, 2.52),73.34 / August,13.82(5.14, 4.12),19.15(3.01, 2.57),78.00 / September,13.25(5.20, 4.17),21.37(3.05, 2.59),90.57 / October,12.53(5.14, 4.13),24.38(3.07, 2.59),113.28 / November,11.81(4.99, 4.01),27.33(3.01, 2.56),126.41 / December,11.29(5.13, 4.14),29.42(2.96, 2.54),146.44 | all expected lines present | PASS |
| 22 | Option 4, year 2017: WindTempSolar.csv | 2017 / January,N/A,N/A,N/A / February,N/A,N/A,N/A / March,N/A,N/A,N/A / April,N/A,N/A,N/A / May,N/A,N/A,N/A / June,N/A,N/A,N/A / July,N/A,N/A,N/A / August,N/A,N/A,N/A / September,N/A,N/A,N/A / October,N/A,N/A,N/A / November,N/A,N/A,N/A / December,N/A,N/A,N/A | all expected lines present | PASS |

27 of 27 passed

### Unit tests

| Programme | Checks | Result |
|---|---|---|
| TestBst (exercise d: all methods, by value / const reference / reference, invariant after every operation) | 87 | all pass |
| TestTree (lecturer's testTree adapted) | 12 | all pass |
| TestMap (both Map versions) | 31 | all pass |
| TestStats (mean, standard deviation, mad, sPCC, undefined cases) | 19 | all pass |
| TestMetStore (hand-checkable data, mad, solar total, duplicates,missing values, constant wind,single pair; 100,000 in-order readings) | 42 | all pass |

Data files whose last lines are blank give exactly the same Option 4 output as the same files without them (checked by comparing the two outputs).

Hand-checkable data in TestMetStore, January: S_T 0.9932178, S_R 0.9320942, T_R 0.9848391.

### Stress test

`data/data_source.txt` lists the same files many times (x10 = 30 files, about 1.5 million readings read).

| Measure | Result |
|---|---|
| Load time, x10 | about 8.5 s |
| Memory, x10 | about 116 MB |
| A month query after loading | about 0.011 s (the old scan over the whole log: 0.21 s) |
| 100,000 in-order readings loaded through `MetStore` | 0.62 s |

Repeated files add duplicate timestamps; the tree refuses them, so the statistics stay the same as for one copy of each file.

## 5. Things you still need to do

1. Replace the synthetic `data/` files with your real files and re-run `bench/testplan.py`.
2. Check a sample of Option 3 results in a spreadsheet.
3. Decide `T &` (textbook) or `const T &` in `Bst`.
4. Confirm your real `Vector`, `DataSource` and `Menu.h` match my reconstructions (README lists what changed).
5. Add your UML for the new classes (Map, Stats, MetStore, Bst relationship).
6. Rewrite section 2 and 3 in your own words.

Known issues in your own code that I reported but did not change: `Time::operator>>` is unsafe on bad input;
`Time.cpp` constructor triggers a `-Weffc++` warning (members not in the initialiser list).
