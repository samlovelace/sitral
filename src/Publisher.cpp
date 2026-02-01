
#include "Publisher.h"
#include "example.pb.h"

#include <cstdlib> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <cstring> 

Publisher::Publisher(const std::string& aTopicName)
{

}

Publisher::~Publisher()
{

}

bool Publisher::advertise()
{   
    // setup client socket to talk to registry 
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket");
        return false; 
    }

    std::string registryAddr = std::getenv("REGISTRY_ADDR");  // ip:port 
    std::string serverIp = ""; 
    int serverPort = -1;
    
    size_t pos = registryAddr.find(':'); 
    if(pos != std::string::npos)
    {
        serverIp = registryAddr.substr(0, pos); 
        serverPort = std::stoi(registryAddr.substr(pos+1)); 
    }

    sockaddr_in serverAddr{}; 
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(serverPort); 
    inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr); 

    if(connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connect"); 
        close(sock); 
        return false; 
    }

    sitral::example::SimpleMessage msg; 
    msg.set_text("hello from protobuf"); 

    // TODO: test msg.SerializeAsString();s
    std::string serialized; 
    msg.SerializeToString(&serialized);

    send(sock, serialized.c_str(), serialized.size(), 0);

    sleep(3); 
    close(sock); 
    return true; 
}

void Publisher::publish(const std::string& aMsg)
{

}