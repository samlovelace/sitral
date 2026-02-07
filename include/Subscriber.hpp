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

    // TODO: eventually this function should handle or dispatch for any generic response from the registry 
    void registryCallback(const std::string& aMsg)
    {
        std::cout << "Recvd response from registry\n";

        sitral::registry::RegistryResponse resp;
        if (!resp.ParseFromString(aMsg)) {
            std::cerr << "Failed to parse RegistryResponse\n";
            return;
        }

        if (resp.msg_case() !=
            sitral::registry::RegistryResponse::kQueryPublishersResponse) {
            std::cerr << "RegistryResponse is not QueryPublishersResponse\n";
            return;
        }

        const auto& pubs =
            resp.query_publishers_response().publishers();

        for (const auto& p : pubs) {
            std::cout << "Ip: " << p.ip()
                    << " Port: " << p.port() << "\n";
        }

        // connect to a publisher 
        auto pub = pubs.at(0);
        
        mDataConnection = std::make_unique<ClientSocket>(pub.ip(), pub.port()); 
        mDataConnection->setReceiveCallback(std::bind(&Subscriber::subscriptionCallback, this, std::placeholders::_1));
    }

    void subscriptionCallback(const std::string& aMsg)
    {
        std::cout << "subscription callback\n"; 

        MsgT msg; 
        if (!msg.ParseFromString(aMsg)) {
            std::cerr << "Failed to parse...\n";
            return;
        }

        if(mCallback)
        {
            mCallback(msg); 
        }
    }

private:
    std::unique_ptr<ClientSocket> mRegistryConnection; 
    std::unique_ptr<ClientSocket> mDataConnection; 

    std::string mTopicName; 
    Callback mCallback; 
   
};
#endif //SUBSCRIBER_HPP