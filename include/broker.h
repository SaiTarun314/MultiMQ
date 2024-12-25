#ifndef BROKER_H
#define BROKER_H

#include <map>
#include <string>
#include "channel.h"
#include <memory>

class Broker {
    private:
    std::map<std::string, std::shared_ptr<Channel>> channels;         // HashMap which stores the mapping between Channel Id and the linked Channel shared_ptr
    
    public:
    void produce(const std::string& channelId, const std::string& message);   // Function to add Messages to the Kakfa
    std::string consume(const std::string& channelId);                      // Funtion to consume Messages from the Kafka
};

#endif