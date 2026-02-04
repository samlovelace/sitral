#ifndef PUBLISHER_H
#define PUBLISHER_H
 
#include <string> 
#include <memory>
#include <thread> 

#include "ClientSocket.h"

class Publisher 
{ 
public:
    // TODO: static assertion to ensure compatible MsgT

    Publisher(const std::string& aTopicName);
    ~Publisher();
    bool advertise(); 


private: 
    void clientAcceptLoop(); 

private:

    std::string mTopicName;
    int mDataSocket;  
    std::vector<int> mSubscribers; 
    std::unique_ptr<ClientSocket> mRegistryConnection; 
    std::thread mClientAcceptThread;
    
    //std::mutex mSubsMutex; 
    
};
#endif //PUBLISHER_H    