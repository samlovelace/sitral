#include "ClientSocket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <iostream>

ClientSocket::ClientSocket(const std::string& aServerIp, int aServerPort)
{
    mSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocketFd < 0) {
        perror("socket");
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(aServerPort);

    if (inet_pton(AF_INET, aServerIp.c_str(), &serverAddr.sin_addr) < 0) {
        perror("inet_pton");
        close(mSocketFd);
        mSocketFd = -1;
        return;
    }

    if (connect(mSocketFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect");
        close(mSocketFd);
        mSocketFd = -1;
    }
}

ClientSocket::~ClientSocket()
{
    mRunning = false;
    if (mSocketFd >= 0) {
        shutdown(mSocketFd, SHUT_RDWR);
        close(mSocketFd);
    }
    if (mRecvThread.joinable()) {
        mRecvThread.join();
    }
}

void ClientSocket::setReceiveCallback(RawCallback cb)
{
    mCallback = std::move(cb);
    mRunning = true;
    mRecvThread = std::thread(&ClientSocket::recvLoop, this);
}

bool ClientSocket::sendAll(const void* data, size_t size)
{
    const char* buf = static_cast<const char*>(data);
    size_t totalSent = 0;

    while (totalSent < size) {
        ssize_t sent = send(mSocketFd, buf + totalSent, size - totalSent, 0);
        if (sent <= 0) {
            perror("send");
            return false;
        }
        totalSent += sent;
        printf("sent more\n"); 
    }

    return true;
}

bool ClientSocket::sendMsg(const std::string& msg)
{
    if (mSocketFd < 0) return false;

    uint32_t len = htonl(static_cast<uint32_t>(msg.size()));

    if (!sendAll(&len, sizeof(len))) return false;
    if (!sendAll(msg.data(), msg.size())) return false;

    return true;
}

bool ClientSocket::recvAll(void* data, size_t size)
{
    char* buf = static_cast<char*>(data);
    size_t total = 0;

    while (total < size) {
        ssize_t n = recv(mSocketFd, buf + total, size - total, 0);
        if (n <= 0) {
            return false;
        }
        total += n;
    }
    return true;
}

void ClientSocket::recvLoop()
{
    while (mRunning) 
    {
        uint32_t len_net;
        if (!recvAll(&len_net, sizeof(len_net))) 
        {
            break;
        }

        uint32_t len = ntohl(len_net);
        std::string payload(len, '\0');

        if (!recvAll(payload.data(), len)) 
        {
            break;
        }

        if (mCallback) 
        {
            mCallback(payload);  // raw bytes
        }
    }

    mRunning = false;
}


