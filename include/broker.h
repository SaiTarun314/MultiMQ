#ifndef BROKER_H
#define BROKER_H

#include <map>
#include <string>
#include "channel.h"
#include <memory>

class Broker {
    private:
    std::map<std::string, std::shared_ptr<Channel>> channels;           // HashMap which stores the mapping between Channel Id and the linked Channel shared_ptr
    std::map<std::string, std::vector<std::string>> topics;             // HashMap which stores the mapping between each topic and its associated channel ids
    std::map<std::string, std::string> leaderNodes;                     // HashMap which stores the current Leader Node for a specific topic
    
    public:
    bool createTopic(const std::string& topic_name);                      // Funtion to create a new topic 
    bool produce(const std::string& channelId, const std::string& message);   // Function to add Messages to the MQ
    std::string consume(const std::string& channelId);                      // Funtion to consume Messages from the MQ
    
};

#endif