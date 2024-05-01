#include <stdio.h>
#include <stdlib.h>
#include <zbar.h>
#include <diag.h>
#include <osa.h>
#include "zbar_utils.h"
#include "crossPlatformSW.h"

//#define IMAGE_DATA_FROM_FILE
#ifndef IMAGE_DATA_FROM_FILE
#include "img_mono.h"
#else
#define INPUT_IMG "D:\\img_mono.bin"
#endif



zbar_image_scanner_t *scanner = NULL;

/* to complete a runnable example, this abbreviated implementation of
 * get_data() will use libpng to read an image file. refer to libpng
 * documentation for details
 */

extern bool sdcard_is_ready(void);

static int get_raw_data(const char* fileName, int *width,int *height,void **raw)
{
	int imgLen,n;
	unsigned int handle;

	debug("get_raw_data entering, fileName:%s, sdcard ready:%d\n", fileName,sdcard_is_ready());
    
	handle = FAT_fopen(fileName, "rb");
	if(!handle) 	
	{
		debug("get_raw_data open FAT_fopen %s failed\n", fileName);		
        return -1;
	}	
    *width = 400;
	*height = 400;
	imgLen = *width * *height;
    *raw = malloc(imgLen);
	if(!(*raw))
	{
        warning("malloc memory failed\n");
		return -1;
	}
    n = FAT_fread(*raw, 1,imgLen,handle);
	if(n<0){
		debug("FAT read error: %d\n",n);
		return -1;
	}

    debug("read %d bytes\n", n);

	//close file
	FAT_fclose(handle);
	
	return 0;
	
}
static int save_raw_data(const char* fileName, char* raw, int size)
{
	unsigned int handle,n;

	handle = FAT_fopen(fileName, "wbc");
	if(!handle) 	
	{
		warning("save_raw_data open FAT_fopen %s failed\n", fileName);		
		return -1;
	}	

    n = FAT_fwrite(raw, sizeof(char), size, handle);
	if(n!=size){
		warning("FAT write error: %d(%d)\n",n,size);
		return -1;
	}
	
	//close file
	FAT_fclose(handle);
	return 0;
}

int qr_test ()
{
    int width = 0, height = 0, n=0, ret;
    void *raw = NULL;
  	const zbar_symbol_t *symbol;
  	zbar_image_t *image;
  	char input;
  
  	debug("############ qrcode test##############");
  	gstQrTime.timer1Start = qrGetTimeMs();
    /* create a reader */
    scanner = zbar_image_scanner_create();
    /* configure the reader */
    zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);
 
    /* wrap image data */
    image = zbar_image_create();
    zbar_image_set_format(image, zbar_fourcc('Y','8','0','0'));
    #ifdef IMAGE_DATA_FROM_FILE
  	ret = get_raw_data(INPUT_IMG, &width, &height, &raw);
  	if(ret)
  		return -1;
    zbar_image_set_size(image, width, height);  		
    zbar_image_set_data(image, raw, width * height, zbar_image_free_data);
    #else
    raw = img_mono;
  	width = 400;
  	height = 400;
    zbar_image_set_size(image, width, height);  	
    zbar_image_set_data(image, raw, width * height, NULL);
	#endif
    /* scan the image for barcodes */
    n = zbar_scan_image(scanner, image);

    /* extract results */
    symbol = zbar_image_first_symbol(image);
	if(!symbol)
	{
 	    goto exit;
	}
	
    for(; symbol; symbol = zbar_symbol_next(symbol)) {
        /* do something useful with results */
        zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
        const char *data = zbar_symbol_get_data(symbol);
	      if(data)
            debug("decoded %s symbol \"%s\"\n",
                  zbar_get_symbol_name(typ), data);
		    else
			      debug("decoded NULL symbol\n");
		    debug("symbol type:%s", zbar_get_symbol_name(typ));
    }
exit:
    
    gstQrTime.timer1Stop = qrGetTimeMs();
    /* clean up */
    zbar_image_destroy(image);	
    zbar_image_scanner_destroy(scanner);
    qrTimePrint();
	 
    debug("############ qrcode test done##############");

    return(0);

}

#ifdef QRCODE_ENABLE_DIAG
//ICAT EXPORTED FUNCTION - A0_QRCODE,QRCODETEST, Qrcode_SetLog
void Create_Test_Qrcode_SetLog(void* para, int lenght)
{
    int logLevel,logMethod;

	logLevel = *((int*)para);
	logMethod = *((int*)para+1); 
	warning("set logLevel:%d,LogMethod:%d", logLevel, logMethod);
	qrSetLog(logLevel, logMethod);
}

//ICAT EXPORTED FUNCTION - A0_QRCODE,QRCODETEST,Qrcode_Test
void Create_Test_Qrcode_Test(void* para, int lenght)
{
	qr_test();
}
#endif

