#ifndef PUBLISHER_H
#define PUBLISHER_H
 
#include <string> 
#include <memory> 
#include <mutex> 

#include "ClientSocket.h"
#include "ServerSocket.h"

class Publisher 
{ 
public:
    // TODO: static assertion to ensure compatible MsgT

    Publisher(const std::string& aTopicName);
    ~Publisher();
    bool advertise(); 


private: 
    void handleClient(int aClientFd); 

private:

    std::string mTopicName;
    ServerSocket mDataSocket;  
    std::vector<int> mSubscribers; 
    std::unique_ptr<ClientSocket> mRegistryConnection; 
    std::mutex mSubsMtx; 
    
};
#endif //PUBLISHER_H    