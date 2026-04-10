#pragma once
#include <stdlib.h>
#include <pcap.h>

struct MsgHeader
{
    uint32_t type;
    uint32_t length;
}__attribute__((packed));

#define MSG_NIC  1
#define MSG_PKT  2
#define MSG_LOG  3
#define MSG_ERR  4


extern pcap_t* g_pcap;
extern bool g_running;
extern pid_t g_child_pid;
