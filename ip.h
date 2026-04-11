#pragma once
#include <stdint.h>
#include <arpa/inet.h>
#include <string>
struct Ip
{
    static constexpr size_t SIZE = 4;
private:
    uint32_t ip_addr_;

public:
    Ip();
    Ip(uint32_t ip_addr) : ip_addr_(ip_addr) {} //network byte order
    Ip(const std::string& ip_str);
    uint32_t addr() const { return ntohl(ip_addr_); }

    explicit operator uint32_t() const { return ntohl(ip_addr_); }
    explicit operator std::string() const;

    bool operator==(const Ip& other) const { return ip_addr_ == other.ip_addr_; }
    bool operator!=(const Ip& other) const { return ip_addr_ != other.ip_addr_; }

    std::string toString() const;

    Ip getDefaultGateway(std::string device) const;
};
