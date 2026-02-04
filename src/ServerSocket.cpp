
#include "ServerSocket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <iostream>

ServerSocket::ServerSocket(std::function<void(int)> aClientHandler, int aPort) : mClientHandler(aClientHandler)
{
    // setup socket 
    mSocketFd = socket(AF_INET, SOCK_STREAM, 0); 
    if(mSocketFd < 0) perror("socket"); 

    sockaddr_in addr{}; 
    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    addr.sin_port = htons(aPort);

    if (bind(mSocketFd, (sockaddr*)&addr, sizeof(addr)) < 0) 
    { 
        perror("bind");
        throw std::runtime_error("failed to bind server socket"); 
    }

    // Query the assigned port and IP 
    socklen_t len = sizeof(addr); 
    if (getsockname(mSocketFd, (sockaddr*)&addr, &len) == 0) 
    { 
        std::cout << "Bound to IP: " << inet_ntoa(addr.sin_addr) 
                  << ", Port: " << ntohs(addr.sin_port) << "\n";
        mPort = addr.sin_port; 
    }

    listen(mSocketFd, 5);
    mClientHandleThread = std::thread(&ServerSocket::clientHandleLoop, this); 
}

ServerSocket::~ServerSocket()
{

}

bool ServerSocket::sendAll(int fd, const void* data, size_t size)
{
    const char* buf = static_cast<const char*>(data);
    size_t totalSent = 0;

    while (totalSent < size) {
        ssize_t sent = send(fd, buf + totalSent, size - totalSent, 0);
        if (sent <= 0) {
            return false;  // client dead or error
        }
        totalSent += sent;
    }
    return true;
}

void ServerSocket::clientHandleLoop()
{
    // keep alive 
    while(true)
    {
        int client = accept(mSocketFd, nullptr, nullptr); 
        std::thread(mClientHandler, client).detach(); 
    }

}

