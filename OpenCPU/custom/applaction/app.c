#include "app.h"
#include "drv_uart.h"
#include "drv_virt_at.h"
#include "button.h"
#include "drv_gpio.h"
#include "transceiver.h"
#include "display.h"
#include "socket_client.h"
#include "drv_socket.h"
#include "drv_audio.h"

#define DBG_NAME "app"

BUTTON* Button = NULL;
TRANSCEIVER* Transceiver = NULL;
SOCKETSERVER* SocketClient = NULL;

void dbg_th(void* argument)
{
    /*串口由于LOG功能,已在cm_opencpu_entry()开头初始化*/
    int readLen = 0 ;
    /* 创建信号量 */
	u0_uart_sem = osSemaphoreNew(1, 0, NULL);
    DBG_F("hmi_uart_sem ID: %d\r\n", u0_uart_sem);
    
    while(1){
        osSemaphoreAcquire(u0_uart_sem, osWaitForever);
        readLen = u0_uart_read(rxBuff);
        my_virt_at_test((unsigned char*)rxBuff, strlen(rxBuff));
        u0_printf("Send %s (%d)\r\n",rxBuff , readLen);
    }
}

void my_button_callback(void){
    void* msg = cm_malloc(1);
    ((uint8_t*)msg)[0] = 0x01; //对应通道宏定义
    osMessageQueuePut(button_click_queue, &msg, 0, 0);
}

void Button_Click_Thread(void* param)
{
    void* massgeQ = NULL;
    osStatus_t ret = osStatusReserved;
    int checkRet = BUTTON_RELEASED;
    ButtonInfo info = {
        .num = BUTTON_NUM,
        .pin = BUTTON_PIN,
        .mode = CM_GPIO_PULL_UP,
        .direction = CM_GPIO_DIRECTION_INPUT,
        .irq = CM_GPIO_IT_EDGE_RISING,
        .call_back = my_button_callback
    };

    Button = BUTTON_CTOR();
    Button->init(Button, info);
    BUTTON_IMPLEMENTS* buttonCtrl = (BUTTON_IMPLEMENTS*)Button;

    button_click_queue = osMessageQueueNew(4, sizeof(void*), NULL);

    while(1){
        ret = osMessageQueueGet(button_click_queue, &massgeQ, 0, 100);
        switch(ret)
        {
        case osOK:
            buttonCtrl->pressed(Button);
            cm_free(massgeQ);
            break;
        case osError:
            checkRet =  buttonCtrl->check(Button);
            if(checkRet != BUTTON_RELEASED){
                void* msg = cm_malloc(1);
                ((uint8_t*)msg)[0] = checkRet; //对应通道宏定义
                osMessageQueuePut(transceiver_queue, &msg, 0, 0);
            }
            break;
        default:
            break;
        }
    }
}


#define PHONE_PRESS_RED     0x01
#define PHONE_PRESS_GREEN   0x02
#define PHONE_INCOMING      0x03
#define PHONE_SET_TARGET    0x04

void Transceiver_Thread(void* param)
{
    void* massgeQ = NULL;
    TransceiverInfo info =  {
        .imei = "123456789012345",
        .imsi = "12345678901234567890",
        .targt.imei = "2234567890"
    };

    Transceiver  = TRANSCEIVER_CTOR();
    Transceiver->init(Transceiver, info);

    TRANSCEIVER_IMPLEMENTS* phone = (TRANSCEIVER_IMPLEMENTS*)Transceiver;

    transceiver_queue = osMessageQueueNew(4, sizeof(void*), NULL);

    my_amr_load_files();
    my_audio_init(100);

    my_audio_io_init();
    my_audio_io_sw(1);

    phone->online(Transceiver);

    while(1)
    {
        osMessageQueueGet(transceiver_queue, &massgeQ, 0, osWaitForever);
        switch(((uint8_t*)massgeQ)[0])
        {
        case PHONE_PRESS_GREEN:
            DBG_F("press green\r\n");
            phone->press_green(Transceiver);
            break;
        case PHONE_PRESS_RED:
            DBG_F("press red\r\n");
            phone->press_red(Transceiver);
            break;
        case PHONE_INCOMING:
            phone->incoming(Transceiver);
        case PHONE_SET_TARGET:{
            TargetInfo info;
            memcpy(&info, massgeQ+1, sizeof(TargetInfo));
            phone->set_targt(Transceiver, info);
            break;
        }
        default:
            break;
        }
        cm_free(massgeQ);
        screen_display(Transceiver);
    }
    
}

void socket_rev_callback(int sock, cm_asocket_event_e event, void *user_param)
{

}

void Socket_Client_Thread(void* param)
{
    void* massgeQ = NULL;
    SocketInfo info = {
        // .ip = "1234567890",
        .addr = "60.205.170.65",
        .port = 8107,
        .callback = socket_rev_callback
    };

    my_device_par();

    SocketClient = SOCKETSERVER_CTOR();

    // while(SocketClient->init(SocketClient, info) != 0){
    //     osDelayMs(1000);
    // }

    SOCKETSERVER_IMPLEMENTS* socketCtrl = (SOCKETSERVER_IMPLEMENTS*)SocketClient;

    socket_send_queue = osMessageQueueNew(4, sizeof(void*), NULL);

    while(1){
        osMessageQueueGet(socket_send_queue, &massgeQ, 0, osWaitForever);
        socketCtrl->ssend(SocketClient, massgeQ, 10);
        // socketCtrl->ssend()
        
    }
}

osThreadId_t osThreadCreat(const char * name,osThreadFunc_t func,osPriority_t priority,uint32_t stacksize)
{
	
	osThreadAttr_t thread_cfg =
	{
		/* 进程名 */
		.name = name,
        /* 优先级 */
		.priority = priority,
        /* 栈空间 */
		.stack_size = stacksize,
	};

    /* ML307A模组模组用户线程优先级必须低于或等于osPriorityNormal */
    if(osPriorityNormal > thread_cfg.priority){
        thread_cfg.priority = osPriorityNormal;
    }

	return osThreadNew(func,NULL,(const osThreadAttr_t *)&thread_cfg);

}

osTimerId_t osTimerCreat(const char * name,osTimerFunc_t func,osTimerType_t type)
{
    osTimerAttr_t timer_attr = 
    {
        .name = name,
    };
    return osTimerNew(func,type,NULL,&timer_attr);
}

void osDelayMs(uint32_t ms)
{
    /* ML307A心跳周期为5ms */
    osDelay(ms/5);
}