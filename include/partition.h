#ifndef PARTITION_H
#define PARTITION_H

#include <map>
#include <string>
#include <memory>
#include "channel.h"

class Partition {
private: 
    std::string partitionName;                              
    std::map<std::string, std::shared_ptr<Channel>> channels;               // HashMap which stores the mapping between Channel Id and the linked Channel shared_ptr
public:
    Partition(const std::string& partitionName) : partitionName(partitionName), channels() {}
    
    const std::string& getPartitionName() const { return partitionName; }

    bool createChannel(const std::string& channelId);                       // Function to create a new Channel in the partition
    bool produce(const std::string& channelId, const std::string& message);   // Function to add Messages to the MQ
    std::string consume(const std::string& channelId);                      // Funtion to consume Messages from the MQ
    bool updateChannelOffset(const std::string& channelId, int& offset);    // Function to update the offset of a channel
    bool partitionHealthCheck();                                            // Funtion to check if the Partition is Healthy
    bool channelHealthCheck(const std::string& channelId);                   // Funtion to check if a specific channel in the Partition is Healthy
};

#endif