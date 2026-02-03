
#include "Publisher.h"
#include "example.pb.h"

#include <cstdlib> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <cstring> 

Publisher::Publisher(const std::string& aTopicName) : mTopicName(aTopicName)
{

}

Publisher::~Publisher()
{

}

bool Publisher::advertise()
{   
    mDataSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mDataSocket < 0) {
        perror("socket"); 
    }

    sockaddr_in addr{}; 
    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    addr.sin_port = htons(0); 

    if (bind(mDataSocket, (sockaddr*)&addr, sizeof(addr)) < 0) 
    { 
        perror("bind"); return 1;
    }

    // Query the assigned port and IP 
    socklen_t len = sizeof(addr); 
    if (getsockname(mDataSocket, (sockaddr*)&addr, &len) == 0) 
    { 
        std::cout << "Bound to IP: " << inet_ntoa(addr.sin_addr) 
                  << ", Port: " << ntohs(addr.sin_port) << "\n"; 
    }

    listen(mDataSocket, 5);
    mClientAcceptThread = std::thread(&Publisher::clientAcceptLoop, this);  

    sitral::registry::RegistryRequest msg;
    auto* registerPub = msg.mutable_register_publisher();

    registerPub->set_topic(mTopicName);
    registerPub->set_type("default");
    registerPub->set_port(ntohs(addr.sin_port));

    // TODO: test msg.SerializeAsString();
    std::string serialized; 
    msg.SerializeToString(&serialized);

    std::string registryAddr = std::getenv("REGISTRY_ADDR");  // ip:port 
    std::string serverIp = ""; 
    int serverPort = -1;
    
    size_t pos = registryAddr.find(':'); 
    if(pos != std::string::npos)
    {
        serverIp = registryAddr.substr(0, pos); 
        serverPort = std::stoi(registryAddr.substr(pos+1)); 
    }

    // connect to registry and register 
    mRegistryConnection = std::make_unique<ClientSocket>(serverIp, serverPort);

    // send msg
    mRegistryConnection->sendMsg(serialized); 
    sleep(3); 
    return true; 
}

void Publisher::publish(const std::string& aMsg)
{

}

void Publisher::clientAcceptLoop()
{
    // while(true)
    // {
    //     int sub_fd = accept(mDataSocket, nullptr, nullptr); 
    //     mSubscribers.push_back(sub_fd); 
    // }
}
