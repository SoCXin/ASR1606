#pragma once

typedef enum{
    TRANSMITTER_OFFLINE = 0x00,
    TRANSMITTER_IDLE,
    TRANSMITTER_DIAL,
    TRANSCEIVER_BUSY_LINE,
    TRANSCEIVER_INCOMING
}transceiverStatus;

typedef struct{
    char imei[15];
}TargetInfo;
 
typedef struct{
    char imei[15];
    char imsi[20];
    TargetInfo targt;
}TransceiverInfo;

typedef struct
{
    int (*incoming)(void*);
    int (*press_green)(void*);
    int (*press_red)(void*);
    transceiverStatus (*get_status)(void*);
    int (*online)(void*);
    int (*set_targt)(void*, TargetInfo);
}TRANSCEIVER_IMPLEMENTS;

typedef struct
{
    TRANSCEIVER_IMPLEMENTS interface;
    transceiverStatus status;
    TransceiverInfo info;
    int (*dial)(void*);
    int (*answer)(void*);
    int (*cancel)(void*);
    int (*reject)(void*);
    int (*hangup)(void*);
    int (*ring)(void*, const char*);
    int (*init)(void*, TransceiverInfo);
}TRANSCEIVER;

TRANSCEIVER* TRANSCEIVER_CTOR(void);
void TRANSCEIVER_DTOR(TRANSCEIVER* t);

