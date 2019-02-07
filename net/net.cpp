//
// Created by morgan on 07.02.2019.
//

#include "net.h"
#include <sstream>

bool net_init()
{
#ifdef _WIN32
    WSADATA wsaData;
    auto res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(res != 0) {
        return false;
    }
#endif
    return true;
}

bool net_deinit()
{
#ifdef _WIN32
    auto res = WSACleanup();
    if(res != 0)
    {
        return false;
    }
#endif
    return true;
}

int net_create_tcp_socket(const char* host, int port)
{
#ifdef _WIN32
    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;
    std::stringstream port_str;
    port_str << port;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // The address family was left unspecified (AF_UNSPEC), so the returned IP address could be either an IPv6 or IPv4 address for the server.
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    auto res = getaddrinfo(host, port_str.str().c_str(), &hints, &result);
    if(res != 0) {
        freeaddrinfo(result);
        return 0;
    }
    SOCKET con = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(con == INVALID_SOCKET) {
        freeaddrinfo(result);
        return 0;
    }
    res = connect(con, result->ai_addr, result->ai_addrlen);
    freeaddrinfo(result);
    if(res == SOCKET_ERROR) {
        closesocket(con);
        return 0;
    }
    return (int) con;
#endif
}

void net_close(int h_socket)
{
#ifdef _WIN32
    closesocket((SOCKET)h_socket);
#endif
}

int net_send(int h_socket, const char* buffer, int len)
{
#ifdef _WIN32
    SOCKET con = (SOCKET)h_socket;
    auto res = send(con, buffer, len, 0);
    if(res == SOCKET_ERROR) {
        closesocket(con);
        return 0;
    }
    return res;
#endif
}

int net_receive(int h_socket, char* buffer, int len)
{
#ifdef _WIN32
    SOCKET con = (SOCKET)h_socket;
    auto res = recv(con, buffer, len, 0);
    return res;
#endif
}

bool net_shutdown_send(int h_socket)
{
#ifdef _WIN32
    SOCKET con = (SOCKET)h_socket;
    auto res = shutdown(con, SD_SEND);
    if(res == SOCKET_ERROR)  {
        return false;
    }
#endif
    return true;
}

