#include <iostream> 
#include <memory>
#include <grpcpp/grpcpp.h>
#include "broker.h"

bool Broker::produce(const std::string& topic_name, const std::string& message) {    
    for(auto &channelInfo: topicChannels[topic_name]) {
        std::string partitionName = channelInfo.partition.partitionName;
        auto client = std::move(getStub(partitionName));

        PartitionProduceRequest req;
        PartitionProduceResponse res;
        req.set_channel_id(channelInfo.channelId);
        req.set_message(message);
        grpc::ClientContext client_context;

        grpc::Status status = client->Produce(&client_context, req, &res);
        res.success();
        if (!status.ok()) {
            return false;
        }
    }    
    
    return true;
}

std::string Broker::consume(const std::string& topic_name) {
    std::string message = ""; 
    for(auto &channelInfo: topicChannels[topic_name]) {
        std::string partitionName = channelInfo.partition.partitionName;
        auto client = std::move(getStub(partitionName));

        PartitionConsumeRequest req;
        PartitionConsumeResponse res;
        req.set_channel_id(channelInfo.channelId);
        grpc::ClientContext client_context;

        grpc::Status status = client->Consume(&client_context, req, &res);

        if (!status.ok()) {
            message = "";
            continue;
        }
        message = res.message();
    }

    return message;
}

bool Broker::createTopic(const std::string& topic_name, int& replication_factor) {
    if(topicChannels.find(topic_name) != topicChannels.end()) {
        return false;
    } else {
        std::vector<ChannelInfo> channels;
        replication_factor = std::max(replication_factor, (int)(partitions.size()));  // Ensuring the replication factor is set such that each partition has data of the topic
        for(int index=1; index<=replication_factor; ++index) {
            std::string channelName= topic_name+"-channel-"+ std::to_string(index);
            std::cout<<"Creating Channel - "<<channelName<<"\n";
            PartitionCreateChannelRequest req;
            PartitionCreateChannelResponse res;
            req.set_channel_id(channelName);
            grpc::ClientContext client_context;
            
            std::shared_ptr<PartitionService::Stub> selectedStub;
            PartitionInfo partitionInfo;
            
            if(index <= partitions.size()) {
                selectedStub = std::move(partitionStubs[partitions[index-1]]);
                partitionInfo.partitionName = partitions[index-1];
                partitionInfo.port = partitionPorts[partitions[index-1]];
            } else {
                int randomPartition = std::floor(random()*(partitions.size()));
                selectedStub = std::move(partitionStubs[partitions[randomPartition]]);
                partitionInfo.partitionName = partitions[randomPartition];
                partitionInfo.port = partitionPorts[partitions[randomPartition]];
            }
            ChannelInfo channelInfo(channelName, partitionInfo);
            std::cout<<"Calling Partition "<<partitionInfo.partitionName<<"\n";
            grpc::Status status = selectedStub->CreateChannel(&client_context, req, &res);
            
            // Add good retry mechanism and change the random partition logic in next feature release
            if(!status.ok()) {
                index--;
                continue;
            }
            
            channels.push_back(channelInfo);
        }
        topicChannels[topic_name] = channels;
        std::cout<<"Created Channels in all partitions \n";

        std::string leaderChannelName= topic_name+"-channel-1";
        PartitionInfo partitionInfo(partitions[0], partitionPorts[partitions[0]]);

        leaderChannels[leaderChannelName] = ChannelInfo(leaderChannelName, partitionInfo);
        return true;
    }
}

bool Broker::linkPartitionToBroker(const std::string& partitionName, int& partitionPort) {
    if(partitionPort < 0 || partitionPort > 9999 || partitionName.size() == 0) {
        return false;
    }
    
    bool isPresent = false;
    for (auto &&partition : partitions) {
        if(partition == partitionName){ 
            isPresent = true; break;
        }
    }
    
    if(partitionPorts.find(partitionName) != partitionPorts.end() || isPresent) {
        return false;
    }
    
    std::string address = "0.0.0.0:" + std::to_string(partitionPort);

    auto stub = PartitionService::NewStub(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
    
    // Check the Partition HealthCheck to check if the Partition exists and is healthy
    PartitionHealthCheckRequest req;
    PartitionHealthCheckResponse res;
    grpc::ClientContext client_context;

    grpc::Status status = stub->PartitionHealthCheck(&client_context, req, &res);

    if(!status.ok()) {
        return false;
    }

    partitions.push_back(partitionName);
    partitionPorts[partitionName] = partitionPort;
    partitionStubs[partitionName] = std::move(stub);

    return true;
}