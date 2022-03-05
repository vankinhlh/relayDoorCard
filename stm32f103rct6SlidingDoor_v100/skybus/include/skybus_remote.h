
/** 
  ******************************************************************************
  * @file    skybus_remote.h
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

#ifndef __skybus_REMOTE_H
#define __skybus_REMOTE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
//#include "skybus_process.h"
/* Exported types ------------------------------------------------------------*/
#define USE_ONE_BUTTON              1
#define USE_DELAY                   0

typedef enum
{
    REMOTE_LOCATION_IDE         = 0,
    REMOTE_LOCATION_RUN_CLOSE   = 1,
    REMOTE_LOCATION_RUN_OPEN    = 2,
    REMOTE_LOCATION_STOP        = 3,
    REMOTE_LOCATION_VUONG       = 4,
    REMOTE_LOCATION_SAVE_EEPROM = 5,
    REMOTE_LOCATION_SETTING_DONE = 6,
    
}skybus_remote_select_t;

typedef enum
{
    LOCATION_NUMBER_IDLE = 0,
    LOCATION_NUMBER_D0 = 1,
    LOCATION_NUMBER_D1 = 2,
    LOCATION_NUMBER_D2 = 3,
    LOCATION_NUMBER_D3 = 4,
    
}skybus_remote_location_number_t;

typedef enum
{
    LOCATION_REMOTE_ADDRESS_RUNCLOSE = 100,
    LOCATION_REMOTE_ADDRESS_RUNOPEN  = 101,
    LOCATION_REMOTE_ADDRESS_STOP     = 102,
    LOCATION_REMOTE_ADDRESS_VUONG    = 103,
}skybus_remote_eeprom_address_t;

typedef enum
{
    REMOTE_D0,
    REMOTE_D1,
    REMOTE_D2,
    REMOTE_D3,
    
}skybus_remote_name_t;

#if (USE_ONE_BUTTON == 1)
typedef struct
{
    bool disable;
    uint8_t timeOut;
//    uint8_t count;
    /// only app for one button
//    uint8_t direct;
}skybus_remote_status_t;
#endif
typedef struct
{
    uint32_t data[4];
    
    /// flag setting location remote button
    bool flag_setting_location[5];
    
    /// mang luu location remote button
    skybus_remote_location_number_t location[5];
    
    /// setting remote location State
    skybus_remote_select_t settingState;
    
    /// bien apply button press
    bool apply[5];
    
    /// flag apply
    uint8_t flag_apply[4];
    
    /// flag cho phep setting
    bool enable_setting;
#if (USE_ONE_BUTTON == 1)
    /// bien status remote
    skybus_remote_status_t remote_D0;
    skybus_remote_status_t remote_D1;
    skybus_remote_status_t remote_D2;
    skybus_remote_status_t remote_D3;
#endif
    /** @brief ham doc input remote D0
        @return true : da doc duoc
                false : chua doc duoc
    */
    bool(*D0)(void);
    /** @brief ham doc input remote D1
        @return true : da doc duoc
                false : chua doc duoc
    */
    
    bool(*D1)(void);
    /** @brief ham doc input remote D2
        @return true : da doc duoc
                false : chua doc duoc
    */
    
    bool(*D2)(void);
    /** @brief ham doc input remote D3
        @return true : da doc duoc
                false : chua doc duoc
    */
    
    bool(*D3)(void);
    /** @brief ham setting vi tri button remote
    @param[in] : bien luu gia tri setting vi tri nut nhan
    @return true : da chon vi tri 1 nut xong
            false : dang chon vi tri
    */
    bool(*select_location)(skybus_remote_location_number_t *button);
    /** @brief ham apply setting location remote
        @param[in] location : lacation number
        @param[in] lacationSelect : lacation remote button
        @param[in] apply : bien apply remote press
        @return none
    */
    
    bool(*apply_location_remote_Callback)(
        skybus_remote_location_number_t location,
        bool* apply
    );
        
    /** @brief ham vao che do setting location remote
        @return none
    */
    void(*enable_setting_Callback)(void);
        
    /** @brief ham save eeprom setting location remote
        @return none
    */
    void(*saveEeprom_Callback)(void);
        
}skybus_remote_t;

//typedef struct
//{
//    uint32_t debug;
//    uint32_t debug1;
//    uint32_t debug2;
//    uint32_t debug3;
//    uint32_t debug4;
//    uint32_t debug5;
//    uint32_t debug6;
//    
//}debug_memory_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @brief ham cau hinh thu vien remote
    @return dia chi con tro remote
*/
skybus_remote_t* skybus_remote_init(void);

/** @brief ham dia chi thu vien remote
    @return dia chi con tro remote
*/
skybus_remote_t* remote_(void);
#ifdef __cplusplus
}
#endif

#endif /* __skybus_REMOTE_H */
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

