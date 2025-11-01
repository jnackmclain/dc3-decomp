#pragma once
#include "os/NetworkSocket.h"

bool UsingHolmes(int);
int HolmesClientSysExec(const char *);
void HolmesClientReInit();
void HolmesClientPrint(const char *);
void HolmesClientTerminate();
void HolmesClientInit();
NetAddress HolmesResolveIP();
void HolmesClientPollKeyboard();
