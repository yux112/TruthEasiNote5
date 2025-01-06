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
    for (int i = 1; i < 16; i++) {
        ExePath.erase(ExePath.end() - 1);
    }
    string TempPath = string(ExePath.begin(), ExePath.end());
    cout << TempPath << endl;
    string FilePath = TempPath + "\\RegService.bat";
    ofstream fout(FilePath.data());
    fout << "@echo off" << endl;
    fout << "\"" << TempPath << "\\nssm.exe\" install Easi_PiepServer \"" << TempPath << "\\Piep_Server.exe\" || pause && exit /B -1" << endl;
    fout << "\"" << TempPath << "\\nssm.exe\" set Easi_PiepServer Description \"Ï£ÎÖ¡°Õæ¡±°×°å\" || pause && exit /B -1" << endl;
    fout << "\"" << TempPath << "\\nssm.exe\" set Easi_PiepServer AppDirectory \"" << TempPath << '\"' <<" || pause && exit /B -1" << endl;
    fout << "\"" << TempPath << "\\nssm.exe\" set Easi_PiepServer AppStdout \"" << TempPath << "\\stdout.log\" || pause && exit /B -1" << endl;
    fout << "\"" << TempPath << "\\nssm.exe\" set Easi_PiepServer AppStderr \"" << TempPath << "\\stderr.log\" || pause && exit /B -1" << endl;
    fout << "\"" << TempPath << "\\nssm.exe\" set Easi_PiepServer Start SERVICE_AUTO_START || pause && exit /B -1" << endl;
    fout << "\"" << TempPath << "\\nssm.exe\" start Easi_PiepServer || pause && exit /B -1" << endl;
	fout << "exit /B 0" << endl;
    fout.close();

    cout << "RegService.bat has been created." << endl;
    cout << FilePath << endl;
    string command = '\"' + FilePath + '\"';
    if (system(command.data()) != 0) {
        cout << "Service registration failed." << endl;
        WinExec((TempPath + "\\RemoveService.exe").data(), SW_HIDE);
        string deletecommand = "del \"" + FilePath + '\"';
        system(deletecommand.data());
        return -1;
    }
	else {
        cout << "Service has been registered." << endl;
        string deletecommand = "del \"" + FilePath + '\"';
        system(deletecommand.data());
        return 0;
	}
    string deletecommand = "del \"" + FilePath + '\"';
    system(deletecommand.data());
    return 0;
}