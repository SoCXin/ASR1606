/****************************************************************************
 * Included Files
 ****************************************************************************/
 
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "cm_async_dns.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* 本源文件内代码用于版本兼容性，请勿进行修改 */
extern int cm_async_dns_request_ex(const char *host_name, cm_async_dns_addrtype_e addr_type,
                         cm_async_dns_ip_addr_t *ip_addr, cm_async_dns_event_cb event_cb, void *cb_param, int cid);

int cm_async_dns_request(const char *host_name, cm_async_dns_addrtype_e addr_type,
                         cm_async_dns_ip_addr_t *ip_addr, cm_async_dns_event_cb event_cb, void *cb_param)
{
    return cm_async_dns_request_ex(host_name, addr_type, ip_addr, event_cb, cb_param, 0);
}
