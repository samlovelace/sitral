#ifndef PUBLISHERINFO_HPP
#define PUBLISHERINFO_HPP

#include <string> 

struct PublisherInfo
{
    std::string topic; 
    std::string type; 
    std::string address; // ip:port  

    bool operator==(const PublisherInfo& other) const
    {
        return topic == other.topic && 
               type == other.type && 
               address == other.address; 
    }
};
#endif 