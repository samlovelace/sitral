#ifndef REGISTRY_H
#define REGISTRY_H
 
#include <string> 
#include <unordered_map> 
#include <vector> 
#include <optional> 

#include "PublisherInfo.hpp"
#include "example.pb.h"

class Registry 
{ 
public:
    Registry();
    ~Registry();

    void run(); 

    bool registerPublisher(const PublisherInfo& aPubInfo); 
    std::optional<std::vector<PublisherInfo>> queryPublishers(const std::string& aTopicName); 

private: 
    void handleClient(int aClientFd); 
    void handleRegister(const sitral::registry::RegisterPublisher& aMsg); 
    bool recvAll(int fd, void* data, size_t size);

private:
   
    std::unordered_map<std::string, std::vector<PublisherInfo>> mRegistry; 

};
#endif //REGISTRY_H