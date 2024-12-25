#ifndef CHANNEL_H
#define CHANNEL_H
#include <queue>
#include <string>

class Channel {
    private:
    std::queue<std::string> messages;       // This Queue will store the messages being produced in the channel
    std::string name;                       // Name of the Channel

    public:
    Channel(const std::string& name);
    void produce(const std::string& message);   // Function to add Messages to the Kakfa Channel
    std::string consume();                      // Funtion to consume Messages from the Kafka Channel
};

#endif