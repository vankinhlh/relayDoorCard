
/** 
  ******************************************************************************
  * @file    skybus_relayMotor.c
  * @author  SKYBUS Team
  * @version 
  * @date    
  * @brief   
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2020 Skybus.  
  * All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "skybus_relayMotor.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static skybus_relayMotor_t* motor;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @group skybus_RELAT_MOTOR_CONTROL
    @{
*/#ifndef skybus_RELAT_MOTOR_CONTROL
#define skybus_RELAT_MOTOR_CONTROL

/** @brief ham dk motor runClose
    @return none
*/
void skybus_relayMotor_runClose(void)
{
    HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_RESET);
}

/** @brief ham dk motor runOpen
    @return none
*/
void skybus_relayMotor_runOpen(void)
{
    HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_SET);
}

/** @brief ham dk motor stop
    @return none
*/
void skybus_relayMotor_stop(void)
{
    HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_SET);
}

/** @brief ham dk motor control
    @return relayMotor_states
*/
relayMotor_states skybus_relayMotor_control(relayMotor_states states)
{
    if(states == RELAY_MOTOR_RUN_CLOSE)
    {
        skybus_relayMotor_runClose();
    }
    else if(states == RELAY_MOTOR_RUN_OPEN)
    {
        skybus_relayMotor_runOpen();
    }
    else if(states == RELAY_MOTOR_RUN_STOP)
    {
        skybus_relayMotor_stop();
    }
    
    return states;
}

#endif
/**
    @}
*/

skybus_relayMotor_t* skybus_relayMotor_init(void)
{
    /// cap vung nho cho thu vien
    motor = calloc(sizeof(skybus_relayMotor_t), 1);
    /// gan ham
    motor->relayMotor_control = skybus_relayMotor_control;
    
    /// set time init runMotor
    motor->ledToogleTime = 1000;
    
    return motor;
}
skybus_relayMotor_t* relayMotor(void)
{
    return motor;
}
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

