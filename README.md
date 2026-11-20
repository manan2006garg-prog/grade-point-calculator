# Weighted SGPA Calculator

Calculate a semester grade point average from course grade points and credit values. The program prints a course-by-course summary and writes a CSV-style report to `sgpa_report.txt`.

## Build and run

Compile with a C11 compiler:

    gcc -std=c11 -Wall -Wextra -pedantic main.c -o sgpa-calculator
    ./sgpa-calculator

On Windows, run `sgpa-calculator.exe` after compiling.

## Features

- Accepts up to 20 courses with names, 1-6 credits, and grade points from 0 to 10.
- Calculates a credit-weighted SGPA: sum of (credits x grade point) divided by total credits.
- Validates text and numeric input, then prints a formatted semester summary.
- Saves the course details and result to `sgpa_report.txt`.

## Learning goals

Practice structures, arrays, functions, validation, weighted averages, and file output in C. This uses a generic 10-point grade scale; adjust it to match your institution's official rules.
