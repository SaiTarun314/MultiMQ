#include "channel.h"
#include <iostream>

Channel::Channel(const std::string& name) : name(name), messages() { }

void Channel::produce(const std::string& message) {
    std::cout << "Added Message to Channel " << name << " -> " << message << std::endl;
    messages.emplace_back(message);
}

std::string Channel::consume() {
    if( messages.size() == 0 || offset < 0 ) return "";

    std::string message = messages.at(offset);
    // messages.pop_back();
    std::cout << "Message consumed from Channel " << name << " -> " << message << std::endl;
    ++offset;

    return message;
}

bool Channel::updateOffset(int& n) {
    offset += n;
    return true;
}
