
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
#include "skybus_setting.h"
#include "skybus_eeprom.h"
#include "skybus_remote.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static skybus_setting_t* setting;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @group skybus_SETTING_VARIABLE_MOTOR
    @{
*/#ifndef skybus_SETTING_VARIABLE_MOTOR
#define skybus_SETTING_VARIABLE_MOTOR

/** @brief get states button setting 1
    @return true : da nhan nut
            false : chua nhan nut
*/
bool skybus_setting_get_states_button1(void)
{
    bool ret = false;
    
    if(!HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin))
    {
        ret = true;
    }
    
    return ret;
}

/** @brief get states button setting 2
    @return true : da nhan nut
            false : chua nhan nut
*/
bool skybus_setting_get_states_button2(void)
{
    bool ret = false;
    
    if(!HAL_GPIO_ReadPin(Button2_GPIO_Port, Button2_Pin))
    {
        ret = true;
    }
    
    return ret;
}

/** @brief get value button press
    @param[in] : button : button setting
    @param[in] : value : gia tri button
    @return none
*/
bool skybus_setting_get_button_value(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    uint8_t i = 0;
    uint8_t noise_cnt = 10;
    bool ret = false;

    /// get button press
    if(!HAL_GPIO_ReadPin(GPIOx, GPIO_Pin))
    {
        /// check noise button
        for(i = 0; i < noise_cnt; i++)
        {
            /// nut nhan unpress
            if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin))
            {
                /// exit    
//                return;
            }
        }
        /// check thoi gian button press van duy tri
        if(i == noise_cnt)
        {
            while(!HAL_GPIO_ReadPin(GPIOx, GPIO_Pin))
            {
            }
        }
        /// 
        ret = true;
        setting->clear = false;
    }
    
    return ret;
}

/** @brief ham luu data setting
    @return FLASH_SETTING_DONE
*/
static skybus_setting_flash_status skybus_setting_flash_data(uint16_t address, uint16_t data)
{
    uint16_t flashStatus;
    
    if(setting->flash == FLASH_SETTING_IDLE)
    {
        setting->flash = FLASH_SETTING_PENDING;
    }
    else if(setting->flash == FLASH_SETTING_PENDING)
    {
        setting->flash = FLASH_PROCESSING;
    }
    else if(setting->flash == FLASH_PROCESSING)
    {
        flashStatus = eeprom_()->write(address, data);
        
        /// kiem tra trang thai flash eeprom
        if(flashStatus == 0)
        {
            setting->flash = FLASH_SETTING_DONE;
        }
    }
    
    return setting->flash;
}
/** @brief doc data setting tu eeprom
    @return FLASH_SETTING_DONE
*/
uint16_t skybus_setting_read_data_from_eeprom(void)
{
    uint16_t status;
    for(uint8_t i = 1; i < END_MODE; i++)
    {
        uint16_t stt = eeprom_()->read(i, &eeprom_()->data[i]);
        
        status = status + stt;
        
     //   HAL_Delay(20);
    }
    
    /// remote eeprom data location remote
    for(uint8_t i = 100; i < 104; i++)
    {
        eeprom_()->read(i, &eeprom_()->data[i]);
    }
    
    /// kiem tra data read from eeprom
    if(eeprom_()->data[TIME_STOP_DEFAULF]              == 0)
    {
        eeprom_()->data[TIME_STOP_DEFAULF]              = DEFAULT_SETTING_TIME_STOP;
    }
    
    /// kiem tra setting over current runClose
    if(eeprom_()->data[TIME_OVER_CURRENT_RUN_CLOSE]    == 0)
    {
        eeprom_()->data[TIME_OVER_CURRENT_RUN_CLOSE]    = DEFAULT_SETTING_CURRENT_RUN_CLOSE;
    }
    
    /// kiem tra setting over current runOpen
    if(eeprom_()->data[TIME_OVER_CURRENT_RUN_OPEN]     == 0)
    {
        eeprom_()->data[TIME_OVER_CURRENT_RUN_OPEN]     = DEFAULT_SETTING_CURRENT_RUN_OPEN;
    }
    
    /// kiem tra setting current defautl
    if(eeprom_()->data[CURRENT_DEFAULT]                == 0)
    {
        eeprom_()->data[CURRENT_DEFAULT]                = DEFAULT_SETTING_CURRENT;
    }
    
    /// kiem tra setting on compare current 
    if(eeprom_()->data[TIME_ON_COMPARE_CURRENT_SETTNG] == 0)
    {
        eeprom_()->data[TIME_ON_COMPARE_CURRENT_SETTNG] = DEFAULT_SETTING_TIME_ON_COMPARE_CURRENT;
    }
    
    /// kiem tra current Offset
    if(eeprom_()->data[CURRENT_OFFSET] == 0)
    {
        setting->currentOffset_eeprom = eeprom_()->data[CURRENT_OFFSET];
    }
    
    /// kiem tra time RORC
    if(eeprom_()->data[SETTING_TIME_RORC] == 0)
    {
        setting->buttun2_value_timeRORC = eeprom_()->data[SETTING_TIME_RORC];
    }
    
    /// kiem tra distance
    if(eeprom_()->data[SETTING_DISTANCE] == 0)
    {
        eeprom_()->data[SETTING_DISTANCE] = DEFAULT_SETTING_DISTANCE;
    }
    
    /// gan gia tri eeprom cho gia tri setting 
//    setting->button2_value_timeStop = eeprom_()->data[TIME_STOP_DEFAULF];
//    setting->button2_value_time_overRunClose = eeprom_()->data[TIME_OVER_CURRENT_RUN_CLOSE];
//    setting->button2_value_time_overRunOpen = eeprom_()->data[TIME_OVER_CURRENT_RUN_OPEN];
//    setting->buttun2_value_overCurrent = eeprom_()->data[CURRENT_DEFAULT];
    setting->buttun2_value_timeOnCompareCurrent = eeprom_()->data[TIME_ON_COMPARE_CURRENT_SETTNG];
    setting->buttun2_value_timeRORC = eeprom_()->data[SETTING_TIME_RORC];
    setting->currentOffset_eeprom = eeprom_()->data[CURRENT_OFFSET];
    
    
    
    /// kiem tra location remote runClose read from eeprom
    if(eeprom_()->data[LOCATION_REMOTE_ADDRESS_RUNCLOSE] == 0)
    {
        remote_()->location[REMOTE_LOCATION_RUN_CLOSE] = LOCATION_NUMBER_IDLE;
    }
    else
    {
        remote_()->location[REMOTE_LOCATION_RUN_CLOSE] = (skybus_remote_location_number_t)eeprom_()->data[LOCATION_REMOTE_ADDRESS_RUNCLOSE];
    }
    
    /// kiem tra location remote runOpen read from eeprom
    if(eeprom_()->data[LOCATION_REMOTE_ADDRESS_RUNOPEN] == 0)
    {
        remote_()->location[REMOTE_LOCATION_RUN_OPEN] = LOCATION_NUMBER_IDLE;
    }
    else
    {
        remote_()->location[REMOTE_LOCATION_RUN_OPEN] = (skybus_remote_location_number_t)eeprom_()->data[LOCATION_REMOTE_ADDRESS_RUNOPEN];
    }
    
    /// kiem tra location remote stop read from eeprom
    if(eeprom_()->data[LOCATION_REMOTE_ADDRESS_STOP] == 0)
    {
        remote_()->location[REMOTE_LOCATION_STOP] = LOCATION_NUMBER_IDLE;
    }
    else
    {
        remote_()->location[REMOTE_LOCATION_STOP] = (skybus_remote_location_number_t)eeprom_()->data[LOCATION_REMOTE_ADDRESS_STOP];
    }
    
    /// kiem tra location remote vuong read from eeprom
    if(eeprom_()->data[LOCATION_REMOTE_ADDRESS_VUONG] == 0)
    {
        remote_()->location[REMOTE_LOCATION_VUONG] = LOCATION_NUMBER_IDLE;
    }
    else
    {
        remote_()->location[REMOTE_LOCATION_VUONG] = (skybus_remote_location_number_t)eeprom_()->data[LOCATION_REMOTE_ADDRESS_VUONG];
    }
    
    return status;
}


/** @brief ham tinh mode setting
    @return gia tri mode
*/
skybus_setting_mode_set_t skybus_setting_mode(void)
{
    skybus_setting_mode_set_t mode;
    
    if(setting->getValue(Button1_GPIO_Port, Button1_Pin) == true)
    {
        setting->button1_value++;
        /// dua bien flash ve trang trai san sang luu data
        setting->flash = FLASH_SETTING_IDLE;
    }

    switch(setting->button1_value)
    {
        case 0 :
        {
            mode = MODE_IDE;
        }break;
        case 1 :
        {
            mode = TIME_STOP_DEFAULF;
        }break;
        case 2 :
        {
            /// luu data khi setting
            skybus_setting_flash_status statusFlash;
            statusFlash = skybus_setting_flash_data(setting->button1_value - 1, eeprom_()->data[setting->button1_value - 1]);
            
            if(statusFlash == FLASH_SETTING_DONE)
            {
                mode = TIME_OVER_CURRENT_RUN_CLOSE;
            }
        }break;
        case 3 :
        {
            /// luu data khi setting
            skybus_setting_flash_status statusFlash;
            statusFlash = skybus_setting_flash_data(setting->button1_value - 1, eeprom_()->data[setting->button1_value - 1]);
            
            if(statusFlash == FLASH_SETTING_DONE)
            {
                mode = TIME_OVER_CURRENT_RUN_OPEN;
            }
        }break;
        case 4 :
        {
            /// luu data khi setting
            skybus_setting_flash_status statusFlash;
            statusFlash = skybus_setting_flash_data(setting->button1_value - 1, eeprom_()->data[setting->button1_value - 1]);
            
            if(statusFlash == FLASH_SETTING_DONE)
            {
                mode = CURRENT_DEFAULT;
            }
        }break;
        case 5 :
        {
            /// luu data khi setting
            skybus_setting_flash_status statusFlash;
            statusFlash = skybus_setting_flash_data(setting->button1_value - 1, eeprom_()->data[setting->button1_value - 1]);
            
            if(statusFlash == FLASH_SETTING_DONE)
            {
                mode = TIME_ON_COMPARE_CURRENT_SETTNG;
            }
        }break;
        case 6:
        {
            /// luu data khi setting
            skybus_setting_flash_status statusFlash;
            statusFlash = skybus_setting_flash_data(setting->button1_value - 1, eeprom_()->data[setting->button1_value - 1]);
            
            if(statusFlash == FLASH_SETTING_DONE)
            {
                mode = CURRENT_OFFSET;
            }
        }break;
        case 7:
        {
            /// luu data khi setting
            skybus_setting_flash_status statusFlash;
            statusFlash = skybus_setting_flash_data(setting->button1_value - 1, eeprom_()->data[setting->button1_value - 1]);
            
            if(statusFlash == FLASH_SETTING_DONE)
            {
                mode = SETTING_TIME_RORC;
            }
        }break;
        case 8:
        {
            /// luu data khi setting
            skybus_setting_flash_status statusFlash;
            statusFlash = skybus_setting_flash_data(setting->button1_value - 1, eeprom_()->data[setting->button1_value - 1]);
            
            if(statusFlash == FLASH_SETTING_DONE)
            {
                mode = SETTING_DISTANCE;
            }
        }break;
        
        case 9:
        {
            /// luu data khi setting
            skybus_setting_flash_status statusFlash;
            statusFlash = skybus_setting_flash_data(setting->button1_value - 1, eeprom_()->data[setting->button1_value - 1]);
            
            if(statusFlash == FLASH_SETTING_DONE)
            {
                mode = SETTING_LOCATION_REMOTE;
            }
        }break;
        
        case END_MODE :
        {
            /// luu data khi setting
            skybus_setting_flash_status statusFlash;
            statusFlash = skybus_setting_flash_data(setting->button1_value - 1, eeprom_()->data[setting->button1_value - 1]);
            
            if(statusFlash == FLASH_SETTING_DONE)
            {
                setting->button1_value = 0;
            }
            
        }break;
    }
    
    return mode;
}

/** @brief ham process button setting
    @return none
*/
void skybus_button_setting_process(void)
{

    skybus_setting_mode_set_t mode = skybus_setting_mode();
    setting->mode = mode;
    
    if(mode == TIME_STOP_DEFAULF)
    {
        if(setting->getValue(Button2_GPIO_Port, Button2_Pin) == true)
        {
//            setting->button2_value_timeStop+=100;
            eeprom_()->data[TIME_STOP_DEFAULF]+=100;
            
            /// gioi han bien gia tri settting timeStop
            if(eeprom_()->data[TIME_STOP_DEFAULF] > 25000) eeprom_()->data[TIME_STOP_DEFAULF] = 3000;
            
//            /// gan gia tri setting vao gia tri luu eeprom
//            eeprom_()->data[TIME_STOP_DEFAULF] = setting->button2_value_timeStop;
        }
        
    }
    else if(mode == TIME_OVER_CURRENT_RUN_CLOSE)
    {
        if(setting->getValue(Button2_GPIO_Port, Button2_Pin) == true)
        {
//            setting->button2_value_time_overRunClose+=100;
            eeprom_()->data[TIME_OVER_CURRENT_RUN_CLOSE]+=100;
            
            /// gioi han bien gia tri time_overCurrent_runClose
            if(eeprom_()->data[TIME_OVER_CURRENT_RUN_CLOSE] > 6000) eeprom_()->data[TIME_OVER_CURRENT_RUN_CLOSE] = 1;
            
//            /// gan gia tri setting vao gia tri luu eeprom
//            eeprom_()->data[TIME_OVER_CURRENT_RUN_CLOSE] = setting->button2_value_time_overRunClose;
        }
        
    }
    else if(mode == TIME_OVER_CURRENT_RUN_OPEN)
    {
        if(setting->getValue(Button2_GPIO_Port, Button2_Pin) == true)
        {
//            setting->button2_value_time_overRunOpen+=100;
            eeprom_()->data[TIME_OVER_CURRENT_RUN_OPEN]+= 100;
            
            /// gioi han gia tri setting overCurrent_runOpen
            if(eeprom_()->data[TIME_OVER_CURRENT_RUN_OPEN] > 6000) eeprom_()->data[TIME_OVER_CURRENT_RUN_OPEN] = 1;
            
            /// gan gia tri setting vao gia tri luu eeprom
//            eeprom_()->data[TIME_OVER_CURRENT_RUN_OPEN] = setting->button2_value_time_overRunOpen;
        }
        
    }
    else if(mode == CURRENT_DEFAULT)
    {
        if(setting->getValue(Button2_GPIO_Port, Button2_Pin) == true)
        {
//            setting->buttun2_value_overCurrent+=50;
            eeprom_()->data[CURRENT_DEFAULT]+=50;
            
            /// gioi han bien gia tri setting overCrrent
            if(eeprom_()->data[CURRENT_DEFAULT] > 3000) eeprom_()->data[CURRENT_DEFAULT] = 1;
            
            /// gan gia tri setting vao gia tri luu eeprom
//            eeprom_()->data[CURRENT_DEFAULT] = setting->buttun2_value_overCurrent;
        }
        
    }
    else if(mode == TIME_ON_COMPARE_CURRENT_SETTNG)
    {
        if(setting->getValue(Button2_GPIO_Port, Button2_Pin) == true)
        {
            setting->buttun2_value_timeOnCompareCurrent+=100;
            
            /// gioi han gia tri bien onCompareCurrent
            if(setting->buttun2_value_timeOnCompareCurrent > 3000) setting->buttun2_value_timeOnCompareCurrent = 1;
            
            /// gan gia tri setting vao gia tri luu eeprom
            eeprom_()->data[TIME_ON_COMPARE_CURRENT_SETTNG] = setting->buttun2_value_timeOnCompareCurrent;
            
        }
    }
    else if(mode == SETTING_TIME_RORC)
    {
        if(setting->getValue(Button2_GPIO_Port, Button2_Pin) == true)
        {
            setting->buttun2_value_timeRORC+=50;
            
            /// gioi han gia tri bien onCompareCurrent
            if(setting->buttun2_value_timeRORC > 1000) setting->buttun2_value_timeRORC = 0;
            
            /// gan gia tri setting vao gia tri luu eeprom
            eeprom_()->data[SETTING_TIME_RORC] = setting->buttun2_value_timeRORC;
        }
    }
    else if(mode == SETTING_DISTANCE)
    {
        if(setting->getValue(Button2_GPIO_Port, Button2_Pin) == true)
        {
            eeprom_()->data[SETTING_DISTANCE]+=100;
            
            /// gioi han gia tri bien onCompareCurrent
            if(eeprom_()->data[SETTING_DISTANCE] > DISTANCE_MAX) eeprom_()->data[SETTING_DISTANCE] = 0;
        }
    }
    
}

#endif
/**
    @}
*/

/** @group skybus_SETTING_INIT
    @{
*/#ifndef skybus_SETTING_INIT
#define skybus_SETTING_INIT

/** @brief ham cau hinh thu vien setting
    @return dia chi con tro setting
*/
skybus_setting_t* skybus_setting_init(void)
{
    ///cap vung nho cho thu vien
    setting = calloc(sizeof(skybus_setting_t), 1);
    /// gan ham
    setting->button1 = skybus_setting_get_states_button1;
    setting->button2 = skybus_setting_get_states_button2;
    setting->getValue = skybus_setting_get_button_value;
    setting->process = skybus_button_setting_process;
    
    /// doc gia tri setting tu eeprom
    setting->readStatus = skybus_setting_read_data_from_eeprom();
    
    return setting;
}

/** @brief ham goi dia chi thu vien setting
    @return dia chi con tro setting
*/
skybus_setting_t* setting_(void)
{
    return setting;
}
#endif
/**
    @}
*/
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

