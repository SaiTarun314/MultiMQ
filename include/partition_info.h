#ifndef PARTITION_INFO_H
#define PARTITION_INFO_H

#include<string>

struct PartitionInfo {
    std::string partitionName;
    std::string port;

    PartitionInfo() = default; 
    PartitionInfo(std::string name, std::string portNumber)
        : partitionName(std::move(name)), port(std::move(portNumber)) {}
};

struct ChannelInfo {
    std::string channelId;
    PartitionInfo partition;

    ChannelInfo() = default;
    ChannelInfo(std::string channelId, PartitionInfo partitionInfo) 
        : channelId(std::move(channelId)), partition(std::move(partitionInfo)) {}
};

#endif