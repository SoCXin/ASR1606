#include "socket_client.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "drv_socket.h"
#include "drv_uart.h"

#define DBG_NAME "socket"

int socket_init(void* t, SocketInfo socket)
{
    int ret = 0;
    SOCKETSERVER* this = (SOCKETSERVER*)t;

    memcpy(&this->info, &socket, sizeof(SocketInfo));
    
    // my_tcp_init(&this->socket, (const char*)this->info.addr, this->info.port, this->info.callback);
    ret = my_tcp_init();
    return ret;
}

int socket_receive(void* t, char* data)
{
    int ret = 0;
    // SOCKETSERVER* this = (SOCKETSERVER*)t;

    DBG_F("Socket receive: %s\r\n", data);

    return ret;
}

int socket_send(void* t, char* data, int len)
{
    int ret = 0;
    SOCKETSERVER* this = (SOCKETSERVER*)t;

    my_socket_send(this->socket, data, len);
    DBG_F("Socket send: %s (%d)\r\n", data, len);

    return ret;
}

SOCKETSERVER* SOCKETSERVER_CTOR(void)
{
    SOCKETSERVER *this = (SOCKETSERVER*)malloc(sizeof(SOCKETSERVER));
    this->init = socket_init;
    this->interface.receive = socket_receive;
    this->interface.ssend = socket_send;
    return this;
}

void SOCKETSERVER_DTOR(SOCKETSERVER* t)
{
    free(t);
}

