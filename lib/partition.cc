#include "partition.h"

bool Partition::createChannel(const std::string& channelId) {
    if(channels.find(channelId) != channels.end()) {
        return false;
    }
    auto newChannel = std::make_shared<Channel>(channelId);
    channels[channelId] = newChannel;
    
    return true;
}

bool Partition::produce(const std::string& channelId, const std::string& message) {
    if (channels.find(channelId) == channels.end()) {
        return false;
    } 

    auto channel = channels[channelId];
    channel->produce(message);
    return true;
}  

std::string Partition::consume(const std::string& channelId) {
    if (channels.find(channelId) == channels.end()) {
        return "";
    } 

    auto channel = channels[channelId];
    return channel->consume();
}

bool Partition::updateChannelOffset(const std::string& channelId, int& offset) {
    if(channels.find(channelId) != channels.end()) {
        auto channel = channels[channelId];
        return channel->updateOffset(offset);
    } else {
        return false;
    }
}
bool Partition::partitionHealthCheck() {
    // Implement in next feature
    return true;
}

bool Partition::channelHealthCheck(const std::string& channelId) {
    if (channels.find(channelId) != channels.end()) {
        auto channel = channels[channelId];
        return channel->getOffset() >= 0 ? true : false;    // Check if the channel is ready for consuming messages in case of partition failure
    } 

    return false;
}