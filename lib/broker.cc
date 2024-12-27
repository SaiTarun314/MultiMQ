#include <iostream> 
#include <memory>
#include "broker.h"

bool Broker::produce(const std::string& topic_name, const std::string& message) {
    std::string channelName = leaderNodes[topic_name];
    if (channels.find(channelName) == channels.end()) { 
        return false;
    }

    auto channel = channels[channelName];
    channel->produce(message);

    return true;
}


std::string Broker::consume(const std::string& topic_name) {
    std::string channelName = leaderNodes[topic_name];
    if (channels.find(channelName) == channels.end()) { 
        return "";
    }
    auto channel = channels[channelName];
    return channel->consume();
}

bool Broker::createTopic(const std::string& topic_name) {
    if(topics.find(topic_name) != topics.end()) {
        return false;
    } else {
        std::string channelName= topic_name+"-channel-1";
        auto newChannel = std::make_shared<Channel>(channelName);
        channels[channelName] = newChannel;
        topics[topic_name].push_back(channelName);
        leaderNodes[topic_name] = channelName;

        return true;
    }
}