/*********************************************************
 *  @file    cm_demo_cJSON.c
 *  @brief   OpenCPU GcJSON示例
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by zyf 2022/11/15
 ********************************************************/

 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "cm_demo_uart.h"
#include "cm_demo_cJSON.h"
#include "cm_mem.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/


 
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Data
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
    char *out;cJSON *json;
    
    json=cJSON_Parse(text);
    if (!json) {cm_demo_printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
    else
    {
        out=cJSON_Print(json);
        cJSON_Delete(json);
        cm_demo_printf("%d:%s\n",__LINE__,out);
        cm_free(out);
    }
}


/* Used by some code below as an example datatype. */
struct record {const char *precision;double lat,lon;const char *address,*city,*state,*zip,*country; };

/* Create a bunch of objects as demonstration. */
void create_objects()
{
    cJSON *root,*fmt;char *out;int i;    /* declare a few. */
    /* Our "days of the week" array: */
    const char *strings[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    /* Our matrix: */
    int numbers[3][3]={{0,-1,0},{1,0,0},{0,0,1}};

    /* Here we construct some JSON standards, from the JSON site. */
    
    /* Our "Video" datatype: */
    root=cJSON_CreateObject();  
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
    cJSON_AddItemToObject(root, "format", fmt=cJSON_CreateObject());
    cJSON_AddStringToObject(fmt,"type",     "rect");
    cJSON_AddNumberToObject(fmt,"width",        1920);
    cJSON_AddNumberToObject(fmt,"height",       1080);
    cJSON_AddFalseToObject (fmt,"interlace");
    cJSON_AddNumberToObject(fmt,"frame rate",   24);
    
    out=cJSON_Print(root);  cJSON_Delete(root); cm_demo_printf("%d:%s\n",__LINE__,out); cm_free(out);  /* Print to text, Delete the cJSON, print it, release the string. */

    /* Our "days of the week" array: */
    root=cJSON_CreateStringArray(strings,7);

    out=cJSON_Print(root);  cJSON_Delete(root); cm_demo_printf("%d:%s\n",__LINE__,out); cm_free(out);

    /* Our matrix: */
    root=cJSON_CreateArray();
    for (i=0;i<3;i++) cJSON_AddItemToArray(root,cJSON_CreateIntArray(numbers[i],3));

/*  cJSON_ReplaceItemInArray(root,1,cJSON_CreateString("Replacement")); */
    
    out=cJSON_Print(root);  cJSON_Delete(root); cm_demo_printf("%d:%s\n",__LINE__,out); cm_free(out);

}

/****************************************************************************
 * Public Functions
****************************************************************************/

void cm_test_cJSON(unsigned char **cmd, int len)
{
    
    cm_demo_printf("%d:cm_test_cJSON start",__LINE__);
    /* a bunch of json: */
    char text[]="{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";   

    /* Process each json textblock by parsing, then rebuilding: */
    doit(text);


    /* Now some samplecode for building objects concisely: */
    create_objects();
}

