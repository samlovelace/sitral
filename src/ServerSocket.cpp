
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

    bind(mSocketFd, (sockaddr*)&addr, sizeof(addr));
    listen(mSocketFd, 5);

    mClientHandleThread = std::thread(&ServerSocket::clientHandleLoop, this); 
}

ServerSocket::~ServerSocket()
{

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

