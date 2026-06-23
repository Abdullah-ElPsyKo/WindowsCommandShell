# WinCommandRedirection

## Overview
A low-level C implementation demonstrating custom I/O redirection (> and <) via the native Windows API. Implements explicit kernel handle inheritance (SECURITY_ATTRIBUTES) and process manipulation by overriding STARTUPINFO standard streams (hStdOutput/hStdInput) before spawning child processes via CreateProcessW.

## Usage
1. Compile and run the program.
2. Enter commands in the format:
   ```
   <command> <operator> <fileName>
   ```
   Example: `dir > output.txt`

## License
MIT License
