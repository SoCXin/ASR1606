/**
 * @file        cm_demo_i2c.c
 * @brief       OpenCPU i2c测试例程
 * @copyright   Copyright ? 2021 China Mobile IOT. All rights reserved.
 */
 

 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
#include "cm_i2c.h"
#include "cm_os.h"
#include "cm_demo_uart.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define CM_DEMO_I2C_LOG     cm_demo_printf

#define EPROM_DEV_ADDR   	0x50
#define EPROM_I2C_ID	    CM_I2C_DEV_1


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
/**
 * @brief eeprom写1字节数据
 *
 * @param [in] addr:地址，范围0-0x7fff
 * @param [in] data:待写入数据
 *
 * @return  
 *   = 0  - 成功.
 *   < 0  - 失败, 返回值为错误码.
 *
 * @details More details
 */
static int32_t is24c256_write_byte(uint16_t addr, int8_t data)
{
	int8_t  tmp[3] = {0};
	int32_t ret = -1;

	tmp[0] = (addr >> 8) & 0xff;
	tmp[1] = addr & 0xff;
	tmp[2] = data;

	ret = cm_i2c_write(EPROM_I2C_ID , EPROM_DEV_ADDR, tmp, 3);
	if(ret < 0)
	{
		CM_DEMO_I2C_LOG("i2c write e2prom err:0x%x\r\n", ret);
		return -1;
	}
		
	return 0;
}


/**
 * @brief eeprom读1字节数据
 *
 * @param [in] addr:地址，范围0-0x7fff
 * @param [out] data:待读取数据指针
 *
 * @return  
 *   = 0  - 成功.
 *   < 0  - 失败, 返回值为错误码.
 *
 * @details More details
 */
static int32_t is24c256_read_byte(uint16_t addr, int8_t* data)
{
	int8_t tmp[2] = {0};
	int32_t ret;

	if(data == NULL)
	{
		CM_DEMO_I2C_LOG("is24c256_read_byte data ptr err\r\n");
		return -1;
	}

	tmp[0] = (addr >> 8) & 0xff;
	tmp[1] = addr & 0xff;

	ret = cm_i2c_write(EPROM_I2C_ID, EPROM_DEV_ADDR, tmp, 2);
	if(ret < 0)
	{
		CM_DEMO_I2C_LOG("i2c read addr err(w):%08x\r\n", ret);
		return ret;
	}

	ret = cm_i2c_read(EPROM_I2C_ID, EPROM_DEV_ADDR, data, 1);
	if(ret < 0)
	{
		CM_DEMO_I2C_LOG("i2c read addr err(r):%08x\r\n", ret);
		return ret;
	}

	return 0;	
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @brief  eeprom利用i2c接口读写数据测试
 *
 * @return  void
 *
 * @details More details
 */
int cm_test_i2c(unsigned char **cmd, int len)
{

	cm_i2c_cfg_t config = 
	{
		CM_I2C_ADDR_TYPE_7BIT,
		CM_I2C_MODE_MASTER, //目前仅支持模式
		CM_I2C_CLK_100KHZ
	};//master模式,(100KHZ)

	uint16_t eprom_addr = 0x100; //选取所支持的任意E2PROM地址进行测试,可修改
	int8_t w_data = 'B',r_data =0;
	int32_t ret;

	CM_DEMO_I2C_LOG("i2c test start，i2c num:%d!!\n",EPROM_I2C_ID);

	ret = cm_i2c_open(EPROM_I2C_ID, &config);
	if(ret != 0)
	{
		CM_DEMO_I2C_LOG("i2c init err,ret=%d\n", ret);
		return -1;
	}
	CM_DEMO_I2C_LOG("i2c init ok\n");
	
	//写入测试数据
	ret = is24c256_write_byte(eprom_addr, w_data);
	if(ret != 0)
	{
		CM_DEMO_I2C_LOG("i2c write e2prom err %d\n", (uint32_t)ret);
		cm_i2c_close(EPROM_I2C_ID);
		return -1;
	}
	CM_DEMO_I2C_LOG("i2c write e2prom ok\n");
	osDelay(10);//延时等待写入完成

	//读取数据
	ret = is24c256_read_byte(eprom_addr, &r_data);
	cm_i2c_close(EPROM_I2C_ID);

	if(ret != 0)
	{
		CM_DEMO_I2C_LOG("i2c read e2prom err:%d\n", ret);
		return -1;
	}

	CM_DEMO_I2C_LOG("i2c read e2prom,%c\n",r_data);
	CM_DEMO_I2C_LOG("i2c test end!!\n");

	return 0;

}



