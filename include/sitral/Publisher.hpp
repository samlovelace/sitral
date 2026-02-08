#pragma once

#include <string>
#include <type_traits>
#include <google/protobuf/message.h>

#include "sitral/ClientSocket.h"
#include "sitral/BroadcastServer.h"
#include "proto/RegistryRequest.pb.h"

namespace sitral {

template<typename MsgT>
class Publisher
{
    static_assert(
        std::is_base_of_v<google::protobuf::Message, MsgT>,
        "MsgT must derive from google::protobuf::Message"
    );

public:
    explicit Publisher(const std::string& topic)
        : mTopic(topic),
          mDataSocket()
    {}

    ~Publisher() {}

    bool advertise()
    {
        auto [ip, port] = parseRegistryAddr();

        mRegistry = std::make_unique<ClientSocket>(ip, port);

        sitral::registry::RegistryRequest req;
        auto* reg = req.mutable_register_publisher();
        reg->set_topic(mTopic);
        reg->set_type(MsgT::descriptor()->full_name());
        reg->set_port(mDataSocket.getPort());

        std::string buf;
        req.SerializeToString(&buf);
        return mRegistry->sendMsg(buf);
    }

    void publish(const MsgT& msg)
    {
        std::string buf;
        msg.SerializeToString(&buf);
        mDataSocket.broadcast(buf);
    }

private:
    std::tuple<std::string, int> parseRegistryAddr()
    {
        const char* env = std::getenv("REGISTRY_ADDR");
        if (!env) {
            return {"127.0.0.1", 5555};
        }

        std::string addr(env);
        auto pos = addr.find(':');
        return {
            addr.substr(0, pos),
            std::stoi(addr.substr(pos + 1))
        };
    }

private:
    std::string mTopic;
    BroadcastServer mDataSocket;
    std::unique_ptr<ClientSocket> mRegistry;
};

} // namespace sitral
