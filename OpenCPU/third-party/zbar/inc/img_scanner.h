#ifndef _IMG_SCANNER_H_
#define _IMG_SCANNER_H_

#include <zbar.h>

/* internal image scanner APIs for 2D readers */

extern zbar_symbol_t *_zbar_image_scanner_alloc_sym(zbar_image_scanner_t*,
                                                    zbar_symbol_type_t,
                                                    int);
extern void _zbar_image_scanner_add_sym(zbar_image_scanner_t*,
                                        zbar_symbol_t*);
extern void _zbar_image_scanner_recycle_syms(zbar_image_scanner_t*,
                                             zbar_symbol_t*);

#endif
