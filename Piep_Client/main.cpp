#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#define BUF_SIZE 4096
#define EXAMP_PIPE   L"\\\\.\\pipe\\EasinotePiep"

int main()
{
    // 定义源文件和目标文件路径
    const string sorce = "\\Seewo\\Easinote5\\Data\\Configs.fkv", tagert = "\\Config.fkv";
    char* ADP = new char[2561];
    size_t buffersize = 2560;
    // 获取环境变量AppData路径
    _dupenv_s(&ADP, &buffersize, "AppData");
    const string AppDataPath = ADP;
    char* TMP = new char[2561];
    // 获取环境变量Temp路径
    _dupenv_s(&TMP, &buffersize, "temp");
    const string TempPath = TMP;
    string fo = AppDataPath + sorce;
    string fi = TempPath + tagert;
    // 构建复制和删除命令
    string copycommand = "copy \"" + fo + '\"' + ' ' + '\"' + fi + '\"';
    string deletecommand = "del \"" + fo + '\"';
    // 执行复制和删除命令
    system(copycommand.data());
    system(deletecommand.data());
    // 打开源文件进行写入，目标文件进行读取
    ofstream fout(fo.data());
    ifstream fin(fi.data());
    string tmp, emt;
    for (;;) {
        getline(fin, tmp);
        // 如果读取到特定行，写入修改后的内容
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
    // 等待命名管道可用
    if (!WaitNamedPipe(EXAMP_PIPE, NMPWAIT_USE_DEFAULT_WAIT))
    {
        cout << "No Read Pipe Accessible" << endl;
        return 0;
    }
    // 打开命名管道
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
    // 将字符串写入缓冲区
    for (int i = 0; i < wtbuffer.length(); i++) {
        szBuffer[i] = buffer[i];
    }
    // 向管道写入数据
    if (!WriteFile(hPipe, szBuffer, strlen(szBuffer), &dwReturn, NULL))
    {
        cout << "Write Failed" << endl;
    }
    // 启动指定程序
    WinExec("C:\\Program Files (x86)\\Seewo\\EasiNote5\\swenlauncher\\swenlauncher.exe", SW_SHOW);
    // 关闭管道句柄
    CloseHandle(hPipe);
    return 0;
}
