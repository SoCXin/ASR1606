/**
 * @file        cm_os.h
 * @brief       操作系统接口
 *
 * @section CMSIS_RTOS_TimeOutValue Timeout Value   
 *  Timeout values are an argument to several \b osXxx functions to allow time for resolving a request. A timeout value of \b 0 means that the RTOS does not wait and the function returns instantly, even when no resource is available. A timeout value of @ref osWaitForever means that the RTOS waits infinitely until a resource becomes available. Or one forces the thread to resume using @ref osThreadResume which is discouraged.
 *  The timeout value specifies the number of timer ticks until the time delay elapses. The value is an upper bound and depends on the actual time elapsed since the last timer tick. 
 *  Examples:
 *    - timeout value \b 0 : the system does not wait, even when no resource is available the RTOS function returns instantly. 
 *    - timeout value \b 1 : the system waits until the next timer tick occurs; depending on the previous timer tick, it may be a very short wait time.
 *    - timeout value \b 2 : actual wait time is between 1 and 2 timer ticks.
 *    - timeout value @ref osWaitForever : system waits infinite until a resource becomes available. 
 * 
 * @defgroup os os
 * @ingroup OS
 * @{
 */
 
#ifndef __CM_OS_H__
#define __CM_OS_H__
 
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <stddef.h>

#ifndef __NO_RETURN
#if   defined(__CC_ARM)
#define __NO_RETURN __declspec(noreturn)
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define __NO_RETURN __attribute__((__noreturn__))
#elif defined(__GNUC__)
#define __NO_RETURN __attribute__((__noreturn__))
#elif defined(__ICCARM__)
#define __NO_RETURN __noreturn
#else
#define __NO_RETURN
#endif
#endif
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/


/****************************************************************************
 * Public Types
 ****************************************************************************/

/** Version information. */
typedef struct {
  uint32_t                       api;   /**< API version (major.minor.rev: mmnnnrrrr dec). */
  uint32_t                    kernel;   /**< Kernel version (major.minor.rev: mmnnnrrrr dec). */
} osVersion_t;
 
/** Kernel state. */
typedef enum {
  osKernelInactive        =  0,         /**< Inactive. */
  osKernelReady           =  1,         /**< Ready. */
  osKernelRunning         =  2,         /**< Running. */
  osKernelLocked          =  3,         /**< Locked. */
  osKernelSuspended       =  4,         /**< Suspended. */
  osKernelError           = -1,         /**< Error. */
  osKernelReserved        = 0x7FFFFFFF  /**< Prevents enum down-size compiler optimization. */
} osKernelState_t;
 
/** Thread state. */
typedef enum {
  osThreadInactive        =  0,         /**< Inactive. */
  osThreadReady           =  1,         /**< Ready. */
  osThreadRunning         =  2,         /**< Running. */
  osThreadBlocked         =  3,         /**< Blocked. */
  osThreadTerminated      =  4,         /**< Terminated. */
  osThreadError           = -1,         /**< Error. */
  osThreadReserved        = 0x7FFFFFFF  /**< Prevents enum down-size compiler optimization. */
} osThreadState_t;
 
/** Priority values. */
 typedef enum {
  osPriorityIdle          = 0,         ///< Reserved for Idle thread.
  osPriorityLow           = 2,         ///< Priority: low
  osPriorityBelowNormal   = 3,         ///< Priority: below normal
  osPriorityNormal        = 4,         ///< Priority: normal
  osPriorityAboveNormal   = 5,         ///< Priority: above normal
  osPriorityHigh          = 6,         ///< Priority: high
  osPrioritySoftRealtime  = 7,         ///< Priority: realtime
  osPriorityHardRealtime  = 8,         ///< Priority: realtime
  osPriorityISR           = 32,        ///< Reserved for ISR deferred thread,Do not use!
  osPriorityError         = -1,         ///< System cannot determine priority or illegal priority.
  osPriorityReserved      = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} osPriority_t;
/** Entry point of a thread. */
typedef void (*osThreadFunc_t) (void *argument);
 
/** Timer callback function. */
typedef void (*osTimerFunc_t) (void *argument);
 
/** Timer type. */
typedef enum {
  osTimerOnce               = 0,          /**< One-shot timer. */
  osTimerPeriodic           = 1           /**< Repeating timer. */
} osTimerType_t;
 
/** Timeout value. */
#define osWaitForever         0xFFFFFFFFU /**< Wait forever timeout value. */
 
/** Flags options (@ref osThreadFlagsWait and @ref osEventFlagsWait). */
#define osFlagsWaitAny        0x00000000U /**< Wait for any flag (default). */
#define osFlagsWaitAll        0x00000001U /**< Wait for all flags. */
#define osFlagsNoClear        0x00000002U /**< Do not clear flags which have been specified to wait for. */
 
/** Flags errors (returned by osThreadFlagsXxxx and osEventFlagsXxxx). */
#define osFlagsError          0x80000000U /**< Error indicator. */
#define osFlagsErrorUnknown   0xFFFFFFFFU /**< osError (-1). */
#define osFlagsErrorTimeout   0xFFFFFFFEU /**< osErrorTimeout (-2). */
#define osFlagsErrorResource  0xFFFFFFFDU /**< osErrorResource (-3). */
#define osFlagsErrorParameter 0xFFFFFFFCU /**< osErrorParameter (-4). */
#define osFlagsErrorISR       0xFFFFFFFAU /**< osErrorISR (-6). */
 
/** Thread attributes (attr_bits in @ref osThreadAttr_t). */
#define osThreadDetached      0x00000000U /**< Thread created in detached mode (default). */
#define osThreadJoinable      0x00000001U /**< Thread created in joinable mode. */
 
/** Mutex attributes (attr_bits in @ref osMutexAttr_t). */
#define osMutexRecursive      0x00000001U /**< Recursive mutex. */
#define osMutexPrioInherit    0x00000002U /**< Priority inherit protocol. */
#define osMutexRobust         0x00000008U /**< Robust mutex. */
 
/** Status code values returned by CMSIS-RTOS functions. */
typedef enum {
  osOK                      =  0,         /**< Operation completed successfully. */
  osError                   = -1,         /**< Unspecified RTOS error: run-time error but no other error message fits. */
  osErrorTimeout            = -2,         /**< Operation not completed within the timeout period. */
  osErrorResource           = -3,         /**< Resource not available. */
  osErrorParameter          = -4,         /**< Parameter error. */
  osErrorNoMemory           = -5,         /**< System is out of memory: it was impossible to allocate or reserve memory for the operation. */
  osErrorISR                = -6,         /**< Not allowed in ISR context: the function cannot be called from interrupt service routines. */
  osStatusReserved          = 0x7FFFFFFF  /**< Prevents enum down-size compiler optimization. */
} osStatus_t;
 
 
/** Thread ID identifies the thread. */
typedef void *osThreadId_t;
 
/** Timer ID identifies the timer. */
typedef void *osTimerId_t;
 
/** Event Flags ID identifies the event flags. */
typedef void *osEventFlagsId_t;
 
/** Mutex ID identifies the mutex. */
typedef void *osMutexId_t;
 
/** Semaphore ID identifies the semaphore. */
typedef void *osSemaphoreId_t;
 
/** Memory Pool ID identifies the memory pool. */
typedef void *osMemoryPoolId_t;
 
/** Message Queue ID identifies the message queue. */
typedef void *osMessageQueueId_t;
 
/** Data type that identifies secure software modules called by a process. */
typedef uint32_t TZ_ModuleId_t;
 
/** Attributes structure for thread. */
typedef struct {
  const char                   *name;   /**< name of the thread. */
  uint32_t                 attr_bits;   /**< attribute bits. */
  void                      *cb_mem;    /**< memory for control block. */
  uint32_t                   cb_size;   /**< size of provided memory for control block. */
  void                   *stack_mem;    /**< memory for stack. */
  uint32_t                stack_size;   /**< size of stack. */
  osPriority_t              priority;   /**< initial thread priority (default: osPriorityNormal). */
  TZ_ModuleId_t            tz_module;   /**< TrustZone module identifier. */
  uint32_t                  reserved;   /**< reserved (must be 0). */
} osThreadAttr_t;
 
/** Attributes structure for timer. */
typedef struct {
  const char                   *name;   /**< name of the timer. */
  uint32_t                 attr_bits;   /**< attribute bits. */
  void                      *cb_mem;    /**< memory for control block. */
  uint32_t                   cb_size;   /**< size of provided memory for control block. */
} osTimerAttr_t;
 
/** Attributes structure for event flags. */
typedef struct {
  const char                   *name;   /**< name of the event flags. */
  uint32_t                 attr_bits;   /**< attribute bits. */
  void                      *cb_mem;    /**< memory for control block. */
  uint32_t                   cb_size;   /**< size of provided memory for control block. */
} osEventFlagsAttr_t;
 
/** Attributes structure for mutex. */
typedef struct {
  const char                   *name;   /**< name of the mutex. */
  uint32_t                 attr_bits;   /**< attribute bits. */
  void                      *cb_mem;    /**< memory for control block. */
  uint32_t                   cb_size;   /**< size of provided memory for control block. */
} osMutexAttr_t;
 
/** Attributes structure for semaphore. */
typedef struct {
  const char                   *name;   /**< name of the semaphore. */
  uint32_t                 attr_bits;   /**< attribute bits. */
  void                      *cb_mem;    /**< memory for control block. */
  uint32_t                   cb_size;   /**< size of provided memory for control block. */
} osSemaphoreAttr_t;
 
/** Attributes structure for memory pool. */
typedef struct {
  const char                   *name;   /**< name of the memory pool. */
  uint32_t                 attr_bits;   /**< attribute bits. */
  void                      *cb_mem;    /**< memory for control block. */
  uint32_t                   cb_size;   /**< size of provided memory for control block. */
  void                      *mp_mem;    /**< memory for data storage. */
  uint32_t                   mp_size;   /**< size of provided memory for data storage. */
} osMemoryPoolAttr_t;
 
/** Attributes structure for message queue. */
typedef struct {
  const char                   *name;   /**< name of the message queue. */
  uint32_t                 attr_bits;   /**< attribute bits. */
  void                      *cb_mem;    /**< memory for control block. */
  uint32_t                   cb_size;   /**< size of provided memory for control block. */
  void                      *mq_mem;    /**< memory for data storage. */
  uint32_t                   mq_size;   /**< size of provided memory for data storage. */
} osMessageQueueAttr_t;

/****************************************************************************
 * Public Data
 ****************************************************************************/


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/ 

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif
 
/**
 * 
 * @brief Lock the RTOS Kernel scheduler.
 *
 * @return previous lock state (1 - locked, 0 - not locked, error code if negative).
 */
int32_t osKernelLock (void);

/**
 * @brief Unlock the RTOS Kernel scheduler.
 *
 * @return previous lock state (1 - locked, 0 - not locked, error code if negative).
 */
int32_t osKernelUnlock (void);

/**
 * @brief Get the RTOS kernel tick frequency.
 *
 * @return frequency of the kernel tick in hertz, i.e. kernel ticks per second.
 */
uint32_t osKernelGetTickFreq (void);

/**
 * @brief Get the RTOS kernel tick count.
 *
 * @return RTOS kernel current tick count.
 */
uint32_t osKernelGetTickCount (void);

/**
 * @brief Create a thread and add it to Active Threads.
 *
 * @param[in]     func          thread function.
 * @param[in]     argument      pointer that is passed to the thread function as start argument.
 * @param[in]     attr          thread attributes; NULL: default values.
 *
 * @return thread ID for reference by other functions or NULL in case of error.
 */
osThreadId_t osThreadNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr);

/**
 * @brief Get name of a thread.
 *
 * @param[in]     thread_id     thread ID obtained by @ref osThreadNew or @ref osThreadGetId.
 *
 * @return name as null-terminated string.
 */ 
const char *osThreadGetName (osThreadId_t thread_id);

/**
 * @brief Return the thread ID of the current running thread.
 *
 * @return thread ID for reference by other functions or NULL in case of error.
 */
osThreadId_t osThreadGetId (void);

/**
 * @brief Get current thread state of a thread.
 *
 * @param[in]     thread_id     thread ID obtained by @ref osThreadNew or @ref osThreadGetId.
 *
 * @return current thread state of the specified thread.
 */
osThreadState_t osThreadGetState (osThreadId_t thread_id);

/**
 * @brief Get stack size of a thread.
 *
 * @param[in]     thread_id     thread ID obtained by @ref osThreadNew or @ref osThreadGetId.
 *
 * @return stack size in bytes.
 */
uint32_t osThreadGetStackSize (osThreadId_t thread_id);

/**
 * @brief Get available stack space of a thread based on stack watermark recording during execution.
 *
 * @param[in]     thread_id     thread ID obtained by @ref osThreadNew or @ref osThreadGetId.
 *
 * @return remaining stack space in bytes.
 */
uint32_t osThreadGetStackSpace (osThreadId_t thread_id);

/**
 * @brief Change priority of a thread.
 *
 * @param[in]     thread_id     thread ID obtained by @ref osThreadNew or @ref osThreadGetId.
 * @param[in]     priority      new priority value for the thread function.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osThreadSetPriority (osThreadId_t thread_id, osPriority_t priority);

/**
 * @brief Get current priority of a thread.
 *
 * @param[in]     thread_id     thread ID obtained by @ref osThreadNew or @ref osThreadGetId.
 *
 * @return current priority value of the specified thread.
 */
osPriority_t osThreadGetPriority (osThreadId_t thread_id);

/**
 * @brief Pass control to next thread that is in state \b READY.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osThreadYield (void);

/**
 * @brief Suspend execution of a thread.
 *
 * @param[in]     thread_id     thread ID obtained by @ref osThreadNew or @ref osThreadGetId.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osThreadSuspend (osThreadId_t thread_id);

/**
 * @brief Resume execution of a thread.
 *
 * @param[in]     thread_id     thread ID obtained by @ref osThreadNew or @ref osThreadGetId.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osThreadResume (osThreadId_t thread_id);


 
/** Terminate execution of current running thread. */
//__NO_RETURN void osThreadExit (void);

/**
 * @brief Terminate execution of a thread.
 *
 * @param[in]     thread_id     thread ID obtained by @ref osThreadNew or @ref osThreadGetId.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osThreadTerminate (osThreadId_t thread_id);

/**
 * @brief Get number of active threads.
 *
 * @return number of active threads.
 */
uint32_t osThreadGetCount (void);



/**
 * @brief Wait for Timeout (Time Delay).
 *
 * @param[in]     ticks         @ref CMSIS_RTOS_TimeOutValue "time ticks" value
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osDelay (uint32_t ticks);



/**
 * @brief Create and Initialize a timer.
 *
 * @param[in]     func          function pointer to callback function.
 * @param[in]     type          @ref osTimerOnce for one-shot or @ref osTimerPeriodic for periodic behavior.
 * @param[in]     argument      argument to the timer callback function.
 * @param[in]     attr          timer attributes; NULL: default values.
 *
 * @return timer ID for reference by other functions or NULL in case of error.
 */
osTimerId_t osTimerNew (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr);

/**
 * @brief Get name of a timer.
 *
 * @param[in]     timer_id      timer ID obtained by @ref osTimerNew.
 *
 * @return name as null-terminated string.
 */
//const char *osTimerGetName (osTimerId_t timer_id);

/**
 * @brief Start or restart a timer.
 *
 * @param[in]     timer_id      timer ID obtained by @ref osTimerNew.
 * @param[in]     ticks         @ref CMSIS_RTOS_TimeOutValue "time ticks" value of the timer.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osTimerStart (osTimerId_t timer_id, uint32_t ticks);

/**
 * @brief Stop a timer.
 *
 * @param[in]     timer_id      timer ID obtained by @ref osTimerNew.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osTimerStop (osTimerId_t timer_id);

/**
 * @brief Check if a timer is running.
 *
 * @param[in]      timer_id      timer ID obtained by @ref osTimerNew.
 *
 * @return
 *   = 0 -  not running \n
 *   = 1 -  running
 */
uint32_t osTimerIsRunning (osTimerId_t timer_id);

/**
 * @brief Delete a timer.
 *
 * @param[in]     timer_id      timer ID obtained by @ref osTimerNew.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osTimerDelete (osTimerId_t timer_id);

/**
 * @brief Create and Initialize an Event Flags object.
 *
 * @param[in]     attr          event flags attributes; NULL: default values.
 *
 * @return event flags ID for reference by other functions or NULL in case of error.
 */
osEventFlagsId_t osEventFlagsNew (const osEventFlagsAttr_t *attr);


/**
 * @brief Set the specified Event Flags.
 *
 * @param[in]     ef_id         event flags ID obtained by @ref osEventFlagsNew.
 * @param[in]     flags         specifies the flags that shall be set.
 *
 * @return thread flags input or error code if highest bit set.
 */
uint32_t osEventFlagsSet (osEventFlagsId_t ef_id, uint32_t flags);

/**
 * @brief Clear the specified Event Flags.
 *
 * @param[in]     ef_id         event flags ID obtained by @ref osEventFlagsNew.
 * @param[in]     flags         specifies the flags that shall be cleared.
 *
 * @return event flags before clearing or error code if highest bit set.
 */
//uint32_t osEventFlagsClear (osEventFlagsId_t ef_id, uint32_t flags);

/**
 * @brief Get the current Event Flags.
 *
 * @param[in]     ef_id         event flags ID obtained by @ref osEventFlagsNew.
 *
 * @return current event flags.
 */
//uint32_t osEventFlagsGet (osEventFlagsId_t ef_id);

/**
 * @brief Wait for one or more Event Flags to become signaled.
 *
 * @param[in]     ef_id         event flags ID obtained by @ref osEventFlagsNew.
 * @param[in]     flags         specifies the flags to wait for.
 * @param[in]     options       specifies flags options (osFlagsXxxx).
 * @param[in]     timeout       @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 *
 * @return event flags before clearing or error code if highest bit set.
 */
uint32_t osEventFlagsWait (osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout);

/**
 * @brief Delete an Event Flags object.
 *
 * @param[in]      ef_id         event flags ID obtained by @ref osEventFlagsNew.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osEventFlagsDelete (osEventFlagsId_t ef_id);

/**
 * @brief Create and Initialize a Mutex object.
 *
 * @param[in]      attr          mutex attributes; NULL: default values.
 *
 * @return mutex ID for reference by other functions or NULL in case of error.
 */
osMutexId_t osMutexNew (const osMutexAttr_t *attr);


/**
 * @brief Acquire a Mutex or timeout if it is locked.
 *
 * @param[in]     mutex_id      mutex ID obtained by @ref osMutexNew.
 * @param[in]     timeout       @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osMutexAcquire (osMutexId_t mutex_id, uint32_t timeout);

/**
 * @brief Release a Mutex that was acquired by @ref osMutexAcquire.
 *
 * @param[in]     mutex_id      mutex ID obtained by @ref osMutexNew.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osMutexRelease (osMutexId_t mutex_id);

/**
 * @brief Get Thread which owns a Mutex object.
 *
 * @param[in]     mutex_id      mutex ID obtained by @ref osMutexNew.
 *
 * @return thread ID of owner thread or NULL when mutex was not acquired.
 */
//osThreadId_t osMutexGetOwner (osMutexId_t mutex_id);

/**
 * @brief Delete a Mutex object.
 *
 * @param[in]     mutex_id      mutex ID obtained by @ref osMutexNew.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osMutexDelete (osMutexId_t mutex_id);

/**
 * @brief Create and Initialize a Semaphore object.
 *
 * @param[in]     max_count     maximum number of available tokens.
 * @param[in]     initial_count initial number of available tokens.
 * @param[in]     attr          semaphore attributes; NULL: default values.
 *
 * @return semaphore ID for reference by other functions or NULL in case of error.(only binary semaphore are supported)
 */
osSemaphoreId_t osSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr);

/**
 * @brief Acquire a Semaphore token or timeout if no tokens are available.
 *
 * @param[in]     semaphore_id  semaphore ID obtained by @ref osSemaphoreNew.
 * @param[in]     timeout       @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout);

/**
 * @brief Release a Semaphore token up to the initial maximum count.
 *
 * @param[in]     semaphore_id  semaphore ID obtained by @ref osSemaphoreNew.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osSemaphoreRelease (osSemaphoreId_t semaphore_id);

/**
 * @brief Get current Semaphore token count.
 *
 * @param[in]     semaphore_id  semaphore ID obtained by @ref osSemaphoreNew.
 *
 * @return number of tokens available.
 */
//uint32_t osSemaphoreGetCount (osSemaphoreId_t semaphore_id);

/**
 * @brief Delete a Semaphore object.
 *
 * @param[in]     semaphore_id  semaphore ID obtained by @ref osSemaphoreNew.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osSemaphoreDelete (osSemaphoreId_t semaphore_id);




/**
 * @brief Create and Initialize a Message Queue object.
 *
 * @param[in]     msg_count     maximum number of messages in queue.
 * @param[in]     msg_size      maximum message size in bytes.
 * @param[in]     attr          message queue attributes; NULL: default values.
 *
 * @return message queue ID for reference by other functions or NULL in case of error.
 */
osMessageQueueId_t osMessageQueueNew (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr);


/**
 * @brief Put a Message into a Queue or timeout if Queue is full.
 *
 * @param[in]     mq_id         message queue ID obtained by @ref osMessageQueueNew.
 * @param[in]     msg_ptr       pointer to buffer with message to put into a queue.
 * @param[in]     msg_prio      message priority.
 * @param[in]     timeout       @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osMessageQueuePut (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout);

/**
 * @brief Get a Message from a Queue or timeout if Queue is empty.
 *
 * @param[in]     mq_id         message queue ID obtained by @ref osMessageQueueNew.
 * @param[out]    msg_ptr       pointer to buffer for message to get from a queue.
 * @param[out]    msg_prio      pointer to buffer for message priority or NULL.
 * @param[in]     timeout       @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);

/**
 * @brief Get maximum number of messages in a Message Queue.
 *
 * @param[in]     mq_id         message queue ID obtained by @ref osMessageQueueNew.
 *
 * @return maximum number of messages.
 */
//uint32_t osMessageQueueGetCapacity (osMessageQueueId_t mq_id);

/**
 * @brief Get maximum message size in a Message Queue.
 *
 * @param[in]     mq_id         message queue ID obtained by @ref osMessageQueueNew.
 *
 * @return maximum message size in bytes.
 */
//uint32_t osMessageQueueGetMsgSize (osMessageQueueId_t mq_id);

/**
 * @brief Get number of queued messages in a Message Queue.
 *
 * @param[in]     mq_id         message queue ID obtained by @ref osMessageQueueNew.
 *
 * @return number of queued messages.
 */
//uint32_t osMessageQueueGetCount (osMessageQueueId_t mq_id);

/**
 * @brief Get number of available slots for messages in a Message Queue.
 *
 * @param[in]     mq_id         message queue ID obtained by @ref osMessageQueueNew.
 *
 * @return number of available slots for messages.
 */
//uint32_t osMessageQueueGetSpace (osMessageQueueId_t mq_id);

/**
 * @brief Reset a Message Queue to initial empty state.
 *
 * @param[in]     mq_id         message queue ID obtained by @ref osMessageQueueNew.
 *
 * @return status code that indicates the execution status of the function.
 */
//osStatus_t osMessageQueueReset (osMessageQueueId_t mq_id);

/**
 * @brief Delete a Message Queue object.
 *
 * @param[in]     mq_id         message queue ID obtained by @ref osMessageQueueNew.
 *
 * @return status code that indicates the execution status of the function.
 */
osStatus_t osMessageQueueDelete (osMessageQueueId_t mq_id);
 
 
#undef EXTERN
#ifdef __cplusplus
}
#endif
 
#endif  /* __CM_OS_H__ */

/** @}*/