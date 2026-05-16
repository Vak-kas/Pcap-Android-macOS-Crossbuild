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


void handleMonitorOff(const char* nic)
{
    (void)nic;

    if (g_running)
    {
        handleStopCapture();
    }

    int ret = system(
        "if [ -f /system/lib64/libfakeioctl.so ]; then "
        "export LD_PRELOAD=/system/lib64/libfakeioctl.so; "
        "else export LD_PRELOAD=/system/lib/libfakeioctl.so; fi; "
        "nexutil -m0 > /dev/null 2>&1"
    );

    system("svc wifi disable");
    sleep(1);
    system("svc wifi enable");

    if (ret != 0)
    {
        sendError("Failed to disable monitor mode");
        return;
    }

    sendLog("Monitor mode disabled");
}

void handleMonitorOn(const char* nic)
{
    (void)nic;

    if (!isMonitorModeSupported())
    {
        sendError("Monitor mode is not supported: libfakeioctl.so not found");
        return;
    }

    if (g_running)
    {
        handleStopCapture();
    }

    int ret = system(
        "if [ -f /system/lib64/libfakeioctl.so ]; then "
        "export LD_PRELOAD=/system/lib64/libfakeioctl.so; "
        "else export LD_PRELOAD=/system/lib/libfakeioctl.so; fi; "
        "nexutil -m2 > /dev/null 2>&1"
    );

    if (ret != 0)
    {
        sendError("Failed to enable monitor mode");
        return;
    }

    sendLog("Monitor mode enabled");
}

bool isMonitorModeSupported()
{
    if (access("/system/bin/nexutil", X_OK) != 0 &&
        access("/system/xbin/nexutil", X_OK) != 0 &&
        access("/data/local/tmp/nexutil", X_OK) != 0)
    {
        return false;
    }

    if (access("/system/lib/libfakeioctl.so", R_OK) != 0)
    {
        return false;
    }

    return true;
}

void sendLog(const char* msg)
{
    MsgHeader header;
    header.type = MSG_LOG;
    header.length = strlen(msg);

    fwrite(&header, sizeof(header), 1, stdout);
    fwrite(msg, header.length, 1, stdout);
    fflush(stdout);
}

void sendError(const char* msg)
{
    MsgHeader header;
    header.type = MSG_ERR;
    header.length = strlen(msg);

    fwrite(&header, sizeof(header), 1, stdout);
    fwrite(msg, header.length, 1, stdout);
    fflush(stdout);
}