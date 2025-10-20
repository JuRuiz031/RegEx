# Kronos InTouch Log Parser

This small C++ utility parses log files produced by the Kronos InTouch device and generates a short human-readable report showing device boot attempts, the start and completion timestamps, line numbers in the original log, and boot durations (when available).

The parser was implemented as a coursework / practice project using Boost.Regex and Boost.Date_Time for parsing timestamps and computing durations.

## Features

- Scans a single log file for two kinds of lines:
  - A boot start line matching the pattern: `YYYY-MM-DD HH:MM:SS: (log.c.166) server started`
  - A boot-complete line matching the pattern: `YYYY-MM-DD HH:MM:SS.mmm:INFO:oejs.AbstractConnector:Started SelectChannelConnector@...:PORT`
- For each boot attempt, the program writes a block to an output file (input filename plus `.rpt`) containing:
  - The input file line number and filename for the start and end events
  - The formatted timestamps for start and end
  - The boot time in milliseconds for completed boots
  - A marker for incomplete boots when the end event is not found

## Build

Requirements:

- A C++17-capable compiler (tested with g++)
- Boost libraries: `boost_regex` and `boost_date_time`

From the repository root (Windows PowerShell / Command Prompt):

```
make
```

This will build an executable named `ps7` using the provided `Makefile`. If you don't have `make` on Windows, build with your compiler manually, for example:

```
g++ --std=c++17 -Wall -Werror -pedantic -g main.cpp -o ps7 -lboost_regex -lboost_date_time
```

## Usage

Run the program with a single log file path as the only argument. It writes output to `<inputfile>.rpt` in the same directory.

```
./ps7 device1_intouch.log.rpt

# produces device1_intouch.log.rpt.rpt
```

Output example (excerpt):

```
=== Device boot ===
12(device1_intouch.log.rpt): 2024-01-01 12:34:56 Boot Start
20(device1_intouch.log.rpt): 2024-01-01 12:34:59 Boot Completed
	Boot Time: 3000ms

```

Notes:

- The program expects timestamps at the beginning of each log line in the formats `YYYY-MM-DD HH:MM:SS` (start) and `YYYY-MM-DD HH:MM:SS.mmm` (end). It uses substring positions to extract hours, minutes, and seconds; malformed lines or alternative timestamp formats will not be parsed.

## Files

- `main.cpp` – Main program. Contains the regexes used to detect start and end events and the `printResult` helper that formats the report.
- `Makefile` – Simple make rules to build `ps7` and clean object files.
## Known limitations

- The regexes and timestamp extraction are somewhat brittle:
  - The code uses fixed substring positions to parse hours/minutes/seconds; if the timestamp format changes (extra spaces, different separators) parsing will fail or produce incorrect times.
  - The start regex looks specifically for `(log.c.166) server started`. If other startup messages exist the program won't detect them.
- No command-line options: only a single input filename is accepted.
- Limited error reporting: the program prints a few simple error messages and exits; it does not provide verbose or structured logging.
- Memory/exception handling: the program uses basic error checks on file streams but does not catch exceptions from Boost APIs.

## Possible future enhancements

- Make timestamp parsing robust:
  - Use Boost.Date_Time's parsers or std::chrono parsing instead of manual substring extraction.
  - Accept multiple timestamp formats and tolerate minor whitespace differences.
- Improve regex coverage:
  - Allow multiple variants of startup and connector messages (make regexes configurable or more permissive).
- Add command-line options:
  - Accept an output filename, recursive directory scanning, or a verbosity flag.
- Unit tests / sample inputs:
  - Add small sample logs and unit tests to verify parsing logic and edge cases (incomplete boots, repeated starts without ends, missing timestamps).
- Packaging & cross-platform build:
  - Add a CMakeLists.txt for cross-platform builds and CI configuration to compile on GitHub Actions.
