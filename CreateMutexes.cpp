#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::wstring ReplaceString(std::wstring str, const std::wstring& target, const std::wstring& replacement) {
    size_t pos = 0;
    while ((pos = str.find(target, pos)) != std::wstring::npos) {
        str.replace(pos, target.length(), replacement);
        pos += replacement.length();
    }
    return str;
}

int main() {
    // Get the computer name
    wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD computerNameSize = sizeof(computerName) / sizeof(computerName[0]);
    if (!GetComputerName(computerName, &computerNameSize)) {
        std::wcerr << L"Failed to get computer name, Error: " << GetLastError() << std::endl;
        return 1;
    }
    
    // Display script information
    std::wcout << L"Script Name:             CreateMutexes\n";
    std::wcout << L"Script Version:          1.0\n";
    std::wcout << L"Executing on Machine:    " << computerName << std::endl;
    std::wcout << L"\n\n";

    // Read mutex names from file
    std::wifstream mutexFile(L"mutex.list");
    if (!mutexFile.is_open()) {
        std::wcerr << L"Failed to open mutex list file." << std::endl;
        return 1;
    }

    std::vector<std::wstring> mutexNames;
    std::wstring line;
    while (std::getline(mutexFile, line)) {
        line = ReplaceString(line, L"[ComputerName]", computerName);
        mutexNames.push_back(line);
    }
    mutexFile.close();

    // Create mutexes
    std::vector<HANDLE> mutexHandles;

    for (const auto& mutexName : mutexNames) {
        HANDLE mutexHandle = CreateMutex(NULL, FALSE, mutexName.c_str());
        if (mutexHandle == NULL) {
            std::wcerr << L"Failed to create mutex: " << mutexName << L", Error: " << GetLastError() << std::endl;
        }
        else {
            mutexHandles.push_back(mutexHandle);
            std::wcout << L"Mutex added: " << mutexName << std::endl;
        }
    }

    // Wait for user input to release mutexes
    std::wcout << L"Press Enter to release mutexes..." << std::endl;
    std::wcin.get();

    // Release mutexes
    for (auto mutexHandle : mutexHandles) {
        ReleaseMutex(mutexHandle);
        CloseHandle(mutexHandle);
    }

    return 0;
}