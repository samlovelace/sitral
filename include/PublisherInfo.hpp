#ifndef PUBLISHERINFO_HPP
#define PUBLISHERINFO_HPP

#include <string> 

struct PublisherInfo
{
    std::string topic; 
    std::string type; 
    std::string ip;
    uint16_t port;   

    bool operator==(const PublisherInfo& other) const
    {
        return topic == other.topic && 
               type == other.type && 
               ip == other.ip && 
               port == other.port;  
    }
};
#endif 