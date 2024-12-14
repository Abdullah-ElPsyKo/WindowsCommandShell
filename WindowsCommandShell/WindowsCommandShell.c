#include <Windows.h>
#include <stdio.h>
#include <wchar.h>

int main() {
	wchar_t commandLine[100];
	fgetws(commandLine, sizeof(commandLine) / sizeof(wchar_t), stdin);
	commandLine[wcslen(commandLine) - 1] = L'\0';

	wchar_t* context = NULL;

	wchar_t* command = wcstok(commandLine, L" ", &context);
	wchar_t* operator = wcstok(NULL, L" ", &context);
	wchar_t* fileName = wcstok(NULL, L" ", &context);

	if (!command || !operator || !fileName) {
		printf("[ERROR] Invalid input. Format: <command> <operator> <fileName>\n");
		return EXIT_FAILURE;
	}

	HANDLE hFile = NULL;

	// Make the file handle inheritable
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if (wcscmp(operator, L">") == 0) {
		hFile = CreateFileW(
			fileName, GENERIC_WRITE, FILE_SHARE_READ, &sa, 
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
		);
		if (hFile == INVALID_HANDLE_VALUE) {
			wprintf(L"[ERROR] Failed to create/open file for writing. Error: %lu\n", GetLastError());
			return EXIT_FAILURE;
		}
	}
	else if (wcscmp(operator, L"<") == 0) {
		hFile = CreateFileW(
			fileName, GENERIC_READ, FILE_SHARE_READ, &sa,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
		);
		if (hFile == INVALID_HANDLE_VALUE) {
			wprintf(L"[ERROR] Failed to open file for reading. Error: %lu\n", GetLastError());
		}
	}
	else {
		wprintf(L"[ERROR] Invalid operator.");
		return EXIT_FAILURE;
	}

	// Prepare process info
	STARTUPINFO startInf;
	PROCESS_INFORMATION procInf;
	ZeroMemory(&startInf, sizeof(startInf));
	startInf.cb = sizeof(startInf);
	startInf.dwFlags = STARTF_USESTDHANDLES;

	if (wcscmp(operator, L">") == 0) {
		startInf.hStdOutput = hFile;
		startInf.hStdError = hFile;
	}
	else if (wcscmp(operator, L"<") == 0) {
		startInf.hStdInput = hFile;
	}

	ZeroMemory(&procInf, sizeof(procInf));
	wchar_t fullCommand[100];
	swprintf_s(fullCommand, _countof(fullCommand), L"cmd.exe /c %ls", command);

	if (CreateProcessW(NULL, fullCommand, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startInf, &procInf)) {
		wprintf(L"[INFO] Command executed successfully.\n");

		WaitForSingleObject(procInf.hProcess, INFINITE);

		CloseHandle(procInf.hProcess);
		CloseHandle(procInf.hThread);
	}
	else {
		wprintf(L"[ERROR] Failed to execute command. Error: %lu\n", GetLastError());
	}

	// Close file handle
	if (hFile != NULL) CloseHandle(hFile);
	return 0;
}