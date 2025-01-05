#ifndef CHANNEL_H
#define CHANNEL_H
#include <queue>
#include <string>

class Channel {
    private:
    std::deque<std::string> messages;       // This Dequeue will store the messages being produced in the channel
    std::string name;                       // Name of the Channel
    int offset = 0;                             // Stores the offeset of the queue channel till which the messages are consumed

    public:
    Channel(const std::string& name);
    const int& getOffset() const { return offset; } 
    void produce(const std::string& message);   // Function to add Messages to the Kakfa Channel
    std::string consume();                      // Funtion to consume Messages from the Kafka Channel
    bool updateOffset(int& n);                   // Function to update the offset
};

#endif