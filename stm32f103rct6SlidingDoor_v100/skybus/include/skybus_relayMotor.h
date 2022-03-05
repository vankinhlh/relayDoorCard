
/** 
  ******************************************************************************
  * @file    skybus_relayMotor.h
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

#ifndef __skybus_RELAY_MOTOR_H
#define __skybus_RELAY_MOTOR_H

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
    RELAY_MOTOR_RUN_STOP  = 0,
    RELAY_MOTOR_RUN_CLOSE = 1,
    RELAY_MOTOR_RUN_OPEN  = 2,
}relayMotor_states;

typedef struct
{
    /// motor States
    relayMotor_states motorStates;
    /// time Led toggle runMotor
    uint16_t ledToogleTime;
    
    /** @brief ham dk motor control
        @return relayMotor_states
    */
    relayMotor_states(*relayMotor_control)(relayMotor_states states);
}skybus_relayMotor_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
skybus_relayMotor_t* skybus_relayMotor_init(void);
skybus_relayMotor_t* relayMotor(void);
#ifdef __cplusplus
}
#endif

#endif /* __skybus_RELAY_MOTOR_H */
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

