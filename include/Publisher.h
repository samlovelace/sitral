#ifndef PUBLISHER_H
#define PUBLISHER_H
 
#include <string> 
#include <memory>
#include <thread> 

#include "ClientSocket.h"

class Publisher 
{ 
public:
    Publisher(const std::string& aTopicName);
    ~Publisher();

    bool advertise();
    void publish(const std::string& aMsg); 


private: 
    void clientAcceptLoop(); 

private:

    std::string mTopicName;
    int mDataSocket;  
    std::vector<int> mSubscribers; 
    std::unique_ptr<ClientSocket> mRegistryConnection; 
    std::thread mClientAcceptThread; 
    
};
#endif //PUBLISHER_H    