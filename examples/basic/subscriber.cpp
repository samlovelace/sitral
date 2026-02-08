
#include "sitral/Subscriber.hpp"

#include "Test.pb.h"
#include <iostream> 

void callback(const sitral::testing::Test& aMsg)
{
    std::cout << "Recvd: " << aMsg.words() << " " << aMsg.number() << "\n"; 
}

int main()
{
    std::string topicName = "testing"; 
    sitral::Subscriber<sitral::testing::Test> s(topicName, callback); 

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }
}