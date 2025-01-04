#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#define BUF_SIZE 4096
#define EXAMP_PIPE   L"\\\\.\\pipe\\EasinotePiep"

int main()
{
    // ����Դ�ļ���Ŀ���ļ�·��
    const string sorce = "\\Seewo\\Easinote5\\Data\\Configs.fkv", tagert = "\\Config.fkv";
    char* ADP = new char[2561];
    size_t buffersize = 2560;
    // ��ȡ��������AppData·��
    _dupenv_s(&ADP, &buffersize, "AppData");
    const string AppDataPath = ADP;
    char* TMP = new char[2561];
    // ��ȡ��������Temp·��
    _dupenv_s(&TMP, &buffersize, "temp");
    const string TempPath = TMP;
    string fo = AppDataPath + sorce;
    string fi = TempPath + tagert;
    // �������ƺ�ɾ������
    string copycommand = "copy \"" + fo + '\"' + ' ' + '\"' + fi + '\"';
    string deletecommand = "del \"" + fo + '\"';
    // ִ�и��ƺ�ɾ������
    system(copycommand.data());
    system(deletecommand.data());
    // ��Դ�ļ�����д�룬Ŀ���ļ����ж�ȡ
    ofstream fout(fo.data());
    ifstream fin(fi.data());
    string tmp, emt;
    for (;;) {
        getline(fin, tmp);
        // �����ȡ���ض��У�д���޸ĺ������
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
    HANDLE hPipe = NULL;
    char  szBuffer[BUF_SIZE] = { 0 };
    DWORD dwReturn = 0; 
    // �ȴ������ܵ�����
    if (!WaitNamedPipe(EXAMP_PIPE, NMPWAIT_USE_DEFAULT_WAIT))
    {
        cout << "No Read Pipe Accessible" << endl;
        return 0;
    }
    // �������ܵ�
    hPipe = CreateFile(EXAMP_PIPE, GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        cout << "Open Read Pipe Error" << endl;
        return 0;
    }
    memset(szBuffer, 0, BUF_SIZE);
    string wtbuffer = "kill";
    const char* buffer = wtbuffer.data();
    // ���ַ���д�뻺����
    for (int i = 0; i < wtbuffer.length(); i++) {
        szBuffer[i] = buffer[i];
    }
    // ��ܵ�д������
    if (!WriteFile(hPipe, szBuffer, strlen(szBuffer), &dwReturn, NULL))
    {
        cout << "Write Failed" << endl;
    }
    // ����ָ������
    WinExec("C:\\Program Files (x86)\\Seewo\\EasiNote5\\swenlauncher\\swenlauncher.exe", SW_SHOW);
    // �رչܵ����
    CloseHandle(hPipe);
    return 0;
}
