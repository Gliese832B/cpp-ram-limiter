// deneme.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.

#include <iostream>
#include <cstdio>
#include <Windows.h>

#include <TlHelp32.h>
#include <memoryapi.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <Psapi.h>
#include <vector>
using namespace std;
std::vector<std::pair<std::wstring, int>> pl;
void RestrictWorkingSetSizeForPid(DWORD pid, SIZE_T minSize, SIZE_T maxSize) {
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (processHandle == NULL) {
        _tprintf(_T("OpenProcess failed. Error code: %d\n"), GetLastError());
        return;
    }

    BOOL result = SetProcessWorkingSetSizeEx(processHandle, minSize, maxSize, QUOTA_LIMITS_HARDWS_MAX_ENABLE);
    if (!result) {
        _tprintf(_T("SetProcessWorkingSetSize failed. Error code: %d\n"), GetLastError());
    }

    CloseHandle(processHandle);
}
DWORD FindProcessId(const std::wstring& processName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile))
    {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!processName.compare(processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}
void PrintProcessNameAndID(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);

    // Get the process name.

    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR));
        }
    }

    // Print the process name and identifier.

    

    // Release the handle to the process.
    pl.push_back(std::make_pair(szProcessName, processID));
    CloseHandle(hProcess);
}
int main()
{

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return 1;
    }


    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            PrintProcessNameAndID(aProcesses[i]);
            
        }
    }

    /*
    auto processid = FindProcessId(L"vivaldi.exe");
    cout << "Process id: " << processid << endl;
    SIZE_T  dwMin, dwMax;
    HANDLE hProcess;
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA,
        FALSE, processid);
    if (!hProcess)
    {
        std::printf("OpenProcess failed (%d)\n", GetLastError());
        return 1;
    }
    
    //for (int i = 0; i < 20; i++) {
        if (!SetProcessWorkingSetSizeEx(hProcess, 409600, 9994240, QUOTA_LIMITS_HARDWS_MAX_ENABLE)) {
            cout << "setprocessworkingsetsizeex error" << endl;
            cout << GetLastError() << endl;
        }
    //}
     

    if (!GetProcessWorkingSetSize(hProcess, &dwMin, &dwMax))
    {
        std::printf("GetProcessWorkingSetSize failed (%d)\n",
            GetLastError());
        return 1;
    }
    
    
    
    std::printf("Minimum working set: %lu KB\n", dwMin / 1024);
    std::printf("Maximum working set: %lu KB\n", dwMax / 1024);
    */
    
    for (auto p : pl) {
        if (p.first == L"vivaldi.exe")
            RestrictWorkingSetSizeForPid(p.second, -1, -1);
        cout << "Limited" << endl;
    }
        


    std::cin.get();
    return 0;
    
}

// Programı çalıştır: Ctrl + F5 veya Hata Ayıkla > Hata Ayıklamadan Başlat menüsü
// Programda hata ayıkla: F5 veya Hata Ayıkla > Hata Ayıklamayı Başlat menüsü

// Kullanmaya Başlama İpuçları: 
//   1. Dosyaları eklemek/yönetmek için Çözüm Gezgini penceresini kullanın
//   2. Kaynak denetimine bağlanmak için Takım Gezgini penceresini kullanın
//   3. Derleme çıktısını ve diğer iletileri görmek için Çıktı penceresini kullanın
//   4. Hataları görüntülemek için Hata Listesi penceresini kullanın
//   5. Yeni kod dosyaları oluşturmak için Projeye Git > Yeni Öğe ekle veya varolan kod dosyalarını projeye eklemek için Proje > Var Olan Öğeyi Ekle adımlarını izleyin
//   6. Bu projeyi daha sonra yeniden açmak için Dosya > Aç > Proje'ye gidip .sln uzantılı dosyayı seçin
