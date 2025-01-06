#include <direct.h>
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

using namespace std;

#define MAX_PATH 260
#if !defined(_AMD64_) && !defined(_X86_) && !defined(_ARM64EC_) && !defined(_IA64_) && !defined(_ARM_) && !defined(_ARM64_)
#define _AMD64_
#endif

int main() {
    WCHAR ExeFile[MAX_PATH];
    GetModuleFileName(NULL, ExeFile, MAX_PATH);
    wstring ExePath = wstring(ExeFile);
    for (int i = 1; i < 19; i++) {
        ExePath.erase(ExePath.end() - 1);
    }
    string TempPath = string(ExePath.begin(), ExePath.end());
    cout << TempPath << endl;
    string FilePath = TempPath + "\\RemoveService.bat";
    ofstream fout(FilePath.data());
    fout << "@echo off" << endl;
    fout << "\"" << TempPath << "\\nssm.exe\" stop Easi_PiepServer || pause && exit /B" << endl;
    fout << "\"" << TempPath << "\\nssm.exe\" remove Easi_PiepServer confirm || pause && exit /B" << endl;
    fout.close();

    cout << "RemoveService.bat has been created." << endl;
    cout << FilePath << endl;
    string command = '\"' + FilePath + '\"';
	cout << command << endl;
    if (system(command.data()) != 0) {
        cout << "Service deleting failed." << endl;
        string deletecommand = "del \"" + FilePath + '\"';
        system(deletecommand.data());
        return -1;
    }
    else {
        cout << "Service has been removed." << endl;
    }
    string deletecommand = "del \"" + FilePath + '\"';
    system(deletecommand.data());
    return 0;
}