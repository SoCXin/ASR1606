
#include "refcnt.h"

#if !defined(_WIN32) && !defined(TARGET_OS_MAC) && defined(HAVE_LIBPTHREAD)

pthread_once_t initialized = PTHREAD_ONCE_INIT;
pthread_mutex_t _zbar_reflock;

static void initialize (void)
{
    pthread_mutex_init(&_zbar_reflock, NULL);
}

void _zbar_refcnt_init ()
{
    pthread_once(&initialized, initialize);
}


#else

void _zbar_refcnt_init ()
{
}

#endif
