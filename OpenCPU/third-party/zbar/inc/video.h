#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <zbar_config.h>

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <zbar.h>

#include "image.h"
#include "error.h"
#include "mutex.h"

/* number of images to preallocate */
#define ZBAR_VIDEO_IMAGES_MAX  4

typedef enum video_interface_e {
    VIDEO_INVALID = 0,          /* uninitialized */
    VIDEO_V4L1,                 /* v4l protocol version 1 */
    VIDEO_V4L2,                 /* v4l protocol version 2 */
    VIDEO_VFW,                  /* video for windows */
} video_interface_t;

typedef enum video_iomode_e {
    VIDEO_READWRITE = 1,        /* standard system calls */
    VIDEO_MMAP,                 /* mmap interface */
    VIDEO_USERPTR,              /* userspace buffers */
} video_iomode_t;

typedef struct video_state_s video_state_t;

struct zbar_video_s {
    errinfo_t err;              /* error reporting */
    int fd;                     /* open camera device */
    unsigned width, height;     /* video frame size */

    video_interface_t intf;     /* input interface type */
    video_iomode_t iomode;      /* video data transfer mode */
    unsigned initialized : 1;   /* format selected and images mapped */
    unsigned active      : 1;   /* current streaming state */

    uint32_t format;            /* selected fourcc */
    unsigned palette;           /* v4l1 format index corresponding to format */
    uint32_t *formats;          /* 0 terminated list of supported formats */

    unsigned long datalen;      /* size of image data for selected format */
    unsigned long buflen;       /* total size of image data buffer */
    void *buf;                  /* image data buffer */

    unsigned frame;             /* frame count */

    zbar_mutex_t qlock;         /* lock image queue */
    int num_images;             /* number of allocated images */
    zbar_image_t **images;      /* indexed list of images */
    zbar_image_t *nq_image;     /* last image enqueued */
    zbar_image_t *dq_image;     /* first image to dequeue (when ordered) */
    zbar_image_t *shadow_image; /* special case internal double buffering */

    video_state_t *state;       /* platform/interface specific state */

#ifdef HAVE_LIBJPEG
    struct jpeg_decompress_struct *jpeg; /* JPEG decompressor */
    zbar_image_t *jpeg_img;    /* temporary image */
#endif

    /* interface dependent methods */
    int (*init)(zbar_video_t*, uint32_t);
    int (*cleanup)(zbar_video_t*);
    int (*start)(zbar_video_t*);
    int (*stop)(zbar_video_t*);
    int (*nq)(zbar_video_t*, zbar_image_t*);
    zbar_image_t* (*dq)(zbar_video_t*);
};


/* video.next_image and video.recycle_image have to be thread safe
 * wrt/other apis
 */
static __inline int video_lock (zbar_video_t *vdo)
{
    int rc = 0;
    if((rc = _zbar_mutex_lock(&vdo->qlock))) {
        err_capture(vdo, SEV_FATAL, ZBAR_ERR_LOCKING, __func__,
                    "unable to acquire lock");
        vdo->err.errnum = rc;
        return(-1);
    }
    return(0);
}

static __inline int video_unlock (zbar_video_t *vdo)
{
    int rc = 0;
    if((rc = _zbar_mutex_unlock(&vdo->qlock))) {
        err_capture(vdo, SEV_FATAL, ZBAR_ERR_LOCKING, __func__,
                    "unable to release lock");
        vdo->err.errnum = rc;
        return(-1);
    }
    return(0);
}

static __inline int video_nq_image (zbar_video_t *vdo,
                                  zbar_image_t *img)
{
    /* maintains queued buffers in order */
    img->next = NULL;
    if(vdo->nq_image)
        vdo->nq_image->next = img;
    vdo->nq_image = img;
    if(!vdo->dq_image)
        vdo->dq_image = img;
    return(video_unlock(vdo));
}

static __inline zbar_image_t *video_dq_image (zbar_video_t *vdo)
{
    zbar_image_t *img = vdo->dq_image;
    if(img) {
        vdo->dq_image = img->next;
        img->next = NULL;
    }
    if(video_unlock(vdo))
        /* FIXME reclaim image */
        return(NULL);
    return(img);
}


/* PAL interface */
extern int _zbar_video_open(zbar_video_t*, const char*);

#endif
