#include "ip.h"

Ip::Ip() : ip_addr_(0) {}

Ip::operator std::string() const {
    uint32_t ip_ = addr();
    char buffer[32];
    sprintf(buffer, "%u.%u.%u.%u", (ip_ >> 24) & 0xFF, (ip_ >> 16) & 0xFF, (ip_ >> 8) & 0xFF, ip_ & 0xFF);
    return std::string(buffer);
}


Ip::Ip(const std::string& ip_str) {
    unsigned int bytes[4] = {0};
    if(sscanf(ip_str.c_str(), "%u.%u.%u.%u", &bytes[0], &bytes[1], &bytes[2], &bytes[3]) !=  SIZE)
    {
        fprintf(stderr, "Invalid IP string format: %s\n", ip_str.c_str());
        exit(1);
    }
    ip_addr_ = htonl((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
}

Ip Ip::getDefaultGateway(std::string device) const
{
    std::string cmd = "ip route show dev " + device + " | awk '$1==\"default\" && $2==\"via\" {print $3; exit}'";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe)
    {
        fprintf(stderr, "popen failed!\n");
        exit(1);
    }
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        std::string ip_str(buffer);
        ip_str.pop_back();
        pclose(pipe);
        return Ip(ip_str);
    }
    //error case
    fprintf(stderr, "Failed to get default gateway for device: %s\n", device.c_str());
    pclose(pipe);
    exit(1);
}
