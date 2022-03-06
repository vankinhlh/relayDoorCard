/**
  ******************************************************************************
  * @file    newControl.c
  * @author  Gremsy Team
  * @version v100
  * @date    2021
  * @brief   This file contains all the functions prototypes for the  
  *          firmware library.
  *
  ************************************************************
  ******************
  * @par
  * COPYRIGHT NOTICE: (c) 2011 Gremsy.
  * All rights reserved.Firmware coding style V1.0.beta
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/
/* Includes------------------------------------------------------------------------------*/
#include "newControl.h"
/* Private typedef------------------------------------------------------------------------------*/
typedef enum _relayState_t
{
    NEW_CONTROL_RELAY_OFF,
    NEW_CONTROL_RELAY_ON,
    
}newControlRelayState_t;
/* Private define------------------------------------------------------------------------------*/
#define TIME_BUTTON_POLLING 1000
#define COUNT_BUTTON_POLLONG 70
#define TIME_RELAY_AUTO_OFF 1500
/* Private macro------------------------------------------------------------------------------*/
/* Private variables------------------------------------------------------------------------------*/
/** @brief newControl_offAllRelay
    @return 
    @function off all realy
*/
static void newControl_offAllRelay(void);
/* Private function prototypes------------------------------------------------------------------------------*/
/* Private functions------------------------------------------------------------------------------*/

/** @group __NEWCONTROL_CONFIGURATION
    @{
*/#ifndef __NEWCONTROL_CONFIGURATION
#define __NEWCONTROL_CONFIGURATION
/** @brief newControlConfiguration
    @return none
*/
void newControlConfiguration(void)
{
    /// turn off led status
    HAL_GPIO_WritePin(ledRun_GPIO_Port, ledRun_Pin, GPIO_PIN_SET);
    
    /// turn off all relay
    newControl_offAllRelay();
}


#endif
/**
    @}
*/

/** @group __NEWCONTROL_FUNCTION
    @{
*/#ifndef __NEWCONTROL_FUNCTION
#define __NEWCONTROL_FUNCTION
/** @brief newControl_relay1State
    @param[in] state : trang thai relay on/off
    @return 
*/
static void newControl_relay1State(newControlRelayState_t state)
{
    if(state == NEW_CONTROL_RELAY_OFF)
    {
        HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_SET);
    }
    else if(state == NEW_CONTROL_RELAY_ON)
    {
        HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_RESET);
    }
}

/** @brief newControl_relay2State
    @param[in] state : trang thai relay on/off
    @return 
*/
static void newControl_relay2State(newControlRelayState_t state)
{
    if(state == NEW_CONTROL_RELAY_OFF)
    {
        HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_SET);
    }
    else if(state == NEW_CONTROL_RELAY_ON)
    {
        HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_RESET);
    }
}

/** @brief newControl_offAllRelay
    @return 
    @function off all realy
*/
static void newControl_offAllRelay(void)
{
    HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_SET);
}

/** @brief OptoAndRemote1Process
    @return 
    @function on/off
*/
static void OptoAndRemote1Process(void)
{
    static GPIO_PinState pinStt[2] = {0, 0};
    static newControlRelayState_t state = NEW_CONTROL_RELAY_OFF;
    static uint32_t timePolling = 0;
    static uint8_t countPolling = 0;
    static bool flagPolling = false;
    
    memset(pinStt, GPIO_PIN_SET, 2);
    
    if(flagPolling == false)
    {
        pinStt[0] = HAL_GPIO_ReadPin(IN1_GPIO_Port, IN1_Pin);
        pinStt[1] = HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin);
        
        for(uint8_t i = 0; i < 2; i++)
        {
            if(pinStt[i] == GPIO_PIN_RESET)
            {
                HAL_Delay(100);
                
                state = !state;
                
                flagPolling = true;
                
                /// get current time
                timePolling = HAL_GetTick();
                
                break;
            }
        }
    }
    else
    {
        if(HAL_GetTick() - timePolling > TIME_BUTTON_POLLING)
        {
            timePolling = HAL_GetTick();
            
            if(++countPolling >= 2)
            {
                countPolling = 0;
                flagPolling = false;
            }
        }
    }
    
    newControl_relay1State(state);
}

/** @brief OptoAndRemote1Process
    @return 
    @function on 1.5s -> off
*/
static void OptoAndRemote2Process(void)
{
    static GPIO_PinState pinStt[2] = {0, 0};
    static newControlRelayState_t state = NEW_CONTROL_RELAY_OFF;
    static uint32_t timePolling = 0;
    static uint8_t countPolling = 0;
    static bool flagPolling = false;
    static uint32_t timeTurnOffRelay = 0;
    
    memset(pinStt, GPIO_PIN_SET, 2);
    
    if(flagPolling == false)
    {
        pinStt[0] = HAL_GPIO_ReadPin(IN2_GPIO_Port, IN2_Pin);
        pinStt[1] = HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin);
        
        for(uint8_t i = 0; i < 2; i++)
        {
            if(pinStt[i] == GPIO_PIN_RESET)
            {
                HAL_Delay(100);
                
                state = NEW_CONTROL_RELAY_ON;
                
                flagPolling = true;
                
                /// get current time
                timePolling = HAL_GetTick();
                timeTurnOffRelay = HAL_GetTick();
                
                break;
            }
        }
    }
    else
    {
        if(HAL_GetTick() - timePolling > TIME_BUTTON_POLLING)
        {
            timePolling = HAL_GetTick();
            
            if(++countPolling >= COUNT_BUTTON_POLLONG)
            {
                countPolling = 0;
                flagPolling = false;
            }
        }
    }
    
    if(state == NEW_CONTROL_RELAY_ON)
    {
        /// ktra thoi gian turn off relay
        if(HAL_GetTick() - timeTurnOffRelay > TIME_RELAY_AUTO_OFF)
        {
            timeTurnOffRelay = 0;
            
            state = NEW_CONTROL_RELAY_OFF;
        }
    }
    
    newControl_relay2State(state);

}

/** @brief OptoAndRemote1Process
    @return 
    @function enable and disable system
*/
static void OptoAndRemoteEnableAndDisableProcess(void)
{
    static GPIO_PinState pinStt[4] = {0, 0, 0, 0};
    static bool flagSystem = true;
    static uint32_t timeState = 0;
    static uint32_t timeCompare = 100;
    
    memset(pinStt, GPIO_PIN_SET, 4);
    
    pinStt[0] = HAL_GPIO_ReadPin(IN3_GPIO_Port, IN3_Pin);
    pinStt[1] = HAL_GPIO_ReadPin(IN4_GPIO_Port, IN4_Pin);
    pinStt[2] = HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin);
    pinStt[3] = HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin);
    
    for(uint8_t i = 0; i < 4; i++)
    {
        if(pinStt[i] == GPIO_PIN_RESET)
        {
            HAL_Delay(500);
            
            flagSystem = !flagSystem;
            
            break;
        }
    }
    
    if(flagSystem == true)
    {
        OptoAndRemote1Process();
        OptoAndRemote2Process();
        
        timeCompare = 100;
    }
    else
    {
        newControl_offAllRelay();
        
        timeCompare = 1000;
    }
    
    if(HAL_GetTick() - timeState > timeCompare)
    {
        timeState = HAL_GetTick();
        
        HAL_GPIO_TogglePin(ledRun_GPIO_Port, ledRun_Pin);
    }
}

#endif
/**
    @}
*/

/** @group __NEWCONTROL_PROCESS
    @{
*/#ifndef __NEWCONTROL_PROCESS
#define __NEWCONTROL_PROCESS
/** @brief newControlProcess
    @return none
*/
void newControlProcess(void)
{
    OptoAndRemoteEnableAndDisableProcess();
}


#endif
/**
    @}
*/

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/