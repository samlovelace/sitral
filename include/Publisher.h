#ifndef PUBLISHER_H
#define PUBLISHER_H
 
#include <string> 
 
class Publisher 
{ 
public:
    Publisher(const std::string& aTopicName);
    ~Publisher();

    bool advertise();
    void publish(const std::string& aMsg); 

private:
   
};
#endif //PUBLISHER_H    