#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
 
#include <vector> 
#include <string> 
 
class ClientSocket 
{
public:
    ClientSocket(const std::string& serverIp, int serverPort);
    ~ClientSocket();

    bool sendMsg(const std::string& msg);

private:
    int mSocketFd{-1};
    bool sendAll(const void* data, size_t size);
};

#endif //CLIENTSOCKET_H 