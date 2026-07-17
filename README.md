# WinCommandRedirection

## Overview

A Windows command shell written in C that demonstrates input and output redirection using the Win32 API.

The project creates child processes with `CreateProcessW`, redirects standard input and output through inherited file handles, and configures the child process using `STARTUPINFO`. It provides a simplified implementation of the `>` and `<` operators to demonstrate how Windows command-line redirection works internally.

## Features

- Input (`<`) and output (`>`) redirection
- Process creation with `CreateProcessW`
- Handle inheritance using `SECURITY_ATTRIBUTES`
- Standard stream redirection with `STARTUPINFO`
- File I/O using the Win32 API

## Usage

Compile and run the program.

```
<command> <operator> <file>
```

Example:

```text
dir > output.txt
```

or

```text
sort < input.txt
```

## What I Learned

This project improved my understanding of:

- Windows process creation
- Handle inheritance
- Standard stream redirection
- Win32 file handling
- Process initialization with `STARTUPINFO`

## License

MIT License
