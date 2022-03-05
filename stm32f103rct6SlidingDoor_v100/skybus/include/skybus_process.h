
/** 
  ******************************************************************************
  * @file    skybus_process.h
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

#ifndef __skybus_PROCESS_H
#define __skybus_PROCESS_H

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
    STATES_IDE          = 0,
    STATES_RUN_CLOSE    = 1,
    STATES_RUN_OPEN     = 2,
    STATES_STOP         = 3,
}skybus_sys_process_states;

typedef enum
{
    REASON_IDE = 0,
    TIME_STOP_RUN_CLOSE = 1,
    TIME_STOP_RUN_OPEN = 2,
    CURRENT_STOP_RUN_CLOSE = 3,
    CURRENT_STOP_RUN_OPEN = 4,
    CURRENT_STOP_RUN_CLOSE_OVER = 5,
    CURRENT_STOP_RUN_OPEN_OVER = 6,
    OVER_CBQ                   = 7,
    BUTTON_DISABLE             = 8,
    BUTTON_REMOTE_STOP         = 9,
    DISTANCE                   =10,
    
}skybus_sys_process_motor_reason_STOP;
    
typedef struct
{
    
    /// get offset current
    uint16_t currentOffset;
    
    /// get current hien tai
    int16_t currentPresent;
    
    /// current setting
    uint16_t currentSet;
    
    /// time runMotor display oled
    uint16_t time_runMotor;
    
    /// flag on compare current
    bool onCompare;
    
    /// flag applyCompare current
    bool applyCompare;
    
    /// bien count reset system
    uint8_t reset;
    
    /// flag kiem tra count reset system
    bool flag_reset;
    
    /// flag kiem tra complete runMotor
    bool runDone;
    
    /// bien retry get Offset current
    bool Offset_retry;
    
    /// flag enable, disable system
    bool enable;
    
    /// bien One button for Remote
    uint8_t remoteOneButton_motorDirect;
    bool remoteOneButton_stop;
    bool remoteOneButton_vuong;
    
    /// bien remote One button apply
    bool remoteOneButton_apply;
    
    /// bien apply runOpen have CBQ
    bool CBQ_enable;
    bool CBQ_apply;
    
    /// bien lan dau runMotor sau khi reset
    bool first_runMotor_remote;
    bool first_runMotor_opto;
    
    /// bien rorc
    bool remote_motor_runOpen;
    bool opto_motor_runOpen;
    bool motor_rorc;
    bool rorc_time_comapre;
    
    
    /// bien delay cho phep nut nhan giua 2 lan gan nhat
    bool remote_button_delay;
    
    /// bien time QD
    int16_t distance;
    
    /// reason stop motor
    skybus_sys_process_motor_reason_STOP    reason_motorStop;
    /** @brief ham stop Motor, reset tat ca cac bien
        @return true : da stop Motor, reset bien
                false : dang process
    */
    bool(*stopMotor_resetVariables)(void);
    
    /** @brief ham get Offset current
        @param[in] Count    : so lan get
        @param[in] ADCValue : gia tri raw
        @return gia Offset current
    */
    uint16_t(*getOffset)(uint16_t Count, uint16_t ADCValue);
    
    /** @brief ham motor system process
        @return none
    */
    void(*system_process)(void);
    
    /** @brief ham apply remote location button
        @return none
    */
    void(*app_remote_process)(void);
    
    /** @brief ham get gia tri thuc Offset Curent
        @return none
    */
    void(*OffsetReal)(void);
}skybus_process_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define USE_APPLY_LOCATOIN_REMOTE   1
#define USE_ONE_BUTTON               1
/* Exported functions --------------------------------------------------------*/
skybus_process_t* skybus_sys_process_init(void);
skybus_process_t* sys_process_(void);
void skybus_process_optoControlSystem(void);
#ifdef __cplusplus
}
#endif

#endif /* __skybus_PROCESS_H */
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

