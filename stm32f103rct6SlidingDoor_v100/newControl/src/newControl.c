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

typedef enum
{
    NEW_CONTROL_DISABLE_SYSTEM,
    NEW_CONTROL_ENABLE_SYSTEM,
}newControlSystemStatus_t;

typedef struct
{
    bool flag;
    uint8_t count;
    uint32_t time;
}buttonManagement_t;

typedef struct
{
    newControlSystemStatus_t system;

    buttonManagement_t disable;
}newControl_t;
/* Private define------------------------------------------------------------------------------*/
#define TIME_BUTTON_POLLING         1000
#define COUNT_BUTTON_2_POLLONG      7
#define COUNT_BUTTON_1_POLLONG      (90)//(60*60*20)
#define COUNT_BUTTON_DIS_POLLING    7
#define TIME_RELAY_AUTO_OFF         1500
#define TIME_BUTTON_1_WAITTING      2000
#define TIME_BUTTON_DIS_WAITTING    2000

/*

Button 1 : khong co tac dung
Button 2 : on relay 1.5s xong off (vo hieu hoa trong 70s)
Button 3 : enable/rs (70s) -> nhan giu lien tuc 2s disable board, off all relay(chi disable) (vo hieu hoa trong 70s)
Button 4 : nhan la enable board (chi enable), 2s sau on relay 1.

*/
/* Private macro------------------------------------------------------------------------------*/
/* Private variables------------------------------------------------------------------------------*/
newControl_t newControl;

/* Private function prototypes------------------------------------------------------------------------------*/
/** @brief newControl_offAllRelay
    @return 
    @function off all realy
*/
static void newControl_offAllRelay(void);

/** @brief newControl_relay1State
    @param[in] state : trang thai relay on/off
    @return 
*/
static void newControl_relay1State(newControlRelayState_t state);
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
    
    /// power on -> enable system
    newControl.system = NEW_CONTROL_ENABLE_SYSTEM;
    
    /// set button 3 waitting 70s
    newControl.disable.flag = true;
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
static void OptoAndRemote1Process(newControlSystemStatus_t enable)
{
    static uint32_t timeWaitOnRelay = 0;
    static bool flagOnRelay = false;
    
    if(enable == NEW_CONTROL_DISABLE_SYSTEM)
    {
        timeWaitOnRelay = HAL_GetTick();
        
        flagOnRelay = false;
    }
    else
    {
        if(flagOnRelay == false)
        {
            if(HAL_GetTick() - timeWaitOnRelay > 500)
            {
                flagOnRelay = true;
                
                /// on relay 1 cho den khi disable system
                newControl_relay1State(NEW_CONTROL_RELAY_ON);
            }
        }
    }
}

/** @brief OptoAndRemote1Process
    @return 
    @function on 1.5s -> off
*/
static void OptoAndRemote2Process(newControlSystemStatus_t enable)
{
    static GPIO_PinState pinStt[2] = {0, 0};
    static newControlRelayState_t state = NEW_CONTROL_RELAY_OFF;
    static uint32_t timePolling = 0;
    static uint8_t countPolling = 0;
    static bool flagPolling = false;
    static uint32_t timeTurnOffRelay = 0;
    
    if(enable == NEW_CONTROL_DISABLE_SYSTEM)
    {
        state = NEW_CONTROL_RELAY_OFF;
        timePolling = 0;
        countPolling = 0;
        flagPolling = false;
        timeTurnOffRelay = 0;
        
        /// get current time
        timePolling = HAL_GetTick();
        timeTurnOffRelay = HAL_GetTick();
    }
    else
    {
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
                
                if(++countPolling >= COUNT_BUTTON_2_POLLONG)
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
}

/** @brief OptoAndRemoteDisableSys
    @return 
    @function sau 2s disable system
*/
static int8_t OptoAndRemoteDisableSys(void)
{
    static GPIO_PinState pinStt[2] = {0, 0};
    static bool flagButtonRelease[2] = {0, 0};
    static uint32_t timeButtonRelease[2] = {0, 0};
    
    memset(pinStt, GPIO_PIN_SET, 2);
    
    pinStt[0] = HAL_GPIO_ReadPin(IN3_GPIO_Port, IN3_Pin);
    pinStt[1] = HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin);
    
    for(uint8_t i = 0; i < 2; i++)
    {
        if(pinStt[i] == GPIO_PIN_RESET && flagButtonRelease[i] == false)
        {
            flagButtonRelease[i] = true;
            
            timeButtonRelease[i] = HAL_GetTick();
        }
        
        if(flagButtonRelease[i] == true && pinStt[i] == GPIO_PIN_RESET)
        {
            if(HAL_GetTick() - timeButtonRelease[i] > TIME_BUTTON_DIS_WAITTING)
            {
                timeButtonRelease[i] = 0;
                
                return i;
            }
        }
        else
        {
            flagButtonRelease[i] = false;
            
            timeButtonRelease[i] = 0;
        }
    }
    
    return -1;
}

/** @brief OptoAndRemoteEnableSys
    @return 
    @function enable system
*/
static int8_t OptoAndRemoteEnableSys(void)
{
    static GPIO_PinState pinStt[2] = {0, 0};
    static bool flagButtonRelease[2] = {0, 0};
    static uint32_t timeButtonRelease[2] = {0, 0};
    
    memset(pinStt, GPIO_PIN_SET, 2);
    
    pinStt[0] = HAL_GPIO_ReadPin(IN4_GPIO_Port, IN4_Pin);
    pinStt[1] = HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin);
    
    for(uint8_t i = 0; i < 2; i++)
    {
        if(pinStt[i] == GPIO_PIN_RESET && flagButtonRelease[i] == false)
        {
            flagButtonRelease[i] = true;
            
            timeButtonRelease[i] = HAL_GetTick();
        }
        
        if(flagButtonRelease[i] == true && pinStt[i] == GPIO_PIN_RESET)
        {
            if(HAL_GetTick() - timeButtonRelease[i] > 100)
            {
                timeButtonRelease[i] = 0;
                
                return i;
            }
        }
        else
        {
            flagButtonRelease[i] = false;
            
            timeButtonRelease[i] = 0;
        }
    }
    
    return -1;
}

/** @brief OptoAndRemote1Process
    @return 
    @function enable and disable system
*/

/** @brief disableManager
    @return 
    @function disable manager
*/
static void disableManager(buttonManagement_t *disable, uint8_t countCompare)
{
    if(disable->flag == true)
    {
        if(HAL_GetTick() - disable->time > 1000 || disable->time == 0)
        {
            disable->time = HAL_GetTick();
            
            if(++disable->count > countCompare)
            {
                disable->count = 0;
                disable->time = 0;
                disable->flag = false;
            }
        }
    }
}

/** @brief OptoAndRemote1Process
    @return 
    @function enable and disable system
*/
static void OptoAndRemoteEnableAndDisableProcess(newControl_t *control)
{
    static int8_t disablePin = -1;
    static uint32_t timeLed = 0;
    static uint16_t timeLedToggle = 0;
    
    if(control->system == NEW_CONTROL_ENABLE_SYSTEM)
    {
        if(control->disable.flag == true)
        {
            disableManager(&control->disable, COUNT_BUTTON_DIS_POLLING);
        }
        else
        {
            if(disablePin == -1)
            {
                disablePin = OptoAndRemoteDisableSys();
            }
            else
            {
                disablePin = -1;
                
                control->system = NEW_CONTROL_DISABLE_SYSTEM;
                control->disable.flag = true;
                
                /// turn off all relay
                newControl_offAllRelay();
            }
        }

        
        timeLedToggle = 100;
    }
    else if(control->system == NEW_CONTROL_DISABLE_SYSTEM)
    {
        if(disablePin == -1)
        {
            disablePin = OptoAndRemoteEnableSys();
        }
        else
        {
            disablePin = -1;
            
//            NVIC_SystemReset();
            control->system = NEW_CONTROL_ENABLE_SYSTEM;
        }
        
        timeLedToggle = 1000;
    }
    
    OptoAndRemote1Process(control->system);
    OptoAndRemote2Process(control->system);
    
    if(HAL_GetTick() - timeLed >  timeLedToggle)
    {
        timeLed = HAL_GetTick();
        
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
    OptoAndRemoteEnableAndDisableProcess(&newControl);
}


#endif
/**
    @}
*/

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/