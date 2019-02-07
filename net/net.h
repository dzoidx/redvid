//
// Created by morgan on 07.02.2019.
//

#ifndef REDVID_NET_H
#define REDVID_NET_H

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "Ws2_32.lib")
#else
// TODO: nix
#endif

enum class net_error
{
    None,
    InitFailed
};

bool net_init();
bool net_deinit();

int net_create_tcp_socket(const char* host, int port);
int net_send(int h_socket, const char* buffer, int len);
bool net_shutdown_send(int h_socket);
int net_receive(int h_socket, char* buffer, int len);
void net_close(int h_socket);

#endif //REDVID_NET_H
