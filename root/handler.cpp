#include "handler.h"

void handleGetNIC()
{
    std::vector<std::string> nicList = getNICList();
    for (const auto& nic : nicList)
    {
        sendNIC(nic.c_str());
    }
        fflush(stdout);
}

void sendNIC(const char* name)
{
    MsgHeader header;
    header.type = MSG_NIC;
    header.length = strlen(name);

    fwrite(&header, sizeof(header), 1, stdout);
    fwrite(name, header.length, 1, stdout);
    fflush(stdout);
}

void handleStartCapture(const char* nicName)
{
    if (g_running)
    {
        return;
    }

    g_pcap = initPcap(nicName);
    if (!g_pcap) 
    {
        return;
    }
    g_running = true;

    g_child_pid = fork();

    if (g_child_pid == 0)
    {
        pcap_loop(g_pcap, -1, packetHandler, NULL);
        exit(0);
    }
    else if (g_child_pid < 0)
    {
        perror("fork failed");
        g_running = false;
    }
    else
    {
        // parent는 그냥 리턴 (명령 계속 받음)
    }
}


void handleStopCapture()
{
    if (g_running && g_child_pid > 0)
    {
        kill(g_child_pid, SIGTERM);
        waitpid(g_child_pid, NULL, 0);

        g_child_pid = -1;
    }

    if (g_pcap)
    {
        closePcap(g_pcap);
        g_pcap = nullptr;
    }

    g_running = false;
}

void handlePacket(const struct pcap_pkthdr* header, const u_char* packet)
{
    MsgHeader msgHeader;
    msgHeader.type = MSG_PKT;
    msgHeader.length = header->caplen;

    //원본 패킷 그대로 전송(임시)
    fwrite(&msgHeader, sizeof(msgHeader), 1, stdout);
    fwrite(packet, header->caplen, 1, stdout);
    fflush(stdout);
}

void packetHandler(u_char* user, const struct pcap_pkthdr* header, const u_char* packet)
{
    handlePacket(header, packet);
}