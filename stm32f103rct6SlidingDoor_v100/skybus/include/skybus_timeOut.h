
/** 
  ******************************************************************************
  * @file    skybus_timeOut.h
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

#ifndef __skybus_TIMEOUT_H
#define __skybus_TIMEOUT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdlib.h"
#include "stdbool.h"
/* Exported types ------------------------------------------------------------*/
typedef enum
{
    RUNLED_TOGGLE,
    OLED_TEST,
    TIME_MOTOR_RUN_CLOSE,
    TIME_MOTOR_RUN_OPEN,
    TIME_MOTOR_RUNCLOSE_OVER_CURRENT,
    TIME_MOTOR_RUNOPEN_OVER_CURRENT,
    TIME_RUNMOTOR_DISPLAY_OLED,
    TIME_ON_COMPARE_CURRENT,
    TIME_ENABLE_SETTING_LOACTION_REMOTE,
    OPTO_CHECK,
    TIME_OUT_CBQ,
    TIME_OUT_RORC,
    TIME_OUT_DELAY_REMOTE_BUTTON,
    TIME_OUT_DELAY_OPTO_BUTTON,
    

}skybus_timeOut_number_task_t;
typedef struct
{
    /** @brief ham get task timeOut
        @param[in]  time : time check
        @param[in]  task : task number
        @return percent speed motor
    */
    bool(*get)(uint32_t time, uint8_t task);
    /** @brief ham set task timeOut
        @param[in]  task : task number
        @return percent speed motor
    */
    bool(*rst)(uint8_t task);
}skybus_timeOut_t;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
skybus_timeOut_t* skybus_timeOut_init(void);
skybus_timeOut_t* tO(void);
#ifdef __cplusplus
}
#endif

#endif /* __skybus_TIMEOUT_H */
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

