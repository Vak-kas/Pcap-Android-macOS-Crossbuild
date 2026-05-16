#include <pcap.h>
#include <stdio.h>
#include "../common.h"
#include <string.h>
#include "../pcapmanager.h"
#include <unistd.h>   // fork()
#include <sys/types.h> // pid_t
#include <signal.h>   // kill()
#include <sys/wait.h> // waitpid()


void handleGetNIC();
void sendNIC(const char* name);
void handleStartCapture(const char* nicName);
void handleStopCapture();
void handlePacket(const struct pcap_pkthdr* header, const u_char* packet);
void packetHandler(u_char* user, const struct pcap_pkthdr* header, const u_char* packet);

void handleMonitorOn(const char* nic);
void handleMonitorOff(const char* nic);
bool isMonitorModeSupported();
void sendLog(const char* msg);
void sendError(const char* msg);