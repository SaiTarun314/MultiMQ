#include "channel.h"
#include <iostream>

Channel::Channel(const std::string& name) : name(name), messages() { }

void Channel::produce(const std::string& message) {
    std::cout << "Added Message to Channel " << name << " -> " << message << std::endl;
    messages.push(message);
}

std::string Channel::consume() {
    std::string message = messages.front();
    messages.pop();
    std::cout << "Message consumed from Channel " << name << " -> " << message << std::endl;

    return message;
}
