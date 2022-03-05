
/** 
  ******************************************************************************
  * @file    skybus_setting.h
  * @author  SKYBUS Team
  * @version 
  * @date    
  * @brief   This file contains all the functions prototypes for the  
  *          firmware library.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2020 Skybus. All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __skybus_SETTING_H
#define __skybus_SETTING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
/* Exported types ------------------------------------------------------------*/

typedef enum
{
    MODE_IDE                        = 0,
    TIME_STOP_DEFAULF               = 1,
    TIME_OVER_CURRENT_RUN_CLOSE     = 2,
    TIME_OVER_CURRENT_RUN_OPEN      = 3,
    CURRENT_DEFAULT                 = 4,
    TIME_ON_COMPARE_CURRENT_SETTNG  = 5,
    CURRENT_OFFSET                  = 6,
    SETTING_TIME_RORC               = 7,
    SETTING_DISTANCE                = 8,
    SETTING_LOCATION_REMOTE         = 9,
    END_MODE                        = 10,
    FLASH_SETTING                   = (END_MODE + 1),
    
    
}skybus_setting_mode_set_t;

typedef enum
{
    PAGE_OLED_HOME  = 0,
    PAGE_OLED_ONE   = 1,
    PAGE_OLED_TWO   = 2,
    PAGE_OLED_THREE   = 3,
    PAGE_OLED_END   = 4,
    
}skybus_setting_page_number_t;


typedef enum
{
    FLASH_SETTING_IDLE      = 0,
    FLASH_SETTING_PENDING   = 1,
    FLASH_PROCESSING        = 2,
    FLASH_SETTING_DONE      = 3,
    
}skybus_setting_flash_status;

typedef struct
{
    /// bien gia tri button
    uint16_t button1_value;
    uint16_t mode_old;
//    uint16_t button2_value_timeStop;
//    uint16_t button2_value_time_overRunClose;
//    uint16_t button2_value_time_overRunOpen;
//    uint16_t buttun2_value_overCurrent;
    uint16_t buttun2_value_timeOnCompareCurrent;
    uint16_t buttun2_value_timeRORC;
    uint16_t currentOffset_eeprom;
    
    /// bien chuyen page hien thi cac thong so setting
    uint8_t page;
    
    /// mode setting
    skybus_setting_mode_set_t mode;
    
    /// flag clear screen
    bool clear;
    
    /// read status
    uint16_t readStatus;
    
    /// bien ket qua kiem tra read flash
    uint16_t result;
    
    /// flag flash data to eeprom
    skybus_setting_flash_status flash;

    /** @brief get states button setting 1
        @return true : da nhan nut
                false : chua nhan nut
    */
    bool(*button1)(void);
    
    /** @brief get states button setting 2
        @return true : da nhan nut
                false : chua nhan nut
    */
    bool(*button2)(void);
    
    /** @brief get value button press
        @param[in] : button : button setting
        @param[in] : value : gia tri button
        @return none
    */
    bool(*getValue)(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
    
    /** @brief ham process button setting
        @return none
    */
    void(*process)(void);
    
}skybus_setting_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define DEFAULT_SETTING_TIME_STOP               6500 
#define DEFAULT_SETTING_CURRENT_RUN_CLOSE       3000
#define DEFAULT_SETTING_CURRENT_RUN_OPEN        200
#define DEFAULT_SETTING_CURRENT                 700
#define DEFAULT_SETTING_TIME_ON_COMPARE_CURRENT 1500
#define DEFAULT_SETTING_LOCATION_REMOTE         1111
#define DEFAULT_SETTING_DISTANCE                20000
#define DISTANCE_MAX                            25000
/* Exported functions --------------------------------------------------------*/
/** @brief ham cau hinh thu vien setting
    @return dia chi con tro setting
*/
skybus_setting_t* skybus_setting_init(void);
    
/** @brief ham goi dia chi thu vien setting
    @return dia chi con tro setting
*/
skybus_setting_t* setting_(void);

#ifdef __cplusplus
}
#endif

#endif /* __skybus_SETTING_H */
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

