
#include "Publisher.h"
#include "Test.pb.h"

int main()
{
    Publisher<sitral::testing::Test> p("testing"); 
    p.advertise(); 

    sitral::testing::Test msg; 
    msg.set_words("Hello, World"); 

    int idx = -1;

    while(true)
    {
        msg.set_number(idx++); 
        p.publish(msg); 
        std::this_thread::sleep_for(std::chrono::seconds(1));  
    }
}