#ifndef PUBLISHER_H
#define PUBLISHER_H
 
#include <string> 
#include <memory> 
#include <mutex> 

#include "RegistryRequest.pb.h"

#include "ClientSocket.h"
#include "BroadcastServer.h"

template<typename MsgT>
class Publisher
{
    static_assert(
        std::is_same_v<
            decltype(std::declval<MsgT>().SerializeToString(
                static_cast<std::string*>(nullptr))),
            bool>,
        "MsgT must be a protobuf message"
    );

public:
    Publisher(const std::string& topic)
        : mTopicName(topic),
          mDataSocket(/* no client handler needed */)
    {}

    bool advertise()
    {
        auto [serverIp, serverPort] = parseRegistryAddr();

        mRegistryConnection = std::make_unique<ClientSocket>(
            serverIp, serverPort);

        sitral::registry::RegistryRequest msg;
        auto* reg = msg.mutable_register_publisher();
        reg->set_topic(mTopicName);
        reg->set_type("default");
        reg->set_port(mDataSocket.getPort());

        std::string serialized;
        msg.SerializeToString(&serialized);

        return mRegistryConnection->sendMsg(serialized);
    }

    void publish(const MsgT& msg)
    {
        std::string serialized;
        msg.SerializeToString(&serialized);
        mDataSocket.broadcast(serialized);
    }

private: 
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

private:
    std::string mTopicName;
    BroadcastServer mDataSocket;
    std::unique_ptr<ClientSocket> mRegistryConnection;
};
#endif //PUBLISHER_H    