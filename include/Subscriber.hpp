#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP
 
#include <memory> 

#include "ClientSocket.h"
#include "RegistryRequest.pb.h"
 
template<typename MsgT> 
class Subscriber 
{ 
public:
    using Callback = std::function<void(const MsgT&)>; 

    Subscriber(const std::string& aTopicName, Callback aCallback) 
        : mTopicName(aTopicName), mCallback(aCallback), mRegistryConnection(nullptr), 
          mDataConnection(nullptr)
    {
        auto [serverIp, serverPort] = parseRegistryAddr();

        mRegistryConnection = std::make_unique<ClientSocket>(
            serverIp, serverPort);

        mRegistryConnection->setReceiveCallback(std::bind(&Subscriber::registryCallback, this, std::placeholders::_1)); 

        sitral::registry::RegistryRequest query;
        auto* reg = query.mutable_query_publishers(); 
        reg->set_topic(mTopicName);

        std::string serialized;
        query.SerializeToString(&serialized);

        mRegistryConnection->sendMsg(serialized);
    }

    ~Subscriber()
    {

    }

private:
    
    // TODO: copied this from publisher, generalize to single location 
    std::tuple<std::string, int> parseRegistryAddr()
    {
        std::string registryAddr = std::getenv("REGISTRY_ADDR");  // ip:port 
        std::string serverIp = ""; 
        int serverPort = -1;
        
        size_t pos = registryAddr.find(':'); 
        if(pos != std::string::npos)
        {
            serverIp = registryAddr.substr(0, pos); 
            serverPort = std::stoi(registryAddr.substr(pos+1)); 
        }

        return {serverIp, serverPort}; 
    }

    // TODO: eventually this function should 
    void registryCallback(const std::string& aMsg)
    {
        std::cout << "Recvd: " << aMsg << std::endl;  
    }

private:
    std::unique_ptr<ClientSocket> mRegistryConnection; 
    std::unique_ptr<ClientSocket> mDataConnection; 

    std::string mTopicName; 
    Callback mCallback; 
   
};
#endif //SUBSCRIBER_HPP