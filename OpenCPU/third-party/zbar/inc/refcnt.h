#ifndef _REFCNT_H_
#define _REFCNT_H_

#include <zbar_config.h>
#include <assert.h>

#if defined(_WIN32)
# include <windows.h>

typedef LONG refcnt_t;

static __inline int _zbar_refcnt (refcnt_t *cnt,
                                int delta)
{
    int rc = -1;
    if(delta > 0)
        while(delta--)
            rc = InterlockedIncrement(cnt);
    else if(delta < 0)
        while(delta++)
            rc = InterlockedDecrement(cnt);
    assert(rc >= 0);
    return(rc);
}

#elif defined(TARGET_OS_MAC)
# include <libkern/OSAtomic.h>

typedef int32_t refcnt_t;

static __inline int _zbar_refcnt (refcnt_t *cnt,
                                int delta)
{
    int rc = OSAtomicAdd32Barrier(delta, cnt);
    assert(rc >= 0);
    return(rc);
}

#elif defined(HAVE_LIBPTHREAD)
# include <pthread.h>

typedef int refcnt_t;

extern pthread_mutex_t _zbar_reflock;

static __inline int _zbar_refcnt (refcnt_t *cnt,
                                int delta)
{
    pthread_mutex_lock(&_zbar_reflock);
    int rc = (*cnt += delta);
    pthread_mutex_unlock(&_zbar_reflock);
    assert(rc >= 0);
    return(rc);
}


#else

typedef int refcnt_t;

static __inline int _zbar_refcnt (refcnt_t *cnt,
                                int delta)
{
    int rc = (*cnt += delta);
    assert(rc >= 0);
    return(rc);
}

#endif


void _zbar_refcnt_init(void);

#endif
