
// #include "Publisher.h"
// #include "example.pb.h"

// #include <arpa/inet.h>

// Publisher::Publisher(const std::string& aTopicName) : 
//     mTopicName(aTopicName), 
//     mDataSocket(std::bind(&Publisher::handleClient, this, std::placeholders::_1))
// {

// }

// Publisher::~Publisher()
// {

// }
 
// bool Publisher::advertise()
// {   
//     std::string registryAddr = std::getenv("REGISTRY_ADDR");  // ip:port 
//     std::string serverIp = ""; 
//     int serverPort = -1;
    
//     size_t pos = registryAddr.find(':'); 
//     if(pos != std::string::npos)
//     {
//         serverIp = registryAddr.substr(0, pos); 
//         serverPort = std::stoi(registryAddr.substr(pos+1)); 
//     }

//     // connect to registry and register 
//     mRegistryConnection = std::make_unique<ClientSocket>(serverIp, serverPort);

//     sitral::registry::RegistryRequest msg;
//     auto* registerPub = msg.mutable_register_publisher();

//     registerPub->set_topic(mTopicName);
//     registerPub->set_type("default");
//     registerPub->set_port(ntohs(mDataSocket.getPort())); // tell the registry what port 

//     // TODO: test msg.SerializeAsString();
//     std::string serialized; 
//     msg.SerializeToString(&serialized);

//     // send msg
//     mRegistryConnection->sendMsg(serialized); 
//     sleep(3); 
//     return true; 
// }

// void Publisher::handleClient(int aClientFd)
// {
//     // TODO: error checking to make sure one doesnt already exist? 
//     std::lock_guard<std::mutex> lock(mSubsMtx); 
//     mSubscribers.push_back(aClientFd); 
// }
