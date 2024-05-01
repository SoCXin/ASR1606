#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef QRCODE_ENABLE_DIAG
#include <diag.h>
#endif
#include "zbar_utils.h"
#include "cm_sys.h"
#include "cm_os.h"

/************************log***********************/
extern int __uart_printf(const char* fmt, va_list ap);
extern void CPUartLogPrint(char* log);

QR_LOG_ST gstQrLogSt = {QR_LOG_WARNING,QR_LOG_UART};

void qrSetLog(QR_LOG_E level, QR_LOG_CTL_E method)
{
    gstQrLogSt.level = level;
	gstQrLogSt.method = method;
	return;
}

void qrPrintf(int level, const char *fmt, ...)
{
        
	char buf[512];
	va_list ap;		
	
	if(level>gstQrLogSt.level)
		return;
	
	va_start(ap, fmt);	
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
#if 0
#ifdef QRCODE_ENABLE_DIAG	
	if((gstQrLogSt.method==QR_LOG_DIAG)||(gstQrLogSt.method==QR_LOG_UART_DIAG))
	{
	    //diag log
	    DIAG_FILTER(qrcode, DEBUG, PRINT_LOG, DIAG_INFORMATION)
	    diagPrintf ("%s", buf);
	}
#endif	
	if((gstQrLogSt.method==QR_LOG_UART)||(gstQrLogSt.method==QR_LOG_UART_DIAG))
	{
	    //uart log	
	    CPUartLogPrint(buf);
	}
    if(gstQrLogSt.method==QR_LOG_PRNTF)
		printf("%s",buf);
#endif
    cm_log_printf(0, "[zbar] %s", buf);
	return;
}

/************************timer*************************/
extern uint32_t  OsaGetTicks(void *notUsed);
#define TICK_TO_MS 5
QR_TIME_STATICS_ST gstQrTime = {0};

uint64_t qrGetTimeMs(void)
{    
	uint64_t timeMs;
	timeMs = (uint64_t)(osKernelGetTickCount()*TICK_TO_MS);
	return timeMs;
}
void qrTimePrint(void)
{
    debug("Total time cost:%lld", gstQrTime.timer1Stop-gstQrTime.timer1Start);
	debug("/t/tX Y density search costs:%lld", gstQrTime.timer2Stop-gstQrTime.timer2Start);
	debug("/t/tX X density search costs:%lld", gstQrTime.timer3Stop-gstQrTime.timer3Start);
	debug("/t/tX qr_decode costs:%lld", gstQrTime.timer4Stop-gstQrTime.timer4Start);
}

//TBC.

