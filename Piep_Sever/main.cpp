#include <windows.h>
#include <iostream>
#include<fstream>
#include <strsafe.h>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
using namespace std;

#define BUF_SIZE 4096
#define EXAMP_PIPE   L"\\\\.\\pipe\\EasinotePiep"   

ofstream logout("server.log");
void log(string str) {
	auto now = chrono::system_clock::now();
	time_t now_time_t = chrono::system_clock::to_time_t(now);
	tm now_tm;
	localtime_s(&now_tm, &now_time_t);
	cout << "[" << put_time(&now_tm, "%y/%m/%d %H:%M:%S") << "]:";
	cout << str << endl;
	logout << "[" << put_time(&now_tm, "%y/%m/%d %H:%M:%S") << "]:";
    logout << str << endl;
}
void ErrorExit(LPCTSTR lpszFunction)
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
    wstring wstr((LPCTSTR)lpDisplayBuf);
    string str(wstr.begin(), wstr.end());
	log(str);
    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}

int main()
{
    for (;;) {
        HANDLE hPipe = NULL;
        SECURITY_ATTRIBUTES sa;
        SECURITY_DESCRIPTOR sd;
        PACL acl = (PACL)LocalAlloc(LPTR, sizeof(ACL) + 1024);
        if (acl == nullptr) {
            ErrorExit(TEXT("LocalAlloc"));
        }
        if (!InitializeAcl(acl, 1024, ACL_REVISION)) {
            ErrorExit(TEXT("InitializeAcl"));
        }
        SID_IDENTIFIER_AUTHORITY SysAuth = SECURITY_NT_AUTHORITY;
        PSID Sys = nullptr, Admins = nullptr, Users = nullptr, Guests = nullptr;
        if (!AllocateAndInitializeSid(&SysAuth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_SYSTEM_OPS, 0, 0, 0, 0, 0, 0, &Sys) || 
            !AllocateAndInitializeSid(&SysAuth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &Admins) || 
            !AllocateAndInitializeSid(&SysAuth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_USERS, 0, 0, 0, 0, 0, 0, &Users) || 
            !AllocateAndInitializeSid(&SysAuth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_GUESTS, 0, 0, 0, 0, 0, 0, &Guests)) {
            ErrorExit(TEXT("AllocateAndInitializeSid"));
        }
        else {
            if (!AddAccessDeniedAce(acl, ACL_REVISION, GENERIC_ALL, Guests) || 
                !AddAccessAllowedAce(acl, ACL_REVISION, GENERIC_ALL, Sys) || 
                !AddAccessAllowedAce(acl, ACL_REVISION, GENERIC_ALL, Admins) || 
                !AddAccessAllowedAce(acl, ACL_REVISION, GENERIC_WRITE | GENERIC_READ, Users)) {
                ErrorExit(TEXT("AddACE"));
            }
        }
        if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION)) {
            ErrorExit(TEXT("InitializeSecurityDescriptor"));
        }
        if (!SetSecurityDescriptorDacl(&sd, TRUE, acl, FALSE)) {
            ErrorExit(TEXT("SetSecurityDescriptorDacl"));
        }
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = &sd;
        sa.bInheritHandle = FALSE;

        hPipe = CreateNamedPipe(EXAMP_PIPE, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, BUF_SIZE, BUF_SIZE, 0, &sa);

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            ErrorExit(TEXT("CreateNamedPipe"));
            return FALSE;
        }
        if (!ConnectNamedPipe(hPipe, NULL))
        {
			ErrorExit(TEXT("ConnectNamedPipe"));
            return FALSE;
        }
        DWORD dwReturn = 0;
        char szBuffer[BUF_SIZE] = { 0 };
        string rdbuffer;
        memset(szBuffer, 0, BUF_SIZE);
        if (ReadFile(hPipe, szBuffer, BUF_SIZE, &dwReturn, NULL))
        {
            szBuffer[dwReturn] = '\0';
            rdbuffer = szBuffer;
            if (rdbuffer == "kill") {
				log("Kill Command Received");
                WinExec("taskkill.exe /F /T /IM easi*", SW_HIDE);
            }
            else {
                ULONG clientProcessId = 0;
                if (!GetNamedPipeClientProcessId(hPipe, &clientProcessId))
                {
                    auto now = chrono::system_clock::now();
                    time_t now_time_t = chrono::system_clock::to_time_t(now);
                    tm now_tm;
                    localtime_s(&now_tm, &now_time_t);
                    logout << "[" << put_time(&now_tm, "%y/%m/%d %H:%M:%S") << "]:";
                    logout << "Failed to get client process ID" << endl;
                    CloseHandle(hPipe);
                    continue;
                }
                else
                {
                    auto now = chrono::system_clock::now();
                    time_t now_time_t = chrono::system_clock::to_time_t(now);
                    tm now_tm;
                    localtime_s(&now_tm, &now_time_t);
					logout << "[" << put_time(&now_tm, "%y/%m/%d %H:%M:%S") << "]:";
                    logout << "Client Process ID: " << clientProcessId << endl;
					string killprocesscommand = "taskkill /F /PID " + to_string(clientProcessId);
                    WinExec(killprocesscommand.data(), SW_HIDE); 
                    CloseHandle(hPipe);
                    continue;
                }
            }
        }
        else
        {
            logout << "Read Failed" << endl;
        }
        CloseHandle(hPipe);
    }
    return 0;
}
