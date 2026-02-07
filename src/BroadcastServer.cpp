
#include "BroadcastServer.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

BroadcastServer::BroadcastServer() : ServerSocket(std::bind(&BroadcastServer::handleClient, this, std::placeholders::_1))
{

}

BroadcastServer::~BroadcastServer()
{

}

void BroadcastServer::handleClient(int aClientFd)
{
    std::cout << "Broadcast server handle client\n";
    std::lock_guard<std::mutex> lock(mClientsMtx); 
    mClients.push_back(aClientFd); 
    std::cout << "Clients size: " << mClients.size(); 
}

void BroadcastServer::broadcast(const std::string& payload)
{
    uint32_t len = htonl(static_cast<uint32_t>(payload.size()));

    std::lock_guard<std::mutex> lock(mClientsMtx);

    for (auto it = mClients.begin(); it != mClients.end(); ) 
    {
        int fd = *it;

        bool ok =
            sendAll(fd, &len, sizeof(len)) &&
            sendAll(fd, payload.data(), payload.size());

        if (!ok) 
        {
            // client disconnected or errored
            close(fd);
            it = mClients.erase(it);
        } 
        else 
        {
            ++it;
        }
    }
}

