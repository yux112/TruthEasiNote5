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
    for (int i = 1; i < 20; i++) {
        ExePath.erase(ExePath.end() - 1);
    }
    string TempPath = string(ExePath.begin(), ExePath.end());
    cout << TempPath << endl;
    string PCPath = TempPath + "\\Piep_Client.exe";
	ifstream fin(PCPath.data());
	if (fin.good()) {
		fin.close();
		WinExec(PCPath.data(), SW_SHOW);
	}
	else {
		fin.open(TempPath + "\\OnlyClient.exe");
		if (fin.good()) {
			fin.close();
			system(('\"'+ TempPath + "\\OnlyClient.exe\"").data());
		}
		else {
			cout << "Client not found." << endl;
		}
	}
	return 0;
}