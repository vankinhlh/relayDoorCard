
/** 
  ******************************************************************************
  * @file    skybus_.c
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
#include "skybus_remote.h"
#include "skybus_timeOut.h"
#include "skybus_setting.h"
#include "skybus_eeprom.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static skybus_remote_t* remote;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*

************ REMOTE DEtail *************

D0 : runClose
D1 : lock
D2 : stop
D3 : runOpen

*/
/** @group skybus_REMOTE_READ_INPUT_DATA
    @{
*/#ifndef skybus_REMOTE_READ_INPUT_DATA
#define skybus_REMOTE_READ_INPUT_DATA

/** @brief ham doc input remote D0
    @return true : da doc duoc
            false : chua doc duoc
*/
bool skybus_remote_read_input_D0(void)
{
    bool ret = false;
    
    if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin))
    {
        ret = true;
    }
    
    return ret;
}

/** @brief ham doc input remote D1
    @return true : da doc duoc
            false : chua doc duoc
*/
bool skybus_remote_read_input_D1(void)
{
    bool ret = false;
    
    if(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin))
    {
        ret = true;
    }
    
    return ret;
}

/** @brief ham doc input remote D2
    @return true : da doc duoc
            false : chua doc duoc
*/
bool skybus_remote_read_input_D2(void)
{
    bool ret = false;
    
    if(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin))
    {
        ret = true;
    }
    
    return ret;
}

/** @brief ham doc input remote D3
    @return true : da doc duoc
            false : chua doc duoc
*/
bool skybus_remote_read_input_D3(void)
{
    bool ret = false;
    
    if(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin))
    {
        ret = true;
    }
    
    return ret;
}

#endif
/**
    @}
*/

/** @group skybus_SETTING_SELECT_BUTTON_REMOTE
    @{
*/#ifndef skybus_SETTING_SELECT_BUTTON_REMOTE
#define skybus_SETTING_SELECT_BUTTON_REMOTE

/** @brief ham setting vi tri button remote
    @param[in] : bien luu gia tri setting vi tri nut nhan
    @return true : da chon vi tri 1 nut xong
            false : dang chon vi tri
*/
bool skybus_setting_select_button_remote(skybus_remote_location_number_t *button)
{
    bool ret = false;
    *button = LOCATION_NUMBER_IDLE;
    
    if(remote->D0() == true)
    {
        
        *button = LOCATION_NUMBER_D0;
    }
    
    if(remote->D1() == true)
    {
        
        *button = LOCATION_NUMBER_D1;
    }
    
    if(remote->D2() == true)
    {
        
        *button = LOCATION_NUMBER_D2;
    }
    
    if(remote->D3() == true)
    {
        
        *button = LOCATION_NUMBER_D3;
    }
    
    if(*button != LOCATION_NUMBER_IDLE)
    {
        ret = true;
    }
    
    return ret;
}

/** @brief ham apply setting location remote
    @param[in] location : lacation number
    @param[in] lacationSelect : lacation remote button
    @param[in] apply : bien apply remote press
    @return none
*/
bool skybus_oled_setting_remote_button_apply_location(
    skybus_remote_location_number_t location,
    bool* apply
)
{
    if(location == LOCATION_NUMBER_D0)
    {
        *apply = remote->D0();
        remote->flag_apply[REMOTE_D0] = 0;
    }
    else if(location == LOCATION_NUMBER_D1)
    {
        *apply = remote->D1();
        remote->flag_apply[REMOTE_D1] = 1;
    }
    else if(location == LOCATION_NUMBER_D2)
    {
        *apply = remote->D2();
        remote->flag_apply[REMOTE_D2] = 2;
    }
    else if(location == LOCATION_NUMBER_D3)
    {
        *apply = remote->D3();
        remote->flag_apply[REMOTE_D3] = 3;
    }
}

/** @brief ham vao che do setting location remote
    @return none
*/
void skybus_remote_setting_location_Callback(void)
{
    if(    (HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin) == GPIO_PIN_RESET)
        && (HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin) == GPIO_PIN_RESET))
    {
        if(tO()->get(2000, TIME_ENABLE_SETTING_LOACTION_REMOTE))
        {
            remote->enable_setting = true;
            setting_()->clear = false;
        }
    }
}

/** @brief ham save eeprom setting location remote
    @return none
*/
void skybus_remote_save_eeprom_location_Callback(void)
{
    eeprom_()->write(LOCATION_REMOTE_ADDRESS_RUNCLOSE, remote->location[REMOTE_LOCATION_RUN_CLOSE]);
    eeprom_()->write(LOCATION_REMOTE_ADDRESS_RUNOPEN, remote->location[REMOTE_LOCATION_RUN_OPEN]);
    eeprom_()->write(LOCATION_REMOTE_ADDRESS_STOP, remote->location[REMOTE_LOCATION_STOP]);
    eeprom_()->write(LOCATION_REMOTE_ADDRESS_VUONG, remote->location[REMOTE_LOCATION_VUONG]);
}

#endif
/**
    @}
*/

/** @group skybus_REMOTE_ONE_BUTTON
    @{
*/#ifndef skybus_REMOTE_ONE_BUTTON
#define skybus_REMOTE_ONE_BUTTON

#if (USE_ONE_BUTTON == 1)
/** @brief getStatus remote button
    @return true : remote button press
            false : remote button unpress
*/
static bool skybus_button_get_status(GPIO_TypeDef* GPIOx, uint32_t pin, bool* flag_disable, uint8_t* timeOut)
{
    uint8_t i = 0;
    bool ret = false;
    uint8_t noise_cnt = 100;
    if(*flag_disable == false)
    {
        /// get button press
        if(!HAL_GPIO_ReadPin(GPIOx, pin))
        {
            #if (USE_DELAY == 1)
                HAL_Delay(300);
            #else
                /// check noise button
                for(i = 0; i < noise_cnt; i++)
                {
                    /// nut nhan unpress
                    if(HAL_GPIO_ReadPin(GPIOx, pin))
                    {
                        ret = false;
                    }
                }
                /// check thoi gian button press van duy tri
                if(i == noise_cnt)
                {
                    while(!HAL_GPIO_ReadPin(GPIOx, pin))
                    {
                        /// check timeOut
                        if(tO()->get(400, OPTO_CHECK))
                        {
                            if(++*timeOut > 5)
                            {
                                *flag_disable = true;
                                return false;
                            }
                        }
                    }
                    tO()->rst(OPTO_CHECK);
                    *timeOut = 0;
                }
            #endif
            ret = true;
        }
    }
    return ret;
}

/** @brief remote D0 get status
    @return true : D0 press
            false : D0 unpress
*/
bool skybus_remote_get_button_D0_status(void)
{
    bool ret = false;
    bool status = skybus_button_get_status(D0_GPIO_Port, D0_Pin, &remote->remote_D0.disable, &remote->remote_D0.timeOut);
    
    if(status == true && remote->remote_D0.disable == false)
    {
        ret = true;
//        remote->remote_D0.count++;
    }
    
    return ret;
}

/** @brief remote D1 get status
    @return true : D1 press
            false : D1 unpress
*/
bool skybus_remote_get_button_D1_status(void)
{
    bool ret = false;
    bool status = skybus_button_get_status(D1_GPIO_Port, D1_Pin, &remote->remote_D1.disable, &remote->remote_D1.timeOut);
    
    if(status == true && remote->remote_D1.disable == false)
    {
        ret = true;
//        remote->remote_D1.count++;
    }
    
    return ret;
}

/** @brief remote D2 get status
    @return true : D2 press
            false : D2 unpress
*/
bool skybus_remote_get_button_D2_status(void)
{
    bool ret = false;
    bool status = skybus_button_get_status(D2_GPIO_Port, D2_Pin, &remote->remote_D2.disable, &remote->remote_D2.timeOut);
    
    if(status == true && remote->remote_D2.disable == false)
    {
        ret = true;
//        remote->remote_D2.count++;
    }
    
    return ret;
}

/** @brief remote D3 get status
    @return true : D3 press
            false : D3 unpress
*/
bool skybus_remote_get_button_D3_status(void)
{
    bool ret = false;
    bool status = skybus_button_get_status(D3_GPIO_Port, D3_Pin, &remote->remote_D3.disable, &remote->remote_D3.timeOut);
    
    if(status == true && remote->remote_D3.disable == false)
    {
        ret = true;
//        remote->remote_D3.count++;
    }
    
    return ret;
}
/** @brief 
    @return 
*/
#endif



#endif
/**
    @}
*/


/** @group skybus_REMOTE_INIT
    @{
*/#ifndef skybus_REMOTE_INIT
#define skybus_REMOTE_INIT

/** @brief ham cau hinh thu vien remote
    @return dia chi con tro remote
*/
skybus_remote_t* skybus_remote_init(void)
{
    /// cao vung nho cho thu vien
    remote = calloc(sizeof(skybus_remote_t), 1);
    /// gan ham
    #if (USE_ONE_BUTTON == 1)
        remote->D0 = skybus_remote_get_button_D0_status;
        remote->D1 = skybus_remote_get_button_D1_status;
        remote->D2 = skybus_remote_get_button_D2_status;
        remote->D3 = skybus_remote_get_button_D3_status;
    #else
        remote->D0 = skybus_remote_read_input_D0;
        remote->D1 = skybus_remote_read_input_D1;
        remote->D2 = skybus_remote_read_input_D2;
        remote->D3 = skybus_remote_read_input_D3;
    #endif
    remote->select_location                 = skybus_setting_select_button_remote;
    remote->apply_location_remote_Callback  = skybus_oled_setting_remote_button_apply_location;
    remote->enable_setting_Callback         = skybus_remote_setting_location_Callback;
    remote->saveEeprom_Callback             = skybus_remote_save_eeprom_location_Callback;
    
    return remote;
}
/** @brief ham dia chi thu vien remote
    @return dia chi con tro remote
*/
skybus_remote_t* remote_(void)
{
    return remote;
}
#endif
/**
    @}
*/
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

