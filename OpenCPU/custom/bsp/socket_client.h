#pragma once

#include "cm_asocket.h"
#include "cm_sim.h"
#include "sockets.h"
#include "stdint.h"

typedef struct
{
    void (*callback)(int sock, cm_asocket_event_e event, void *user_param);
    char* addr;
    uint16_t port;
}SocketInfo;


typedef struct{
    int (*receive)(void*, char*);
    int (*ssend)(void*, char*, int);
}SOCKETSERVER_IMPLEMENTS;

typedef struct{
    SOCKETSERVER_IMPLEMENTS interface;
    SocketInfo info;
    int socket;
    int (*init)(void*, SocketInfo);
}SOCKETSERVER;

SOCKETSERVER* SOCKETSERVER_CTOR(void);
void SOCKETSERVER_DTOR(SOCKETSERVER* t);

