#include <Windows.h>
#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t commandLine[256];

    wprintf(L"Enter command (<command> <operator> <file>):\n");

    if (!fgetws(commandLine, _countof(commandLine), stdin))
    {
        fwprintf(stderr, L"[ERROR] Failed to read input.\n");
        return EXIT_FAILURE;
    }

    size_t len = wcslen(commandLine);

    if (len > 0 && commandLine[len - 1] == L'\n')
        commandLine[len - 1] = L'\0';

    wchar_t* context = NULL;

    wchar_t* command = wcstok(commandLine, L" ", &context);
    wchar_t* op = wcstok(NULL, L" ", &context);
    wchar_t* fileName = wcstok(NULL, L" ", &context);

    if (!command || !op || !fileName)
    {
        fwprintf(stderr, L"[ERROR] Format: <command> <operator> <file>\n");
        return EXIT_FAILURE;
    }

    SECURITY_ATTRIBUTES sa = {
        sizeof(SECURITY_ATTRIBUTES),
        NULL,
        TRUE
    };

    HANDLE hFile = INVALID_HANDLE_VALUE;

    if (wcscmp(op, L">") == 0)
    {
        hFile = CreateFileW(
            fileName,
            GENERIC_WRITE,
            FILE_SHARE_READ,
            &sa,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    }
    else if (wcscmp(op, L"<") == 0)
    {
        hFile = CreateFileW(
            fileName,
            GENERIC_READ,
            FILE_SHARE_READ,
            &sa,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    }
    else
    {
        fwprintf(stderr, L"[ERROR] Supported operators: < and >\n");
        return EXIT_FAILURE;
    }

    if (hFile == INVALID_HANDLE_VALUE)
    {
        fwprintf(stderr,
            L"[ERROR] CreateFileW failed (%lu)\n",
            GetLastError());
        return EXIT_FAILURE;
    }

    STARTUPINFOW si;
    ZeroMemory(&si, sizeof(si));

    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;

    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    if (wcscmp(op, L">") == 0)
    {
        si.hStdOutput = hFile;
        si.hStdError = hFile;
    }
    else
    {
        si.hStdInput = hFile;
    }

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    wchar_t fullCommand[256];

    swprintf_s(
        fullCommand,
        _countof(fullCommand),
        L"cmd.exe /c %ls",
        command);

    if (!CreateProcessW(
        NULL,
        fullCommand,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si,
        &pi))
    {
        fwprintf(stderr,
            L"[ERROR] CreateProcessW failed (%lu)\n",
            GetLastError());

        CloseHandle(hFile);
        return EXIT_FAILURE;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(hFile);

    wprintf(L"[INFO] Command completed successfully.\n");

    return EXIT_SUCCESS;
}
