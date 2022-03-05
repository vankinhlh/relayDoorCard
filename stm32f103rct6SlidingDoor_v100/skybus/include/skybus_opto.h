
/** 
  ******************************************************************************
  * @file    skybus_opto.h
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

#ifndef __skybus_OPTO_H
#define __skybus_OPTO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
/* Exported types ------------------------------------------------------------*/
typedef struct
{
    bool disable;
    uint8_t timeOut;
    uint8_t count;
    /// only app for one button
    uint8_t direct;
}skybus_opto_status_t;

typedef struct
{
    /// bien Status opto
    skybus_opto_status_t OPTO1;
    skybus_opto_status_t OPTO2;
    skybus_opto_status_t OPTO3;
    skybus_opto_status_t OPTO4;
    
    /// opto CBQ
    uint8_t CBQ_cnt;
    
    /// bien delay cho phep nut nhan giua 2 lan gan nhat
    bool opto_button_delay;
    
}skybus_opto_t;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
bool skybus_opto_runClose(void);
bool skybus_opto_runOpen(void);
bool skybus_opto_enableSystem(void);
bool skybus_opto_disableSystem(void);
bool skybus_opto_one_button_direct(void);
bool skybus_opto_one_button_CBQ(void);

void skybus_opto_process(void);
#ifdef __cplusplus
}
#endif

#endif /* __skybus_OPTO_H */
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

