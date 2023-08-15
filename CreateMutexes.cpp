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

void LogError(const std::wstring& message) {
    std::wcerr << L"Error: " << message << L", Error Code: " << GetLastError() << std::endl;
}

void DisplayScriptInfo(const std::wstring& computerName) {
    std::wcout << L"Script Name:             CreateMutexes\n";
    std::wcout << L"Script Version:          1.1\n";
}

bool GetComputerNameSafe(wchar_t* computerName, DWORD* size) {
    if (!GetComputerName(computerName, size)) {
        LogError(L"Failed to get computer name");
        return false;
    }
    return true;
}

void DisplayExecutionInfo(const std::wstring& computerName) {
    std::wcout << L"Executing on Machine:    " << computerName << std::endl;
    std::wcout << L"\n\n";
}

bool ReadMutexNamesFromFile(const std::wstring& filePath, std::vector<std::wstring>& mutexNames, const std::wstring& computerName) {
    std::wifstream mutexFile(filePath);
    if (!mutexFile.is_open()) {
        LogError(L"Failed to open mutex list file.");
        return false;
    }

    std::wstring line;
    while (std::getline(mutexFile, line)) {
        line = ReplaceString(line, L"[ComputerName]", computerName);
        mutexNames.push_back(line);
    }
    mutexFile.close();
    return true;
}

bool CreateMutexes(const std::vector<std::wstring>& mutexNames, std::vector<HANDLE>& mutexHandles) {
    for (const auto& mutexName : mutexNames) {
        HANDLE mutexHandle = CreateMutex(NULL, FALSE, mutexName.c_str());
        if (mutexHandle == NULL) {
            LogError(L"Failed to create mutex: " + mutexName);
        }
        else {
            mutexHandles.push_back(mutexHandle);
            std::wcout << L"Mutex added: " << mutexName << std::endl;
        }
    }
    return true;
}

void ReleaseMutexes(const std::vector<HANDLE>& mutexHandles) {
    for (auto mutexHandle : mutexHandles) {
        ReleaseMutex(mutexHandle);
        CloseHandle(mutexHandle);
    }
}

int main() {
    // Get the computer name
    wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD computerNameSize = sizeof(computerName) / sizeof(computerName[0]);
    if (!GetComputerNameSafe(computerName, &computerNameSize)) {
        return 1;
    }

    DisplayScriptInfo(computerName);

    DisplayExecutionInfo(computerName);

    std::vector<std::wstring> mutexNames;
    if (!ReadMutexNamesFromFile(L"mutex.list", mutexNames, computerName)) {
        return 1;
    }

    std::vector<HANDLE> mutexHandles;
    if (!CreateMutexes(mutexNames, mutexHandles)) {
        ReleaseMutexes(mutexHandles);
        return 1;
    }

    std::wcout << L"Press Enter to release mutexes..." << std::endl;
    std::wcin.get();

    ReleaseMutexes(mutexHandles);

    return 0;
}
