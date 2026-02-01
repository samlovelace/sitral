
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#include "Registry.h"
#include "example.pb.h"

Registry::Registry()
{

}

Registry::~Registry()
{

}

void Registry::run()
{
    // setup socket 
    int sock = socket(AF_INET, SOCK_STREAM, 0); 
    if(sock < 0) perror("socket"); 

    sockaddr_in addr{}; 
    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    addr.sin_port = htons(5555); 

    bind(sock, (sockaddr*)&addr, sizeof(addr));
    listen(sock, 5); 

    // keep alive 
    while(true)
    {
        int client = accept(sock, nullptr, nullptr); 
        std::thread(&Registry::handleClient, this, client).detach(); 
    }
}

void Registry::handleClient(int aClientFd)
{
    char buf[1024]; 
    ssize_t n = recv(aClientFd, buf, sizeof(buf), 0); 

    if(n > 0)
    {
        // got data, process 
        sitral::example::SimpleMessage msg;
        msg.ParseFromArray(buf, n); 
        std::cout << "Received: " << msg.text() << std::endl; 
    }
}

bool Registry::registerPublisher(const PublisherInfo& aPubInfo)
{ 
    std::vector<PublisherInfo> pubs = mRegistry[aPubInfo.topic]; 

    for(const auto& pub : mRegistry.at(aPubInfo.topic))
    {
        if(pub == aPubInfo)
        {
            // already exists, 
            return false; 
        }
    }

    // add to registry if you get here 
    pubs.push_back(aPubInfo); 
    return true; 
}

std::optional<std::vector<PublisherInfo>> Registry::queryPublishers(const std::string& aTopicName)
{
    if(mRegistry.find(aTopicName) == mRegistry.end())
    {
        return std::nullopt; 
    }

    return mRegistry.at(aTopicName); 
}

