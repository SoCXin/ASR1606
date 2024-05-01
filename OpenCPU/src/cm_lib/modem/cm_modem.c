#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cm_sys.h"
#include "cm_virt_at.h"
#include "cm_modem.h"
#include "cm_modem_info.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "cm_os.h"
#define  CB_MSG_SIZE        (4)
#define  CB_MSG_COUNT       (1)
#define  CB_MSG_OUTTIME     (1000)

typedef enum
{
    CM_STATUS_RAT_INVALID = 0,
    CM_STATUS_RAT_GSM,
    CM_STATUS_RAT_WCDMA,
    CM_STATUS_RAT_TDSCDMA,
    CM_STATUS_RAT_LTE,
    CM_STATUS_RAT_eMTC,
    CM_STATUS_RAT_NBIoT,
    CM_STATUS_RAT_CDMA,
    CM_STATUS_RAT_EVDO, 
} cm_extern_status_rat_e;

osMessageQueueId_t modem_mq = NULL;
static void cb(void * param)
{
    if(modem_mq != NULL)
    {
        osMessageQueuePut(modem_mq, param, 0, CB_MSG_OUTTIME/5);  
    }
}

int32_t cm_modem_get_cpin(void)
{
	unsigned char at_data[12] = {0};
	int cm_cpin = -1;
	char  modem_flag[4] = {-1}; 
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
	modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL);
	if(cm_virt_at_send((uint8_t *)"AT+CPIN?\r\n", strlen("AT+CPIN?\r\n")) < 0)
    {
        cm_virt_at_deinit();
        return -1;
    }
	if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
	{
		cm_virt_at_get(at_data, sizeof(at_data));
		if(0 == atoi(modem_flag))
        {
			if (0 == strncmp((char *)at_data, "+CPIN: READY", 11))
            {
				cm_cpin = 0;
            }
		}
	}
	osMessageQueueDelete(modem_mq);
    cm_virt_at_deinit(); 
	return cm_cpin;
}

int32_t cm_modem_get_cgmr(char *cgmr)
{
	unsigned char data[21] = {0};
	char  modem_flag[4] = {-1}; 
    int get_cgmr = -1;
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL); 
    if(cm_virt_at_send((uint8_t *)"AT+CGMR\r\n", strlen("AT+CGMR\r\n")) < 0)
    {
        cm_virt_at_deinit();
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(data,sizeof(data));           
            strcpy(cgmr, (char *)data);
            get_cgmr = 0; 
        }   
    }
    osMessageQueueDelete(modem_mq);  
    cm_virt_at_deinit();
    return get_cgmr;
}

int32_t cm_modem_get_cgmm(char *cgmm)
{ 
    char  modem_flag[4] = {-1}; 
    unsigned char data[7] = {0};
    int get_cgmm = -1;
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL); 
    if(cm_virt_at_send((uint8_t *)"AT+CGMM\r\n", strlen("AT+CGMM\r\n")) < 0)
    {
        cm_virt_at_deinit();
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(data, 7);           
            strcpy(cgmm, (char *)data);
            get_cgmm = 0;  
        }   
    }
    osMessageQueueDelete(modem_mq);  
    cm_virt_at_deinit();
    return get_cgmm;
}

int32_t cm_modem_get_cgmi(char *cgmi)
{  
    unsigned char data[5] = {0};
    char  modem_flag[4] = {-1};
    int get_cgmi = -1;       
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL); 
    if(cm_virt_at_send((uint8_t *)"AT+CGMI\r\n", strlen("AT+CGMI\r\n")) < 0)
    {
        cm_virt_at_deinit();
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(data, 7);           
            strcpy(cgmi, (char *)data);
            get_cgmi = 0; 
        }
    }
    osMessageQueueDelete(modem_mq);     
    cm_virt_at_deinit();    
    return get_cgmi; 
}

int32_t cm_modem_get_cops(cm_cops_info_t *cops)
{
    unsigned char at_data[30] = {0};
    char *result = NULL;        
    char data[30] = {0};
    char  modem_flag[4] = {-1}; 
    int get_cops = -1;     
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL); 
    if(cm_virt_at_send((uint8_t *)"AT+COPS?\r\n", strlen("AT+COPS?\r\n")) < 0)
    {
        cm_virt_at_deinit();
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        cm_virt_at_get(at_data, sizeof(at_data));   
        if(0 == atoi(modem_flag))
        { 
            strcpy(data, (char *)at_data);         
            if (0 == strncmp(data, "+COPS", 5))
            {
                result = strtok(data, ":");
                if (NULL != result)
                    result = strtok(NULL, ":");
                result = strtok(result,  ",");
                cops->mode = (uint8_t)atoi(result);
                result = strtok(NULL, ",");
                cops->format = (uint8_t)atoi(result);
                result = strtok(NULL, ",");
                memcpy(cops->oper, result, sizeof(cops->oper));
                result = strtok(NULL, ",");
                cops->act = (uint8_t)atoi(result);
            }                        
            get_cops = 0;
        }
    }
    osMessageQueueDelete(modem_mq);  
    cm_virt_at_deinit();         
    return get_cops;
}

int32_t cm_modem_get_csq(char *rssi, char *ber)
{
    char data[16] = {0};
    unsigned char at_data[16] = {0};
    char *result = NULL;
    char oc_rssi[8] = {0};
    char oc_ber[8] = {0};
    char  modem_flag[4] = {-1}; 
    int get_csq = -1;    
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL); 
    if(cm_virt_at_send((uint8_t *)"AT+CSQ\r\n", strlen("AT+CSQ\r\n")) < 0)
    {
        cm_virt_at_deinit();
        return -1;
    } 
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(at_data, sizeof(at_data));  
            strcpy(data, (char *)at_data);   
            if (0 == strncmp(data, "+CSQ", 4))
            {
                result = strtok(data, ":");
                if (NULL != result)
                    result = strtok(NULL, ":");  
                result = strtok(result, ",");
                memcpy(oc_rssi, result, strlen(result));
                result = strtok(NULL, ",");
                memcpy(oc_ber, result, strlen(result));
            }
            strcpy(rssi, oc_rssi);
            strcpy(ber, oc_ber); 
            get_csq = 0;
        } 
    }
    osMessageQueueDelete(modem_mq);        
    cm_virt_at_deinit(); 
    return get_csq;
}

int32_t cm_modem_get_radio_info(cm_radio_info_t *radio_info)
{
    int ret = -1; 
    cm_cops_info_t *cops = NULL;
    cops = malloc(sizeof(cm_cops_info_t));
    ret = cm_modem_get_cops(cops);
    if(cops != NULL)
    {
        radio_info->rat = cops->mode;
    }
    free(cops);
    cm_modem_info_radio(radio_info);
    return ret;
}

int32_t cm_modem_get_cell_info(cm_cell_info_t cell_info[], uint16_t cell_info_num)
{
    int ret = -1;
    ret = cm_modem_info_cell(cell_info,cell_info_num);
    return ret;
}


int32_t cm_modem_set_cfun(uint16_t fun)
{
    int ret = -1;
    unsigned char at_data[10] = {0};    
    char cfun_cmd[12] = {0};
    char  modem_flag[4] = {-1};
    snprintf(cfun_cmd, sizeof(cfun_cmd), "AT+CFUN=%hd\r\n",fun);      
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL);
    if(cm_virt_at_send((uint8_t *)cfun_cmd, strlen(cfun_cmd)) < 0)
    {
        cm_virt_at_deinit();
        return -1;
    }    
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(at_data, sizeof(at_data));       
            if (0 == strncmp((char *)at_data, "OK", 2))
            {
                ret = 0;
            }
        }        
    }
    osMessageQueueDelete(modem_mq); 
    cm_virt_at_deinit();    
    return ret;
}

int32_t cm_modem_get_cfun(void)
{
    char data[10] = {0};
    unsigned char at_data[10] = {0};
    char *result = NULL;
    int cm_cfun = -1;
    char  modem_flag[4] = {-1};
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL); 
    if(cm_virt_at_send((uint8_t *)"AT+CFUN?\r\n", strlen("AT+CFUN?\r\n")) < 0)
    {
        cm_virt_at_deinit();
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(at_data, sizeof(at_data));
            strcpy(data, (char *)at_data);          
            if (0 == strncmp(data, "+CFUN", 5))
            {
                result = strtok(data, ":");
                if (result != NULL)
                    result = strtok(NULL, ":");
                result = strtok(result,  ",");
            }
            cm_cfun = (int)atoi(result); 
        }
    }
    osMessageQueueDelete(modem_mq);    
    cm_virt_at_deinit();   
    return cm_cfun;
}

int32_t cm_modem_set_edrx_cfg(const cm_edrx_cfg_set_t *cfg)
{
    char cfg_cmd[30] = {0};
    int   cfg_r = -1;
    unsigned char data[5] = {0};
    char edrx_value_tmp[5] = {0};
    char  modem_flag[4] = {-1};
    itoa((cfg->requested_edrx_value), edrx_value_tmp, 2);
    snprintf(cfg_cmd, sizeof(cfg_cmd), "AT+CEDRXS=%d, %d, %s\r\n", cfg->mode, cfg->act_type, edrx_value_tmp); 
    if(0 != cm_virt_at_init(cb))
    { 
        cm_virt_at_deinit();
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL);
    if(cm_virt_at_send((uint8_t *)cfg_cmd, sizeof(cfg_cmd)) < 0)
    {
        cm_virt_at_deinit();
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(data, sizeof(data));
            if(0 == strncmp((char *)data, "OK", 2))
            {
                cfg_r = 0;
            }            
        }
    }
    osMessageQueueDelete(modem_mq); 
    cm_virt_at_deinit();
    return cfg_r;
}

int32_t cm_modem_get_edrx_cfg(cm_edrx_cfg_get_t *cfg)
{
    char data[90] = {0};
    unsigned char at_data[90] = {0};
    char *result = NULL;
    char act_type[10] = {0};
    char edrx_value[10] = {0};
    char  modem_flag[4] = {-1}; 
    int get_edrx_cfg = -1;     
    if(0 != cm_virt_at_init(cb))
    { 
        cm_virt_at_deinit();
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL);
    if(cm_virt_at_send((uint8_t *)"AT+CEDRXS?\r\n", strlen("AT+CEDRXS?\r\n")) < 0)
    { 
        cm_virt_at_deinit();
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(at_data, sizeof(at_data));           
            strcpy(data, (char *)at_data); 
            if (0 == strncmp(data, "+CEDRXS", 7))
            {
                result = strtok(data, ":");
                if (NULL != result)
                    result = strtok(NULL, ":");
                result = strtok(result, ",");
                if(NULL != result)
                {
                    if(strlen(result) + 1 <= sizeof(act_type))
                        memcpy(act_type, result, strlen(result) + 1);
                    else
                    {
                        cm_virt_at_deinit();
                        return -1;
                    } 					    
                }
                result = strtok(NULL, "\"");
                if(NULL != result)
                {
                    if(strlen(result) + 1 <= sizeof(edrx_value))
                        memcpy(edrx_value, result, strlen(result) + 1);
                    else
                    {
                        cm_virt_at_deinit();
                        return -1;
                    } 					    
                }
        
            }     
            cfg->act_type = (unsigned char) atoi(act_type);
            cfg->requested_edrx_value = (unsigned char)atoi(edrx_value);
            get_edrx_cfg = 0;
        }
    }
    osMessageQueueDelete(modem_mq);        
    cm_virt_at_deinit();
    return get_edrx_cfg;
}

int32_t cm_modem_set_psm_cfg(const cm_psm_cfg_t* cfg)
{
    char cfg_cmd[50] = {0};
    char tau_tmp[9] = {0};
    char  modem_flag[4] = {-1};  
    char active_time_tmp[9] = {0};
    unsigned char at_data[10] = {0};
    int set_cfg = -1;
    itoa((cfg->requested_periodic_tau), tau_tmp, 2);
    itoa((cfg->requested_active_time), active_time_tmp, 2);
    snprintf(cfg_cmd, sizeof(cfg_cmd), "AT+CPSMS=%d,,, %s, %s\r\n", cfg->mode, tau_tmp, active_time_tmp); 
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL);
    if(cm_virt_at_send((uint8_t *)cfg_cmd, sizeof(cfg_cmd)) < 0)
    { 
        cm_virt_at_deinit();
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(at_data, sizeof(at_data));
            if (0 == strncmp((char *)at_data, "OK", 2))
            {
                set_cfg = 0;
            }
        }
    }
    osMessageQueueDelete(modem_mq);  
    cm_virt_at_deinit();       
    return set_cfg;
}

int32_t cm_modem_get_psm_cfg(cm_psm_cfg_t *cfg)
{
    char data[50] = {0};
    unsigned char at_data[50] = {0};
    char *result = NULL;
    char mode[4] = {0};
    char periodic_tau[10] = {0};
    char active_time[10] = {0};
    unsigned char tau_tmp = 0;
    unsigned char active_time_tmp = 0;;
    char  modem_flag[4] = {-1}; 
    int get_cfg = -1;    
    if(NULL == cfg)
    {
        return -1;
    }
    if(0 != cm_virt_at_init(cb))
    { 
        cm_virt_at_deinit(); 
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL);
    if(cm_virt_at_send((uint8_t *)"AT+CPSMS?\r\n", strlen("AT+CPSMS?\r\n")) < 0)
    {
        cm_virt_at_deinit();  
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(at_data, sizeof(at_data));
            strcpy(data, (char *)at_data); 
            if (0 == strncmp(data, "+CPSMS", 5))
            {
                result = strtok(data, ":");
                if (NULL != result)
                    result = strtok(NULL, ":");
                result = strtok(result, ",");
                memcpy(mode, result, strlen(result));
                result = strtok(NULL, ",");
                result = strtok(NULL, ",");
                result = strtok(NULL, ",");
                memcpy(periodic_tau, result, strlen(result) + 1);
                result = strtok(NULL, ",");
                memcpy(active_time, result, strlen(result) + 1);
            }
            for(int i = 8;i > 0;i--)
            {
                if('1' == periodic_tau[i])
                {
                    tau_tmp = tau_tmp + pow(2, 8 - i);
                }
                if('1' == active_time[i])
                {
                    active_time_tmp = active_time_tmp + pow(2,8 - i);
                }
            }
            cfg->mode = (unsigned char)atoi(mode);
            cfg->requested_active_time = active_time_tmp;
            cfg->requested_periodic_tau = tau_tmp; 
            get_cfg = 0;
            }
        }
    osMessageQueueDelete(modem_mq);  
    cm_virt_at_deinit();          
    return get_cfg;
}

int32_t cm_modem_get_cereg_state(cm_cereg_state_t *cereg)
{
    char data[60] = {0};
    unsigned char at_data[60] = {0};
    char *result = NULL;
    char n[5]  = {0};            
    char state[5]  = {0};        
    char lac[7]  = {0};         
    char ci[11]  = {0}; 
    unsigned int ci_num = 0;
    unsigned short lac_num = 0;        
    char act[5]  = {0}; 
    char  modem_flag[4] = {-1};
    int get_cereg_state = -1;           
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit();  
        return -1;
    }     
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL);           
    if(cm_virt_at_send((uint8_t *)"AT+CEREG=5;+CEREG?\r\n", strlen("AT+CEREG=5;+CEREG?\r\n")) < 0)
    {
        cm_virt_at_deinit(); 
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {             
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(at_data, sizeof(at_data));
            strcpy(data, (char *)at_data);         
            if (0 == strncmp(data, "+CEREG", 6))
            {
                result = strtok(data, ":");
                if (NULL != result)
                    result = strtok(NULL, ":");
                result = strtok(result, ",");
                memcpy(n, result, strlen(result));
                result = strtok(NULL, ",");
                memcpy(state, result, strlen(result));
                result = strtok(NULL, ",");
                memcpy(lac, result, strlen(result));
                result = strtok(NULL, ",");
                memcpy(ci, result, strlen(result));
                result = strtok(NULL, "O");
                memcpy(act, result, strlen(result));           
            }  
            for(int i = 8;i > 0;i--)
            {
                if(ci[i] >= 'A')
                {
                    ci_num = ci_num + pow(16, (8 - i))*(ci[i] - 55);  
                }
                else
                {
                    ci_num = ci_num + pow(16, (8 - i))*(ci[i] - 48);    
                }
            }
            for (int i = 4; i > 0; i--)
            {
                lac_num = lac_num + pow(10, (4 - i))*(lac[i] - 48);  
            }    
            cereg->n = (unsigned char)atoi(n);
            cereg->state = (unsigned char)atoi(state);
            cereg->lac = lac_num;
            cereg->ci = ci_num;
            cereg->act =  (unsigned char)atoi(act);
            get_cereg_state = 0;
        }
    }
    osMessageQueueDelete(modem_mq);         
    cm_virt_at_deinit(); 
    return get_cereg_state;
}

int32_t cm_modem_get_cscon(void)
{
    char data[20] = {0};
    unsigned char at_data[20] = {0};
    char *result = NULL;
    char  modem_flag[4] = {-1};
    int cm_cscon = -1; 
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL); 
    if(cm_virt_at_send((uint8_t *)"AT+CSCON?\r\n", strlen("AT+CSCON?\r\n")) < 0)
    { 
        cm_virt_at_deinit(); 
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        cm_virt_at_get(at_data, sizeof(at_data));        
        if(0 == atoi(modem_flag))
        {
            strcpy(data, (char *)at_data); 
            if (0 == strncmp(data, "+CSCON", 6))
            {
                result = strtok(data, ":");
                if (NULL != result)
                    result = strtok(NULL, ":");
                result = strtok(result, ",");
                result = strtok(NULL, ",");
                result = strtok(NULL, ",");
            }
            cm_cscon = (int)atoi(result);   
        }
    }
    osMessageQueueDelete(modem_mq);    
    cm_virt_at_deinit();
    return cm_cscon;
}

int32_t cm_modem_activate_pdp(uint16_t cid)
{
    char at_cmd[20] ={0};
    int pdp_r = -1;
    char  modem_flag[4] = {-1};
    char cid_cmd[5] = {0}; 
    unsigned char data[5] = {0};
    itoa((short int)cid, cid_cmd, 10);
    snprintf(at_cmd, sizeof(at_cmd), "AT+CGACT=1, %s\r\n", cid_cmd);           
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit(); 
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL); 
    if(cm_virt_at_send((uint8_t *)at_cmd, strlen(at_cmd)) < 0)
    {
        cm_virt_at_deinit();
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(data, sizeof(data));
            if(0 == strncmp((char *)data, "OK", 2))
            {
                pdp_r = 0;
            }                 
        }
    }
    osMessageQueueDelete(modem_mq);     
    cm_virt_at_deinit();
    return pdp_r;
}

int32_t cm_modem_deactivate_pdp(uint16_t cid)
{
    char at_cmd[20] ={0};
    char cid_cmd[5] = {0};
    char  modem_flag[4] = {-1};
    int pdp_r = -1;
    unsigned char data[5] = {0};
    itoa((short int)cid, cid_cmd, 10);
    snprintf(at_cmd, sizeof(at_cmd), "AT+CGACT=0, %s\r\n", cid_cmd);         
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit(); 
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL);
    if(cm_virt_at_send((uint8_t *)at_cmd, strlen(at_cmd)) < 0)
    { 
        cm_virt_at_deinit(); 
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {
            cm_virt_at_get(data, sizeof(data));
            if(0 == strncmp((char *)data, "OK", 2))
            {
                pdp_r = 0;
            }                 
        }
    } 
    osMessageQueueDelete(modem_mq);    
    cm_virt_at_deinit();
    return pdp_r;
}

int32_t cm_modem_get_pdp_state(uint16_t cid)
{
    char data[180] = {0};
    unsigned char at_data[180] = {0};
    char  modem_flag[4] = {-1};     
    char *result = NULL;
    int  pdp_state0 = -1;  
    int pdp_state1 = -1;
    int pdp_state = -1;
    if(cid < 1 || cid > 15 )
    {
        return -1;
    }    
    if(0 != cm_virt_at_init(cb))
    {
        cm_virt_at_deinit(); 
        return -1;
    }
    modem_mq = osMessageQueueNew(CB_MSG_COUNT, CB_MSG_SIZE, NULL);      
    if(cm_virt_at_send((uint8_t *)"AT+CGACT?\r\n", strlen("AT+CGACT?\r\n")) < 0)
    {
        cm_virt_at_deinit();
        return -1;
    }
    if(0 == osMessageQueueGet(modem_mq, modem_flag, 0, CB_MSG_OUTTIME))
    {
        if(0 == atoi(modem_flag))
        {     
            cm_virt_at_get(at_data, sizeof(at_data));
            strcpy(data, (char *)at_data); 
            if (0 == strncmp(data, "+CGACT", 6)) 
            {
                result = strtok(data, ".");                  
                while(1)
                {
                    result = strtok(result, ":");             
                    if (NULL != result)
                        result = strtok(NULL, ".");                 
                    result = strtok(result, ",");
                    pdp_state0 = (unsigned short)atoi(result);
                    result = strtok(NULL, "+");
                    pdp_state1 = atoi(result);
                    result = strtok(NULL, ".");
                    if(cid == pdp_state0)
                    {
                        pdp_state = pdp_state1;
                        break;
                    }                      
                    if(NULL == result)
                    {
                        break;
                    }            
                }
            }           
        }        
    } 
    osMessageQueueDelete(modem_mq);    
    cm_virt_at_deinit();     
    return pdp_state;
}