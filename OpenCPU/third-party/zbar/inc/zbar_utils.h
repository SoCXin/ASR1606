#ifndef _ZBAR_UTILS_H_
#define _ZBAR_UTILS_H_
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
//typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#else
# ifndef HAVE__BOOL
#  ifdef __cplusplus
typedef bool _Bool;
#  else
#   define _Bool signed char
#  endif
# endif
# define bool _Bool
# define false 0
# define true 1
# define __bool_true_false_are_defined 1
#endif

/************************log***********************/
typedef enum
{
    QR_LOG_OFF = 0,
	QR_LOG_WARNING,
	QR_LOG_DBG,
	QR_LOG_INFO,
}QR_LOG_E;
typedef enum
{
    QR_LOG_UART = 0,
	QR_LOG_DIAG,
	QR_LOG_UART_DIAG,
	QR_LOG_PRNTF,
}QR_LOG_CTL_E;
typedef struct
{
    int level;
	int method;
}QR_LOG_ST;

#define QR_LOG_MODULE_NAME "[qrcode]--"
extern void qrPrintf(int level, const char *fmt, ...);
extern void qrSetLog(QR_LOG_E level, QR_LOG_CTL_E method);

#define info(format, args...) do {                          \
            qrPrintf(QR_LOG_INFO,"%s%s: " format, QR_LOG_MODULE_NAME,__func__, ##args);          \
        } while(0)
#define debug(format, args...) do {                          \
            qrPrintf(QR_LOG_DBG,"%s%s: " format, QR_LOG_MODULE_NAME,__func__, ##args);          \
        } while(0)
#define warning(format, args...) do {                          \
            qrPrintf(QR_LOG_WARNING,"%s%s: " format, QR_LOG_MODULE_NAME,__func__, ##args);          \
        } while(0)

//#define QRCODE_DEBUG
#ifdef QRCODE_DEBUG
#define QR_DBG_PRINT(format, args...) info(format, args...)
#else
#define QR_DBG_PRINT(format, args...) while(0);
#endif

/************************timer*************************/
typedef struct
{   
    uint64_t timer1Start;
    uint64_t timer1Stop;
    uint64_t timer2Start;
    uint64_t timer2Stop;
    uint64_t timer3Start;
    uint64_t timer3Stop;
    uint64_t timer4Start;
    uint64_t timer4Stop;
}QR_TIME_STATICS_ST;
extern QR_TIME_STATICS_ST gstQrTime;
extern uint64_t qrGetTimeMs();

#endif
