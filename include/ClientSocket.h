#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
 
#include <vector> 
#include <string> 
#include <atomic> 
#include <thread> 
#include <functional>
 
class ClientSocket 
{
public:
    using RawCallback = std::function<void(const std::string&)>;

    ClientSocket(const std::string& ip, int port);
    ~ClientSocket();

    bool sendMsg(const std::string& msg);

    void setReceiveCallback(RawCallback cb);

private:
    void recvLoop();
    bool recvAll(void* data, size_t size);
    bool sendAll(const void* data, size_t size);

private:
    int mSocketFd{-1};
    std::thread mRecvThread;
    RawCallback mCallback;
    std::atomic<bool> mRunning{false};
};
#endif //CLIENTSOCKET_H 