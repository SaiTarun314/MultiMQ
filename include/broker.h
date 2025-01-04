#ifndef BROKER_H
#define BROKER_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "proto/partition.grpc.pb.h"
#include "partition_info.h"

class Broker {
private:
    std::map<std::string, std::vector<ChannelInfo>> topicChannels;      // HashMap which stores the mapping between each topic and its associated Channels
    std::map<std::string, ChannelInfo> leaderChannels;                  // HashMap which stores the current Leader Channel for a specific topic
    std::map<std::string, std::shared_ptr<PartitionService::Stub>> partitionStubs;       // HashMap which stores the Partition Stub for each partition
    std::map<std::string, std::string> partitionPorts;                  // HashMap which stores all the port information for each partition
    std::vector<std::string> partitions;                                // Vector which stores all the partitions
    
public:
    std::shared_ptr<PartitionService::Stub> getStub(const std::string& partitionName) { 
        return partitionStubs[partitionName];
    }

    bool createTopic(const std::string& topic_name, int& replication_factor);       // Funtion to create a new topic 
    bool produce(const std::string& channelId, const std::string& message);         // Function to add Messages to the MQ
    std::string consume(const std::string& channelId);                              // Funtion to consume Messages from the MQ
    bool linkPartitionToBroker(const std::string& partitionName, int& partitionPort); // Funtion to link a Partition
    
};

#endif