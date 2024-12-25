#include <iostream> 
#include <memory>
#include "broker.h"

void Broker::produce(const std::string& channelId, const std::string& message) {
    if (channels.find(channelId) == channels.end()) { 
        auto newChannel = std::make_shared<Channel>(channelId);
        channels[channelId] = newChannel;
    }
    auto channel = channels[channelId];
    channel->produce(message);
}


std::string Broker::consume(const std::string& channelId) {
    if (channels.find(channelId) == channels.end()) { 
        return "";
    }
    auto channel = channels[channelId];
    return channel->consume();
}