
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#include "Registry.h"
#include "example.pb.h"

Registry::Registry() : mServer(std::bind(&Registry::handleClient, this, std::placeholders::_1), 5555)
{

}

Registry::~Registry()
{

}

void Registry::run()
{   
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }
}

void Registry::handleClient(int aClientFd)
{
    std::cout << "handleClient connected\n";

    while (true) {
        uint32_t len_net;
        if (!recvAll(aClientFd, &len_net, sizeof(len_net))) {
            std::cout << "client disconnected\n";
            break;
        }

        uint32_t len = ntohl(len_net);
        std::vector<char> buf(len);

        if (!recvAll(aClientFd, buf.data(), len)) {
            std::cout << "client disconnected mid-message\n";
            break;
        }

        sitral::registry::RegistryRequest msg;
        if (!msg.ParseFromArray(buf.data(), len)) {
            std::cerr << "failed to parse RegistryRequest\n";
            continue;
        }

        switch (msg.msg_case()) {
            case sitral::registry::RegistryRequest::kRegisterPublisher:
                handleRegister(msg.register_publisher());
                break;

            case sitral::registry::RegistryRequest::kQueryPublishers:
                handleQuery(aClientFd, msg.query_publishers()); 
                break; 
            default:
                std::cerr << "unknown registry message\n";
                break;
        }
    }

    close(aClientFd);
}

void Registry::handleRegister(const sitral::registry::RegisterPublisher& aMsg)
{
    std::cout << "handleRegister invoked for " << aMsg.topic() << "\n"; 
}

void Registry::handleQuery(int fd, const sitral::registry::QueryPublishers& aQuery)
{
    std::cout << "handleQuery invoked for " << aQuery.topic() << "\n";
    mServer.sendMsg(fd, "responding from the registry"); 
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

bool Registry::recvAll(int fd, void* data, size_t size)
{
    char* buf = static_cast<char*>(data);
    size_t total = 0;

    while (total < size) {
        ssize_t n = recv(fd, buf + total, size - total, 0);
        if (n <= 0) {
            return false;
        }
        total += n;
    }
    return true;
}

