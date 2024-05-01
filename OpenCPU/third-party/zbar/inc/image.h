#ifndef _IMAGE_H_
#define _IMAGE_H_
//lianghai  debug
//#include "unistd.h"
#include <zbar_config.h>
#include <zbar_utils.h>
#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif
#include <stdlib.h>
#include <assert.h>

#include <zbar.h>
#include "error.h"
#include "symbol.h"
#include "refcnt.h"

#define fourcc zbar_fourcc

/* unpack size/location of component */
#define RGB_SIZE(c)   ((c) >> 5)
#define RGB_OFFSET(c) ((c) & 0x1f)

/* coarse image format categorization.
 * to limit conversion variations
 */
typedef enum zbar_format_group_e {
    ZBAR_FMT_GRAY,
    ZBAR_FMT_YUV_PLANAR,
    ZBAR_FMT_YUV_PACKED,
    ZBAR_FMT_RGB_PACKED,
    ZBAR_FMT_YUV_NV,
    ZBAR_FMT_JPEG,

    /* enum size */
    ZBAR_FMT_NUM
} zbar_format_group_t;


struct zbar_image_s {
    uint32_t format;            /* fourcc image format code */
    unsigned width, height;     /* image size */
    const void *data;           /* image sample data */
    unsigned long datalen;      /* allocated/mapped size of data */
    unsigned crop_x, crop_y;    /* crop rectangle */
    unsigned crop_w, crop_h;
    void *userdata;             /* user specified data associated w/image */

    /* cleanup handler */
    zbar_image_cleanup_handler_t *cleanup;
    refcnt_t refcnt;            /* reference count */
    zbar_video_t *src;          /* originator */
    int srcidx;                 /* index used by originator */
    zbar_image_t *next;         /* internal image lists */

    unsigned seq;               /* page/frame sequence number */
    zbar_symbol_set_t *syms;    /* decoded result set */
};

/* description of an image format */
typedef struct zbar_format_def_s {
    uint32_t format;                    /* fourcc */
    zbar_format_group_t group;          /* coarse categorization */
    union {
        uint8_t gen[4];                 /* raw bytes */
        struct {
            uint8_t bpp;                /* bits per pixel */
            uint8_t red, green, blue;   /* size/location a la RGB_BITS() */
        } rgb;
        struct {
            uint8_t xsub2, ysub2;       /* chroma subsampling in each axis */
            uint8_t packorder;          /* channel ordering flags
                                         *   bit0: 0=UV, 1=VU
                                         *   bit1: 0=Y/chroma, 1=chroma/Y
                                         */
        } yuv;
        uint32_t cmp;                   /* quick compare equivalent formats */
    } p;
} zbar_format_def_t;


extern int _zbar_best_format(uint32_t, uint32_t*, const uint32_t*);
extern const zbar_format_def_t *_zbar_format_lookup(uint32_t);
extern void _zbar_image_free(zbar_image_t*);

#ifdef DEBUG_SVG
extern int zbar_image_write_png(const zbar_image_t*, const char*);
#else
# define zbar_image_write_png(...)
#endif

static __inline void _zbar_image_refcnt (zbar_image_t *img,
                                       int delta)
{
    if(!_zbar_refcnt(&img->refcnt, delta) && delta <= 0) {
        if(img->cleanup)
            img->cleanup(img);
        if(!img->src)
            _zbar_image_free(img);
    }
}

static __inline void _zbar_image_swap_symbols (zbar_image_t *a,
                                             zbar_image_t *b)
{
    zbar_symbol_set_t *tmp = a->syms;
    a->syms = b->syms;
    b->syms = tmp;
}

static __inline void _zbar_image_copy_size (zbar_image_t *dst,
                                          const zbar_image_t *src)
{
    dst->width = src->width;
    dst->height = src->height;
    dst->crop_x = src->crop_x;
    dst->crop_y = src->crop_y;
    dst->crop_w = src->crop_w;
    dst->crop_h = src->crop_h;
}

#endif
