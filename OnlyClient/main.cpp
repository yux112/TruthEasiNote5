#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
int main()
{
    const string sorce = "\\Seewo\\Easinote5\\Data\\Configs.fkv", tagert = "\\Config.fkv";
    char* ADP = new char[2561];
    size_t buffersize = 2560;
    _dupenv_s(&ADP, &buffersize, "AppData");
    const string AppDataPath = ADP;
    char* TMP = new char[2561];
    _dupenv_s(&TMP, &buffersize, "temp");
    const string TempPath = TMP;
    string fo = AppDataPath + sorce;
    string fi = TempPath + tagert;
    string copycommand = "copy \"" + fo + '\"' + ' ' + '\"' + fi + '\"';
    string deletecommand = "del \"" + fo + '\"';
    system(copycommand.data());
    system(deletecommand.data());
    ofstream fout(fo.data());
    ifstream fin(fi.data());
    string tmp, emt;
    for (;;) {
        getline(fin, tmp);
        if (tmp == "DeviceCache.IsIwb") {
            fout << tmp << endl;
            fout << "True" << endl;
            getline(fin, emt);
            emt.clear();
            getline(fin, tmp);
        }
        fout << tmp << endl;
        if (fin.eof()) {
            break;
        }
    }
    fin.close();
    WinExec("taskkill.exe /F /T /IM easi*", SW_HIDE);
    WinExec("C:\\Program Files (x86)\\Seewo\\EasiNote5\\swenlauncher\\swenlauncher.exe", SW_SHOW);
	return 0;
}