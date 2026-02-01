#ifndef REGISTRY_H
#define REGISTRY_H
 
#include <string> 
#include <unordered_map> 
#include <vector> 
#include <optional> 

#include "PublisherInfo.hpp"

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

private:
   
    std::unordered_map<std::string, std::vector<PublisherInfo>> mRegistry; 

};
#endif //REGISTRY_H