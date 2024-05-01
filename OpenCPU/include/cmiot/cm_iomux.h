/**
 * @file        cm_iomux.h
 * @brief       IOMUX接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By zyf
 * @date        2021/07/30
 *
 * @defgroup iomux
 * @ingroup PI
 * @{
 */

#ifndef __CM_IOMUX_H__
#define __CM_IOMUX_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
/****************************************************************************
 * Public Types
 ****************************************************************************/
/**IOMUX PIN  definition 和模组实际PIN脚对应一致,请参照资源综述*/
typedef enum{
    CM_IOMUX_PIN_0 = 0,    
    CM_IOMUX_PIN_1,        
    CM_IOMUX_PIN_2,        
    CM_IOMUX_PIN_3,        
    CM_IOMUX_PIN_4,        
    CM_IOMUX_PIN_5,        
    CM_IOMUX_PIN_6,        
    CM_IOMUX_PIN_7,        
    CM_IOMUX_PIN_8,        
    CM_IOMUX_PIN_9,        
    CM_IOMUX_PIN_10,       
    CM_IOMUX_PIN_11,       
    CM_IOMUX_PIN_12,       
    CM_IOMUX_PIN_13,       
    CM_IOMUX_PIN_14,       
    CM_IOMUX_PIN_15,       
    CM_IOMUX_PIN_16,       
    CM_IOMUX_PIN_17,       
    CM_IOMUX_PIN_18,       
    CM_IOMUX_PIN_19,       
    CM_IOMUX_PIN_20,       
    CM_IOMUX_PIN_21,       
    CM_IOMUX_PIN_22,       
    CM_IOMUX_PIN_23,       
    CM_IOMUX_PIN_24,       
    CM_IOMUX_PIN_25,       
    CM_IOMUX_PIN_26,       
    CM_IOMUX_PIN_27,       
    CM_IOMUX_PIN_28,       
    CM_IOMUX_PIN_29,       
    CM_IOMUX_PIN_30,       
    CM_IOMUX_PIN_31,       
    CM_IOMUX_PIN_32,       
    CM_IOMUX_PIN_33,       
    CM_IOMUX_PIN_34,       
    CM_IOMUX_PIN_35,       
    CM_IOMUX_PIN_36,       
    CM_IOMUX_PIN_37,       
    CM_IOMUX_PIN_38,       
    CM_IOMUX_PIN_39,       
    CM_IOMUX_PIN_40,       
    CM_IOMUX_PIN_41,       
    CM_IOMUX_PIN_42,       
    CM_IOMUX_PIN_43,       
    CM_IOMUX_PIN_44,       
    CM_IOMUX_PIN_45,       
    CM_IOMUX_PIN_46,       
    CM_IOMUX_PIN_47,       
    CM_IOMUX_PIN_48,       
    CM_IOMUX_PIN_49,       
    CM_IOMUX_PIN_50,       
    CM_IOMUX_PIN_51,       
    CM_IOMUX_PIN_52,       
    CM_IOMUX_PIN_53,       
    CM_IOMUX_PIN_54,       
    CM_IOMUX_PIN_55,       
    CM_IOMUX_PIN_56,       
    CM_IOMUX_PIN_57,       
    CM_IOMUX_PIN_58,       
    CM_IOMUX_PIN_59,       
    CM_IOMUX_PIN_60,       
    CM_IOMUX_PIN_61,       
    CM_IOMUX_PIN_62,       
    CM_IOMUX_PIN_63,       
    CM_IOMUX_PIN_64,       
    CM_IOMUX_PIN_65,       
    CM_IOMUX_PIN_66,       
    CM_IOMUX_PIN_67,       
    CM_IOMUX_PIN_68,       
    CM_IOMUX_PIN_69,       
    CM_IOMUX_PIN_70,       
    CM_IOMUX_PIN_71,       
    CM_IOMUX_PIN_72,       
    CM_IOMUX_PIN_73,       
    CM_IOMUX_PIN_74,       
    CM_IOMUX_PIN_75,       
    CM_IOMUX_PIN_76,       
    CM_IOMUX_PIN_77,       
    CM_IOMUX_PIN_78,       
    CM_IOMUX_PIN_79,       
    CM_IOMUX_PIN_80,       
    CM_IOMUX_PIN_81,       
    CM_IOMUX_PIN_82,       
    CM_IOMUX_PIN_83,       
    CM_IOMUX_PIN_84,       
    CM_IOMUX_PIN_85,       
    CM_IOMUX_PIN_86,       
    CM_IOMUX_PIN_87,
    CM_IOMUX_PIN_88,    
    CM_IOMUX_PIN_89,
    CM_IOMUX_PIN_90,
    CM_IOMUX_PIN_91,
    CM_IOMUX_PIN_92,
    CM_IOMUX_PIN_93,
    CM_IOMUX_PIN_94,
    CM_IOMUX_PIN_95,
    CM_IOMUX_PIN_96,
    CM_IOMUX_PIN_97,
    CM_IOMUX_PIN_98,    
    CM_IOMUX_PIN_99,
    CM_IOMUX_PIN_100,
    CM_IOMUX_PIN_101,
    CM_IOMUX_PIN_102,
    CM_IOMUX_PIN_103,
    CM_IOMUX_PIN_104,
    CM_IOMUX_PIN_105,
    CM_IOMUX_PIN_106,
    CM_IOMUX_PIN_107,
    CM_IOMUX_PIN_108,
    CM_IOMUX_PIN_109,
    CM_IOMUX_PIN_110,
    CM_IOMUX_PIN_111,
    CM_IOMUX_PIN_112,
    CM_IOMUX_PIN_113,
    CM_IOMUX_PIN_114,
    CM_IOMUX_PIN_115,
    CM_IOMUX_PIN_116,
    CM_IOMUX_PIN_117,
    CM_IOMUX_PIN_118,
    CM_IOMUX_PIN_119,
    CM_IOMUX_PIN_120,
    CM_IOMUX_PIN_121,
    CM_IOMUX_PIN_122,
    CM_IOMUX_PIN_123,
    CM_IOMUX_PIN_124,
    CM_IOMUX_PIN_125,
    CM_IOMUX_PIN_126,
    CM_IOMUX_PIN_127,
    CM_IOMUX_PIN_128,
    CM_IOMUX_PIN_129,
    CM_IOMUX_PIN_130,
    CM_IOMUX_PIN_131,
    CM_IOMUX_PIN_132,
    CM_IOMUX_PIN_133,
    CM_IOMUX_PIN_134,
    CM_IOMUX_PIN_135,
    CM_IOMUX_PIN_136,
    CM_IOMUX_PIN_137,
    CM_IOMUX_PIN_138,
    CM_IOMUX_PIN_139,
    CM_IOMUX_PIN_140,
    CM_IOMUX_PIN_141,
    CM_IOMUX_PIN_142,
    CM_IOMUX_PIN_143,
    CM_IOMUX_PIN_144,
    CM_IOMUX_PIN_145,
    CM_IOMUX_PIN_146,
    CM_IOMUX_PIN_147,
    CM_IOMUX_PIN_148,
    CM_IOMUX_PIN_149,
    CM_IOMUX_PIN_150,
    CM_IOMUX_PIN_151,
    CM_IOMUX_PIN_152,
    CM_IOMUX_PIN_153,
    CM_IOMUX_PIN_154,      
    CM_IOMUX_PIN_MAX      
} cm_iomux_pin_e;

/*IOMUX FUNC  definition */
typedef enum{
    CM_IOMUX_FUNC_FUNCTION0 ,         /*!<不支持*/
    CM_IOMUX_FUNC_FUNCTION1,          /*!<功能1*/
    CM_IOMUX_FUNC_FUNCTION2,          /*!<功能2*/
    CM_IOMUX_FUNC_FUNCTION3,          /*!<功能3*/
    CM_IOMUX_FUNC_FUNCTIONNUM_END,
} cm_iomux_func_e;


/*IOMUX PIN  cmd*/
typedef enum{
    CM_IOMUX_PINCMD0_SLEEP ,          /*!<休眠状态*/
    CM_IOMUX_PINCMD1_LPMEDEG,         /*!<边沿检测*/
    CM_IOMUX_PINCMD2_DRIVEABILITY,    /*!<驱动能力*/
    CM_IOMUX_PINCMD3_PULL,            /*!<上下拉*/
    CM_IOMUX_PINCMDNUM_END 
} cm_iomux_pincmd_e;

/*IOMUX PIN  cmdnum enumeration*/
typedef enum{
    CM_IOMUX_PINCMD0_FUNC0_SLEEP_NONE,             /*!<不使能 pad 的 sleep 功能*/
    CM_IOMUX_PINCMD0_FUNC1_SLEEP_DIR,              /*!<不使能 sleep，只是设置为 sleep 时候，输入状态*/
    CM_IOMUX_PINCMD0_FUNC2_SLEEP_DATA,             /*!<不使能 sleep，只是设置为 sleep 时候，输出状态*/
    CM_IOMUX_PINCMD0_FUNC3_SLEEP_FLOAT,            /*!<使能 sleep 功能，设置为 sleep 时候，输入状态*/
    CM_IOMUX_PINCMD0_FUNC4_SLEEP_OUTPUT_HIGH, /*!<使能 sleep 功能，设置为 sleep 时候，输出高状态*/
    CM_IOMUX_PINCMD0_FUNC5_SLEEP_OUTPUT_LOW,       /*!<使能 sleep 功能，设置为 sleep 时候，输出低状态*/
    CM_IOMUX_PINCMD0_FUNCNUM_END  
} cm_iomux_pincmd0_e;
    
typedef enum{
    CM_IOMUX_PINCMD1_FUNC0_LPM_EDGE_NONE,        /*!<不使能 pad 的边沿检测功能*/
    CM_IOMUX_PINCMD1_FUNC1_LPM_EDGE_RISE ,       /*!<使能 pad 的边沿检测功能，且是上升沿。并提供唤醒事件*/
    CM_IOMUX_PINCMD1_FUNC2_LPM_EDGE_FALL,        /*!<使能 pad 的边沿检测功能，且是下降沿。并提供唤醒事件*/
    CM_IOMUX_PINCMD1_FUNC3_EDGE_BOTH,            /*!<使能 pad 的边沿检测功能，且是双沿。并提供唤醒事件*/
    CM_IOMUX_PINCMD1_FUNCNUM_END 
} cm_iomux_pincmd1_e;
    
typedef enum{
    CM_IOMUX_PINCMD2_FUNC0_DRIVE_VERY_SLOW,
    CM_IOMUX_PINCMD2_FUNC1_DRIVE_SLOW, 
    CM_IOMUX_PINCMD2_FUNC2_DRIVE_MEDIUM,
    CM_IOMUX_PINCMD2_FUNC3_DRIVE_FAST,
    CM_IOMUX_PINCMD2_FUNC4_MASK,
    CM_IOMUX_PINCMD2_FUNCNUM_END
} cm_iomux_pincmd2_e;

typedef enum{
    CM_IOMUX_PINCMD3_FUNC0_PULL_NONE,              /*!<不使能内部的上拉下拉功能*/
    CM_IOMUX_PINCMD3_FUNC1_PULL_LOW,               /*!<使能下拉*/
    CM_IOMUX_PINCMD3_FUNC2_PULL_HIGH,              /*!<使能上拉*/
    CM_IOMUX_PINCMD3_FUNC3_PULL_BOTH,              /*!<使能上下拉*/
    CM_IOMUX_PINCMD3_FUNC4_PULL_FLOAT,             /*!<浮空功能*/
    CM_IOMUX_PINCMD3_FUNCNUM_END
} cm_iomux_pincmd3_e;

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/*IOMUX PIN FUNCTION  definition*/


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
 * @brief IOMUX 设置引脚复用功能
 *
 * @param [in] pin PIN 定义号
 * @param [in] fun FUN 定义号
 *
 *  @return  
 *    = 0  - 成功 \n
 *    = -1 - 失败
 */
int32_t cm_iomux_set_pin_func(cm_iomux_pin_e pin, cm_iomux_func_e fun);

/**
 * @brief IOMUX 获取引脚功能
 *
 * @param [in] pin PIN 定义号
 * @param [out] fun FUN 定义号
 *
 * @return  
 *    = 0  - 成功\n
 *    < 0  - 失败
 */
int32_t cm_iomux_get_pin_func(cm_iomux_pin_e pin, cm_iomux_func_e *fun);
 
/**
 * @brief IOMUX PIN脚控制
 *
 * @param [in] pin PIN 定义号
 * @param [in] cmd 功能定义
 * @param [in] cmd_arg 具体功能值
 *
 * @return  
 *    = 0  - 成功\n
 *    < 0  - 失败
 */
int32_t cm_iomux_set_pin_cmd(cm_iomux_pin_e pin, cm_iomux_pincmd_e cmd, uint8_t cmd_arg);


#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_IOMUX_H__ */

/** @}*/
