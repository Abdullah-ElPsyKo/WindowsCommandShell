# WinCommandRedirection

## Overview

A Windows systems programming project written in C that demonstrates input and output redirection using the Win32 API.

The application launches child processes with `CreateProcessW`, redirects the child process's standard streams through inherited file handles, and configures the process using `STARTUPINFO`. Commands are executed through `cmd.exe`, allowing the project to focus on the mechanics of Windows stream redirection rather than implementing a full command interpreter.

## Features

- Input (`<`) and output (`>`) redirection
- Process creation with `CreateProcessW`
- Handle inheritance using `SECURITY_ATTRIBUTES`
- Standard stream redirection with `STARTUPINFO`
- Win32 file handling with `CreateFileW`
- Unicode support through the Win32 wide-character APIs

---

## How It Works

1. Read a command from the user.
2. Parse the command, redirection operator, and filename.
3. Open the target file with `CreateFileW`.
4. Create inheritable file handles using `SECURITY_ATTRIBUTES`.
5. Redirect the child process's standard streams through `STARTUPINFO`.
6. Launch the child process with `CreateProcessW`.
7. Wait for the child process to finish and clean up all handles.

---

## Usage

Compile and run the program.

```text
<command> <operator> <file>
```

Examples:

```text
dir > output.txt
```

```text
sort < input.txt
```

---

## Win32 APIs Used

| API | Purpose |
|------|---------|
| `CreateProcessW` | Creates the child process |
| `CreateFileW` | Opens the redirected file |
| `STARTUPINFO` | Configures redirected standard streams |
| `SECURITY_ATTRIBUTES` | Enables handle inheritance |
| `WaitForSingleObject` | Waits for the child process to terminate |
| `CloseHandle` | Releases Windows object handles |

---

## What I Learned

Building this project improved my understanding of:

- Windows process creation
- Handle inheritance
- Standard stream redirection
- Win32 file handling
- Process synchronization
- Child process initialization with `STARTUPINFO`

---

## Future Improvements

- Support command pipelines (`|`) using anonymous pipes (`CreatePipe`)
- Support multiple redirections in a single command
- Improve command parsing
- Implement a standalone command interpreter instead of delegating execution to `cmd.exe`

---

## License

MIT License
