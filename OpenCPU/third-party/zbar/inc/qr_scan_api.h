#ifndef __QR_SCAN_API_H__
#define __QR_SCAN_API_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

/**
pScanImg: input qrcode image buffer in formate of "cf"
width,height: image size of the input qrcode image buffer
cf: 0-nv12, 1-I420, 2-rgb888, 3-rgb565
    !!!only support nv12 and I420 as of now
parsedStr:
    qrcode string output
maxLen:
    length of parsedStr buffer
return:
    0-OK, 
    1-parsed string exceeds the limit, 2-scan without result, 
    -1-error
**/
int qrcode_scan(char* pScanImg, short width, short height, unsigned char cf, char* parseStr, short maxLen);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif //end of __QR_SCAN_API_H__
