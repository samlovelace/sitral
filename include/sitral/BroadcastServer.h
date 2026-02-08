#ifndef BROADCASTSERVER_H
#define BROADCASTSERVER_H

#include <mutex>
#include "ServerSocket.h"

namespace sitral
{

class BroadcastServer : public ServerSocket
{ 
public:
    BroadcastServer();  
    ~BroadcastServer();  
    
    void broadcast(const std::string& aSerializedMsg); 

private:

    void handleClient(int aClientFd); 

private: 
    std::mutex mClientsMtx; 
    std::vector<int> mClients; 
   
};

} // namespace sitral
#endif //BROADCASTSERVER_H