
/** 
  ******************************************************************************
  * @file    skybus_flash.h
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

#ifndef __skybus_FLASH_H
#define __skybus_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "stm32f1xx_hal_flash.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
/* Exported types ------------------------------------------------------------*/
typedef enum
{
    TIME_STOP = 0,
    TIME_CBD_RUN_CLOSE = 1,
    TIME_CBD_RUN_OPEN = 2,
}skybus_flash_data_t;

typedef struct
{
    uint32_t variables;
    
    uint16_t EEPROM_data[10];
    

}skybus_flash_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define DATA_START_ADDRESS      ((uint32_t)0x0801FC00)	//Page 127
#define LENGTH_START_ADDRESS    ((uint32_t)0x0801F810)	//Page 126
/* Exported functions --------------------------------------------------------*/
void FLASH_PageErase(uint32_t PageAddress);
skybus_flash_t* skybus_flash_init(void);
skybus_flash_t* flash_(void);
void skybus_flash_test(void);
#ifdef __cplusplus
}
#endif

#endif /* __skybus_FLASH_H */
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

