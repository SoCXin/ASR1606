/*
 * 这个例程适用于`Linux`这类支持pthread的POSIX设备, 它演示了用SDK配置MQTT参数并建立连接, 之后创建3个线程
 *
 * + 一个线程用于保活长连接
 * + 一个线程用于接收消息, 并在有消息到达时进入默认的数据回调, 在连接状态变化时进入事件回调
 * + 一个线程用于从网络上HTTP下载待升级的固件, 这个线程由接收消息线程得到OTA升级的MQTT消息后启动
 *
 * 需要用户关注或修改的部分, 已用 `TODO` 在注释中标明
 *
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
#include "aiot_ota_api.h"
#include "aiot_mqtt_api.h"

#include "cm_os.h"
#include "cm_sys.h"
#include "cm_demo_uart.h"

#define CM_ALIYUN_OTA_OK 0
/** 服务器OTA升级方式选择"云端主动推送升级" */
//#define CM_ALIYUN_OTA_ACTIVE_PUSH
/** 阿里云OTA消息事件ID*/
typedef enum
{
    CM_ALIYUN_OTA_UPGRADE_START = 0,
    CM_ALIYUN_OTA_CHECK_FAIL,
    CM_ALIYUN_OTA_UPGRADE_UNKNOWN
} cm_aliyun_ota_event_e;

/** 阿里云OTA队列消息*/
typedef struct{
    cm_aliyun_ota_event_e event; //事件ID
    void *arg;                   //用户参数
} cm_aliyun_ota_msg_t;
static osMessageQueueId_t s_aliyun_ota_queue = NULL;


/* TODO: 替换为自己设备的三元组 */
static char *product_key       = "${YourProductKey}";
static char *device_name       = "${YourDeviceName}";
static char *device_secret     = "${YourDeviceSecret}";
#define printf(...) do{ cm_demo_printf("Line[%d]: ",__LINE__); cm_demo_printf(__VA_ARGS__); cm_demo_printf("\r\n");}while(0);

/*
    TODO: 替换为自己实例的接入点

    对于企业实例, 或者2021年07月30日之后（含当日）开通的物联网平台服务下公共实例
    mqtt_host的格式为"${YourInstanceId}.mqtt.iothub.aliyuncs.com"
    其中${YourInstanceId}: 请替换为您企业/公共实例的Id

    对于2021年07月30日之前（不含当日）开通的物联网平台服务下公共实例
    需要将mqtt_host修改为: mqtt_host = "${YourProductKey}.iot-as-mqtt.${YourRegionId}.aliyuncs.com"
    其中, ${YourProductKey}：请替换为设备所属产品的ProductKey。可登录物联网平台控制台，在对应实例的设备详情页获取。
    ${YourRegionId}：请替换为您的物联网平台设备所在地域代码, 比如cn-shanghai等
    该情况下完整mqtt_host举例: a1TTmBPIChA.iot-as-mqtt.cn-shanghai.aliyuncs.com

    详情请见: https://help.aliyun.com/document_detail/147356.html
*/
static char  *mqtt_host = "${YourInstanceId}.mqtt.iothub.aliyuncs.com";

/* 位于portfiles/aiot_port文件夹下的系统适配函数集合 */
extern aiot_sysdep_portfile_t g_aiot_sysdep_portfile;

/* 位于external/ali_ca_cert.c中的服务器证书 */
extern const char *ali_ca_cert;

static osThreadId_t g_mqtt_process_thread = NULL; /* 用于MQTT的长连接保活线程 */
static void __cm_aliyun_ota_response(int res);

/* TODO: 如果要关闭日志, 就把这个函数实现为空, 如果要减少日志, 可根据code选择不打印
 *
 * 例如: [1578463098.611][LK-0309] pub: /ota/device/upgrade/a13FN5TplKq/ota_demo
 *
 * 上面这条日志的code就是0309(十六进制), code值的定义见core/aiot_state_api.h
 *
 */

/* 日志回调函数, SDK的日志会从这里输出 */
static int32_t __demo_state_logcb(int32_t code, char *message)
{
    /* 下载固件的时候会有大量的HTTP收包日志, 通过code筛选出来关闭 */
    if (STATE_HTTP_LOG_RECV_CONTENT != code) {
        //printf("%s", message);
    }
    return 0;
}

/* 用户通过 aiot_ota_setopt() 注册的OTA消息处理回调, 如果SDK收到了OTA相关的MQTT消息, 会自动识别, 调用这个回调函数 */
static void __demo_ota_recv_handler(void *ota_handle, aiot_ota_recv_t *ota_msg, void *userdata)
{
    switch (ota_msg->type) {
    case AIOT_OTARECV_FOTA: {

        if (NULL == ota_msg->task_desc || ota_msg->task_desc->protocol_type != AIOT_OTA_PROTOCOL_HTTPS) {
            break;
        }
        printf("OTA target firmware version: %s, size: %u Bytes \r\n", ota_msg->task_desc->version,
               ota_msg->task_desc->size_total);

        /*
        *   非常重要!!!
        *
        *   该部分为ASR从服务器下载ota升级文件并执行ota的接口调用步骤,用户请不要修改
        *
        *   1. 使用cm_fota_set_url接口配置阿里云服务器OTA文件下载url,该url配置掉电保存;
        *   2. 使用cm_fota_exec_upgrade接口执行升级;
        * 
        *    注意:
        *    1. 执行升级后模组会进行多次重启,且升级异常后无法还原至升级前的版本,请在FOTA升级前先使用单个模组调试,保证升级无异常后再进行大规模升级;
        *    2. 执行升级后url有效期为24小时,且升级异常后无法还原至升级前的版本,模组必须在升级前保证模组的网络,电源,SIM流量剩余等,防止升级失败无法恢复;
        *
        */
        extern int cm_fota_set_url(char *url);
        extern int cm_fota_exec_upgrade(void);
        extern void cm_fota_res_callback_register(void *cb);

        /** 底层注册回调函数,ASR大系统验证FOTA url有效性结果反馈 */
        cm_fota_res_callback_register(__cm_aliyun_ota_response);

        int ret = 0;
        ret = cm_fota_set_url(ota_msg->task_desc->url);
        if(ret != 0)
        {
            printf("set url err\r\n");
            return;
        }

        printf("set url: %s\r\n", ota_msg->task_desc->url);
        osDelay(2);  //延迟2s

        /** 
         * 芯片执行ota升级任务,芯片将首先验证url有消息,理论上有效会重启进入最小系统并执行ota包下载与升级,
         * 失败则会通过
         */
        ret = cm_fota_exec_upgrade();
        if(ret != 0)
        {
            printf("exec upgrade err\r\n");
            return;
        }

        /** 芯片执行ota升级任务,通知关闭连接并释放资源 */
        if(s_aliyun_ota_queue)
        {
            cm_aliyun_ota_msg_t ota_data = {0};
            ota_data.event = CM_ALIYUN_OTA_UPGRADE_START;
            osMessageQueuePut(s_aliyun_ota_queue, &ota_data, 0, 0x0U);
        }
        break;
    }

    default:
        break;
    }
}

/* MQTT事件回调函数, 当网络连接/重连/断开时被触发, 事件定义见core/aiot_mqtt_api.h */
static void __demo_mqtt_event_handler(void *handle, const aiot_mqtt_event_t *event, void *userdata)
{
    switch (event->type) {
    /* SDK因为用户调用了aiot_mqtt_connect()接口, 与mqtt服务器建立连接已成功 */
    case AIOT_MQTTEVT_CONNECT: {
        printf("AIOT_MQTTEVT_CONNECT\r\n");
        /* TODO: 处理SDK建连成功, 不可以在这里调用耗时较长的阻塞函数 */
    }
    break;

    /* SDK因为网络状况被动断连后, 自动发起重连已成功 */
    case AIOT_MQTTEVT_RECONNECT: {
        printf("AIOT_MQTTEVT_RECONNECT\r\n");
        /* TODO: 处理SDK重连成功, 不可以在这里调用耗时较长的阻塞函数 */
    }
    break;

    /* SDK因为网络的状况而被动断开了连接, network是底层读写失败, heartbeat是没有按预期得到服务端心跳应答 */
    case AIOT_MQTTEVT_DISCONNECT: {
        char *cause = (event->data.disconnect == AIOT_MQTTDISCONNEVT_NETWORK_DISCONNECT) ? ("network disconnect") :
                      ("heartbeat disconnect");
        printf("AIOT_MQTTEVT_DISCONNECT: %s\r\n", cause);
        /* TODO: 处理SDK被动断连, 不可以在这里调用耗时较长的阻塞函数 */
    }
    break;

    default: {

    }
    }
}

/* MQTT默认消息处理回调, 当SDK从服务器收到MQTT消息时, 且无对应用户回调处理时被调用 */
static void __demo_mqtt_default_recv_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata)
{
    switch (packet->type) {
    case AIOT_MQTTRECV_HEARTBEAT_RESPONSE: {
        printf("heartbeat response\r\n");
        /* TODO: 处理服务器对心跳的回应, 一般不处理 */
    }
    break;

    case AIOT_MQTTRECV_SUB_ACK: {
        printf("suback, res: -0x%04X, packet id: %d, max qos: %d\r\n",
               -packet->data.sub_ack.res, packet->data.sub_ack.packet_id, packet->data.sub_ack.max_qos);
        /* TODO: 处理服务器对订阅请求的回应, 一般不处理 */
    }
    break;

    case AIOT_MQTTRECV_PUB: {
        printf("pub, qos: %d, topic: %.*s\r\n", packet->data.pub.qos, packet->data.pub.topic_len, packet->data.pub.topic);
        printf("pub, payload: %.*s\r\n", packet->data.pub.payload_len, packet->data.pub.payload);
        /* TODO: 处理服务器下发的业务报文 */
    }
    break;

    case AIOT_MQTTRECV_PUB_ACK: {
        printf("puback, packet id: %d\r\n", packet->data.pub_ack.packet_id);
        /* TODO: 处理服务器对QoS1上报消息的回应, 一般不处理 */
    }
    break;

    default: {

    }
    }
}

/* 执行aiot_mqtt_process的线程, 包含心跳发送和QoS1消息以及接收mqtt信息重发 */
static void *__demo_mqtt_process_thread(void *args)
{
    int32_t res = STATE_SUCCESS;
    while (1) {
        aiot_mqtt_process(args);
        res = aiot_mqtt_recv(args);
        if (res < STATE_SUCCESS) {
            osDelay(200);
        }
        osDelay(1000);
    }
    return NULL;
}

 /**
 *  @brief OTA升级检测失败回调函数
 * 
 *  @param [in] ver : 模组返回升级结果错误码
 * 
 *  @return void
 *
 *  @details 由于OC SDK与ASR芯片升级逻辑的物理隔离,需要回调通知执行结果:
 *           理论上这个接口不会上报升级检测成功信息,仅当检测失败时执行,检测成功后模组将自动重启升级
 * 
 */
static void __cm_aliyun_ota_response(int res)
{
    if(CM_ALIYUN_OTA_OK != res)
    {
        printf("ota checked failed, please try after confirm\r\n");
        /** 
         * TO DO
         */
    }
}

 /**
 *  @brief 阿里云OTA升级Demo
 * 
 *  @param void
 * 
 *  @return
 *   =  0 - 成功 \n
 *   <  0 - 失败
 *
 *  @details 运行该用例需要配置实例的接入点mqtt_host以及设备的product_key,device_name,device_secret,配置与阿里云平台配置保持一致,另外:
 *           1. 执行升级后模组会进行多次重启,且升级异常后无法还原至升级前的版本,请在FOTA升级前先使用单个模组调试,保证升级无异常后再进行大规模升级;
 *           2. 执行升级后url有效期为24小时,且升级异常后无法还原至升级前的版本,模组必须在升级前保证模组的网络,电源,SIM流量剩余等,防止升级失败无法恢复;
 *           3. 升级后需要客户自行版本管理并调用cm_aliyun_ota_version_report接口向阿里云平台上报版本号
 */
void cm_aliyun_ota_test(unsigned char **cmd, int len)
{
    int32_t     res = STATE_SUCCESS;
    void       *mqtt_handle = NULL;
    uint16_t    port = 443;      /* 无论设备是否使用TLS连接阿里云平台, 目的端口都是443 */
    aiot_sysdep_network_cred_t cred; /* 安全凭据结构体, 如果要用TLS, 这个结构体中配置CA证书等参数 */
    void       *ota_handle = NULL;
    char       *cur_version = NULL;

    /* 配置SDK的底层依赖 */
    aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile);

    /* 配置SDK的日志输出 */
    aiot_state_set_logcb(__demo_state_logcb);

    /* 创建SDK的安全凭据, 用于建立TLS连接 */
    memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
    cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;  /* 使用RSA证书校验MQTT服务端 */
    cred.max_tls_fragment = 16384; /* 最大的分片长度为16K, 其它可选值还有4K, 2K, 1K, 0.5K */
    cred.sni_enabled = 1;                               /* TLS建连时, 支持Server Name Indicator */
    cred.x509_server_cert = ali_ca_cert;                 /* 用来验证MQTT服务端的RSA根证书 */
    cred.x509_server_cert_len = strlen(ali_ca_cert);     /* 用来验证MQTT服务端的RSA根证书长度 */

    /* 创建1个MQTT客户端实例并内部初始化默认参数 */
    mqtt_handle = aiot_mqtt_init();
    if (NULL == mqtt_handle) {
        printf("aiot_mqtt_init failed\r\n");
        return;
    }

    /* 配置MQTT服务器地址 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_HOST, (void *)mqtt_host);
    /* 配置MQTT服务器端口 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PORT, (void *)&port);
    /* 配置设备productKey */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PRODUCT_KEY, (void *)product_key);
    /* 配置设备deviceName */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_NAME, (void *)device_name);
    /* 配置设备deviceSecret */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_SECRET, (void *)device_secret);
    /* 配置网络连接的安全凭据, 上面已经创建好了 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_NETWORK_CRED, (void *)&cred);
    /* 配置MQTT默认消息接收回调函数 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_RECV_HANDLER, (void *)__demo_mqtt_default_recv_handler);
    /* 配置MQTT事件回调函数 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_EVENT_HANDLER, (void *)__demo_mqtt_event_handler);

    /* 与MQTT例程不同的是, 这里需要增加创建OTA会话实例的语句 */
    ota_handle = aiot_ota_init();
    if (NULL == ota_handle) {
        printf("aiot_ota_init failed\r\n");
        aiot_mqtt_deinit(&mqtt_handle);
        return;
    }

    /* 用以下语句, 把OTA会话和MQTT会话关联起来 */
    aiot_ota_setopt(ota_handle, AIOT_OTAOPT_MQTT_HANDLE, mqtt_handle);
    /* 用以下语句, 设置OTA会话的数据接收回调, SDK收到OTA相关推送时, 会进入这个回调函数 */
    aiot_ota_setopt(ota_handle, AIOT_OTAOPT_RECV_HANDLER, __demo_ota_recv_handler);

    /* 与服务器建立MQTT连接 */
    res = aiot_mqtt_connect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        /* 尝试建立连接失败, 销毁MQTT实例, 回收资源 */
        aiot_mqtt_deinit(&mqtt_handle);
        aiot_ota_deinit(&ota_handle);
        printf("aiot_mqtt_connect failed: -0x%04X\r\n\r\n", -res);
        printf("please check variables like mqtt_host, produt_key, device_name, device_secret in demo\r\n");
        return;
    }

    /*   TODO: 非常重要!!!
     *
     *   cur_version 要根据用户实际情况, 改成从设备的配置区获取, 要反映真实的版本号, 而不能像示例这样写为固定值
     *
     *   1. 如果设备从未上报过版本号, 在控制台网页将无法部署升级任务
     *   2. 如果设备升级完成后, 上报的不是新的版本号, 在控制台网页将会显示升级失败
     * 
     *   注意:这里的上报版本号为触发升级时向阿里云平台上报的当前版本,升级后请调用cm_aliyun_ota_version_report接口
     *        像阿里云平台report升级后的版本，而不是再次使用该接口。
     *
     */

    /* 演示MQTT连接建立起来之后, 就可以上报当前设备的版本号了 */
    cur_version = "1.0.0";
    res = aiot_ota_report_version(ota_handle, cur_version);
    if (res < STATE_SUCCESS) {
        printf("aiot_ota_report_version failed: -0x%04X\r\n", -res);
    }

    /* 创建一个单独的线程, 专用于执行aiot_mqtt_process, 它会自动发送心跳保活, 以及重发QoS1的未应答报文 */
    osThreadAttr_t mqtt_process_attr = {0};
    
    mqtt_process_attr.name  = "mqtt_process_task";
    mqtt_process_attr.stack_size = 1024 * 4;
    mqtt_process_attr.priority = osPriorityNormal;
    g_mqtt_process_thread = osThreadNew((osThreadFunc_t)__demo_mqtt_process_thread, mqtt_handle,&mqtt_process_attr);
	
    if (g_mqtt_process_thread == NULL) {
        printf("pthread_create __demo_mqtt_process_thread failed: %d\n", res);
        return;
    }

    /** 
     * 主动向服务器请求OTA升级,
     * 注意:服务器OTA升级方式选择"云端主动推送升级"时才调用,否则不需要调用该部分,
     *      服务器将主动下发OTA升级task
     */
#ifndef CM_ALIYUN_OTA_ACTIVE_PUSH
    {
        osDelay(200);
        aiot_ota_query_firmware(ota_handle);
    }
#endif

    cm_aliyun_ota_msg_t ota_data = {0};
    s_aliyun_ota_queue = osMessageQueueNew(5, sizeof(cm_aliyun_ota_msg_t), NULL);
    /* 主循环进入休眠 */
    while (1) {
        osMessageQueueGet(s_aliyun_ota_queue, &ota_data, NULL, osWaitForever);  //阻塞
        switch(ota_data.event)
        {
            case CM_ALIYUN_OTA_UPGRADE_START:
                /** 退出循环阻塞,客户检测模组状态后可重 */
                goto quit_ota;
                break;
            default:
                break;
        }
        continue;
quit_ota:
        break;
    }

    if(s_aliyun_ota_queue)
    {
        osMessageQueueDelete(s_aliyun_ota_queue);
        s_aliyun_ota_queue = NULL;
    }

    /* 终止MQTT Receive线程,释放资源 */
    if(g_mqtt_process_thread)
    {
        osThreadTerminate(g_mqtt_process_thread);
        g_mqtt_process_thread = NULL;
    }

    /* 断开MQTT连接, 一般不会运行到这里 */
    res = aiot_mqtt_disconnect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        printf("aiot_mqtt_disconnect failed: -0x%04X\r\n", -res);
        goto exit;
    }

exit:
    while (1) {
        /* 销毁MQTT实例, 一般不会运行到这里 */
        res = aiot_mqtt_deinit(&mqtt_handle);

        if (res < STATE_SUCCESS) {
            printf("aiot_mqtt_deinit failed: -0x%04X\r\n", -res);
            return;
        } else {
            break;
        }
    }
    /* 销毁OTA实例, 一般不会运行到这里 */
    aiot_ota_deinit(&ota_handle);

    return;
}

 /**
 *  @brief 阿里云平台上报OTA版本号,用于OTA升级完成后向服务器上报升级后版本
 *
 *  @param [in] ver : 待上报的设备版本号字符串
 * 
 *  @return
 *   =  0 - 成功 \n
 *   <  0 - 失败
 *
 *  @details 用于OTA升级完成后向服务器上报升级后版本,上报完成后即可断开连接并释放资源
 *           客户可以在初始化时版本管理过程中上报版本
 */
int cm_aliyun_ota_version_report(const char *ver)
{
    int32_t     res = STATE_SUCCESS;
    void       *mqtt_handle = NULL;
    uint16_t    port = 443;      /* 无论设备是否使用TLS连接阿里云平台, 目的端口都是443 */
    aiot_sysdep_network_cred_t cred; /* 安全凭据结构体, 如果要用TLS, 这个结构体中配置CA证书等参数 */
    void       *ota_handle = NULL;

    /* 配置SDK的底层依赖 */
    aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile);

    /* 配置SDK的日志输出 */
    aiot_state_set_logcb(__demo_state_logcb);

    /* 创建SDK的安全凭据, 用于建立TLS连接 */
    memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
    cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;  /* 使用RSA证书校验MQTT服务端 */
    cred.max_tls_fragment = 16384; /* 最大的分片长度为16K, 其它可选值还有4K, 2K, 1K, 0.5K */
    cred.sni_enabled = 1;                               /* TLS建连时, 支持Server Name Indicator */
    cred.x509_server_cert = ali_ca_cert;                 /* 用来验证MQTT服务端的RSA根证书 */
    cred.x509_server_cert_len = strlen(ali_ca_cert);     /* 用来验证MQTT服务端的RSA根证书长度 */

    /* 创建1个MQTT客户端实例并内部初始化默认参数 */
    mqtt_handle = aiot_mqtt_init();
    if (NULL == mqtt_handle) {
        printf("aiot_mqtt_init failed\r\n");
        return -1;
    }

    /* 配置MQTT服务器地址 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_HOST, (void *)mqtt_host);
    /* 配置MQTT服务器端口 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PORT, (void *)&port);
    /* 配置设备productKey */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PRODUCT_KEY, (void *)product_key);
    /* 配置设备deviceName */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_NAME, (void *)device_name);
    /* 配置设备deviceSecret */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_SECRET, (void *)device_secret);
    /* 配置网络连接的安全凭据, 上面已经创建好了 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_NETWORK_CRED, (void *)&cred);
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_RECV_HANDLER, (void *)__demo_mqtt_default_recv_handler);
    /* 配置MQTT事件回调函数 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_EVENT_HANDLER, (void *)__demo_mqtt_event_handler);

    /* 与MQTT例程不同的是, 这里需要增加创建OTA会话实例的语句 */
    ota_handle = aiot_ota_init();
    if (NULL == ota_handle) {
        printf("aiot_ota_init failed\r\n");
        aiot_mqtt_deinit(&mqtt_handle);
        return -2;
    }

    /* 用以下语句, 把OTA会话和MQTT会话关联起来 */
    aiot_ota_setopt(ota_handle, AIOT_OTAOPT_MQTT_HANDLE, mqtt_handle);
    /* 用以下语句, 设置OTA会话的数据接收回调, SDK收到OTA相关推送时, 会进入这个回调函数 */
    aiot_ota_setopt(ota_handle, AIOT_OTAOPT_RECV_HANDLER, __demo_ota_recv_handler);

    /* 与服务器建立MQTT连接 */
    res = aiot_mqtt_connect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        /* 尝试建立连接失败, 销毁MQTT实例, 回收资源 */
        aiot_mqtt_deinit(&mqtt_handle);
        aiot_ota_deinit(&ota_handle);
        printf("aiot_mqtt_connect failed: -0x%04X\r\n\r\n", -res);
        printf("please check variables like mqtt_host, produt_key, device_name, device_secret in demo\r\n");
        return -3;
    }

    /* 上报设备版本号,上报完成后断开连接并释放资源 */
    res = aiot_ota_report_version(ota_handle, (char *)ver);
    if (res < STATE_SUCCESS) {
        printf("aiot_ota_report_version failed: -0x%04X\r\n", -res);
    }

    /* 断开MQTT连接, 一般不会运行到这里 */
    res = aiot_mqtt_disconnect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        printf("aiot_mqtt_disconnect failed: -0x%04X\r\n", -res);
        goto exit;
    }

exit:
    while (1) {
        /* 销毁MQTT实例, 一般不会运行到这里 */
        res = aiot_mqtt_deinit(&mqtt_handle);

        if (res < STATE_SUCCESS) {
            printf("aiot_mqtt_deinit failed: -0x%04X\r\n", -res);
            return -1;
        } else {
            break;
    }
    }
    /* 销毁OTA实例, 一般不会运行到这里 */
    aiot_ota_deinit(&ota_handle);

    return 0;
}

