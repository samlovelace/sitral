#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H
 
#include <functional>
#include <thread> 

class ServerSocket 
{ 
public:
    ServerSocket(std::function<void(int)> aClientHandler, int aPort = 0);
    ~ServerSocket();

private: 
    void clientHandleLoop(); 

private:

    std::function<void(int)> mClientHandler; 
    int mSocketFd; 
    std::thread mClientHandleThread; 

   
};
#endif //SERVERSOCKET_H