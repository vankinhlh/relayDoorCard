
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
#include "skybus_oled_handle.h"
#include "fonts_dma.h"
#include "ssd1306_dma.h"
#include "image.h"
#include "skybus_timeOut.h"
#include <stdio.h>
#include "skybus_relayMotor.h"
#include "skybus_setting.h"
#include "skybus_process.h"
#include "skybus_eeprom.h"
#include "skybus_remote.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t countTask[4];
extern skybus_relayMotor_t* motor;
extern skybus_setting_t* setting;
extern __IO uint16_t adc_value[1];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @group skybus_OLED_CLEAR_SCREEN
    @{
*/#ifndef skybus_OLED_CLEAR_SCREEN
#define skybus_OLED_CLEAR_SCREEN

/** @brief ham xoa man hinh oled
    @return 
*/
void skybus_oled_clear_screen(void)
{
    char buff[100];
    
    sprintf(buff, "                           ");
    
    ssd1306_SetCursor (5,15);
    ssd1306_WriteString (buff, Font_7x10);
    ssd1306_SetCursor (5,25);
    ssd1306_WriteString (buff, Font_7x10);
    ssd1306_SetCursor (5,35);
    ssd1306_WriteString (buff, Font_7x10);
    ssd1306_SetCursor (5,45);
    ssd1306_WriteString (buff, Font_7x10);
    
    ssd1306_UpdateScreen();
}

#endif
/**
    @}
*/

/** @group skybus_OLED_DISPLAY
    @{
*/#ifndef skybus_OLED_DISPLAY
#define skybus_OLED_DISPLAY

/** @brief ham hien thi man hinh home
    @return none
*/
void skybus_oled_home_display(void)
{
    char buff[50];
    
    if(setting->clear == false)
    {
        setting->clear = true;
        skybus_oled_clear_screen();
    }
    //***********************************************************************************
//    sprintf(buff, "HLP v300 : %4d", adc_value[0
    #if (USE_ONE_BUTTON == 1)
        sprintf(buff, "HLP v421");
    #else
        sprintf(buff, "HLP v420");
    #endif
      
    ssd1306_SetCursor (40,0);
    ssd1306_WriteString (buff, Font_7x10);
    //***********************************************************************************
    if(motor->motorStates != RELAY_MOTOR_RUN_STOP)
    {
        if(tO()->get(500, TIME_RUNMOTOR_DISPLAY_OLED))
        {
            sys_process_()->time_runMotor-=500;
        }
        
        /// limit time display oled
        if((int16_t)sys_process_()->time_runMotor < 0)
        {
            sys_process_()->time_runMotor = 0;
        }
    }
    else
    {
        sys_process_()->time_runMotor = 0;
    }
    
    sprintf(buff, "timeStop : %5d", sys_process_()->time_runMotor);
    ssd1306_SetCursor (5,15);
    ssd1306_WriteString (buff, Font_7x10);
    
    //***********************************************************************************
    sprintf(buff, "Current : %5d", sys_process_()->currentPresent);
    ssd1306_SetCursor (5,25);
    ssd1306_WriteString (buff, Font_7x10);
    
//    sprintf(buff, "%4d  %4d", sys_process_()->currentOffset, adc_value[0]);
//    ssd1306_SetCursor (5,35);
//    ssd1306_WriteString (buff, Font_7x10);
    
    //***********************************************************************************
    if(motor->motorStates == RELAY_MOTOR_RUN_CLOSE)
    {
        sprintf(buff, "  runClose  ");
    }
    else if(motor->motorStates == RELAY_MOTOR_RUN_OPEN)
    {
        sprintf(buff, "  runOpen  ");
    }
    else if(motor->motorStates == RELAY_MOTOR_RUN_STOP)
    {
        sprintf(buff, " stopMotor ");
    }
    ssd1306_SetCursor (30,45);
    ssd1306_WriteString (buff, Font_7x10);
    //***********************************************************************************
}
/** @brief ham hien thi page 1 (cac thong so setting)
    @return none
*/
void skybus_oled_page_display(void)
{
    char buff[100];
    
    /// clear screen
    if(setting->clear == false)
    {
        setting->clear = true;
        skybus_oled_clear_screen();
    }
    
    /// next page
    if(setting->mode == MODE_IDE)
    {
        if(setting->getValue(Button2_GPIO_Port, Button2_Pin) == true)
        {
            setting->page++;
            setting->clear = false;
        }
    }
    switch(setting->page)
    {
        case PAGE_OLED_HOME :
        {
            skybus_oled_home_display();
        }break;
        case PAGE_OLED_ONE :
        {
            /// hien thi cac thong so setting
            sprintf(buff, "timeStop:%4d", eeprom_()->data[TIME_STOP_DEFAULF]);
            ssd1306_SetCursor (5,15);ssd1306_WriteString (buff, Font_7x10);
            
            sprintf(buff, "Current:%4d", eeprom_()->data[CURRENT_DEFAULT]);
            ssd1306_SetCursor (5,25);ssd1306_WriteString (buff, Font_7x10);
            
            sprintf(buff, "TcurrentRC:%4d", eeprom_()->data[TIME_OVER_CURRENT_RUN_CLOSE]);
            ssd1306_SetCursor (5,35);ssd1306_WriteString (buff, Font_7x10);
            
            sprintf(buff, "TcurrentRO:%4d", eeprom_()->data[TIME_OVER_CURRENT_RUN_OPEN]);
            ssd1306_SetCursor (5,45);ssd1306_WriteString (buff, Font_7x10);
        }break;
        case PAGE_OLED_TWO :
        {
            /// clear screen
            if(setting->clear == false)
            {
                setting->clear = true;
                skybus_oled_clear_screen();
            }
            
            /// hien thi cac thong so setting
            sprintf(buff, "TonCurrent:%4d", eeprom_()->data[TIME_ON_COMPARE_CURRENT_SETTNG]);
            ssd1306_SetCursor (5,15);ssd1306_WriteString (buff, Font_7x10);
            
            sprintf(buff, "cOffset:%4d", eeprom_()->data[CURRENT_OFFSET]);
            ssd1306_SetCursor (5,25);ssd1306_WriteString (buff, Font_7x10);
            
            sprintf(buff, "timeRORC:%4d", eeprom_()->data[SETTING_TIME_RORC]);
            ssd1306_SetCursor (5,35);ssd1306_WriteString (buff, Font_7x10);
            
            sprintf(buff, "distance:%4d", eeprom_()->data[SETTING_DISTANCE]);
            ssd1306_SetCursor (5,45);ssd1306_WriteString (buff, Font_7x10);
            
        }break;
        case PAGE_OLED_THREE :
        {
            /// clear screen
            if(setting->clear == false)
            {
                setting->clear = true;
                skybus_oled_clear_screen();
            }
            
            /// hien thi cac thong so setting
            sprintf(buff, "DISTANCE RUN");
            ssd1306_SetCursor (5,15);ssd1306_WriteString (buff, Font_7x10);
            
            sprintf(buff, "   %4d", sys_process_()->distance);
            ssd1306_SetCursor (5,25);ssd1306_WriteString (buff, Font_7x10);
            
        }break;
        case PAGE_OLED_END :
        {
            setting->page = 0;
        }break;
    }
    
}

/** @brief ham hien thi man hinh setting timeStop
    @return none
*/
void skybus_oled_setting_timeStop_display(void)
{
    char buff[100];
    
    if(setting->clear == false)
    {
        setting->clear = true;
        skybus_oled_clear_screen();
    }
    
    sprintf(buff, "TIME STOP : %4d", eeprom_()->data[TIME_STOP_DEFAULF]);
    ssd1306_SetCursor (5,25);
    ssd1306_WriteString (buff, Font_7x10);
}

/** @brief ham hien thi man hinh setting overCurrent
    @return none
*/
void skybus_oled_setting_overCurrent_display(void)
{
    char buff[100];
    
    if(setting->clear == false)
    {
        setting->clear = true;
        skybus_oled_clear_screen();
    }
    
    sprintf(buff, "Current : %4d", eeprom_()->data[CURRENT_DEFAULT]);
    ssd1306_SetCursor (5,25);
    ssd1306_WriteString (buff, Font_7x10);
}

/** @brief ham hien thi man hinh setting timeOnCompareCurrent
    @return none
*/
void skybus_oled_setting_time_onCompareCurrent_display(void)
{
    char buff[100];
    
    if(setting->clear == false)
    {
        setting->clear = true;
        skybus_oled_clear_screen();
    }
    
    sprintf(buff, "TimeOnC : %4d", setting->buttun2_value_timeOnCompareCurrent);
    ssd1306_SetCursor (5,25);
    ssd1306_WriteString (buff, Font_7x10);
}

/** @brief ham hien thi man hinh setting time_overRunClose
    @return none
*/
void skybus_oled_setting_time_overRunClose_display(void)
{
    char buff[100];
    
    if(setting->clear == false)
    {
        setting->clear = true;
        skybus_oled_clear_screen();
    }
    
    sprintf(buff, "OverRC : %4d", eeprom_()->data[TIME_OVER_CURRENT_RUN_CLOSE]);
    ssd1306_SetCursor (5,25);
    ssd1306_WriteString (buff, Font_7x10);
}

/** @brief ham hien thi man hinh setting time_overRunOpen
    @return none
*/
void skybus_oled_setting_time_overRunOpen_display(void)
{
    char buff[100];
    
    if(setting->clear == false)
    {
        setting->clear = true;
        skybus_oled_clear_screen();
    }
    
    sprintf(buff, "OverRO : %4d", eeprom_()->data[TIME_OVER_CURRENT_RUN_OPEN]);
    ssd1306_SetCursor (5,25);
    ssd1306_WriteString (buff, Font_7x10);
}

/** @brief ham hien thi man hinh setting time_overRunOpen
    @return none
*/
void skybus_oled_setting_time_RORC_display(void)
{
    char buff[100];
    
    if(setting->clear == false)
    {
        setting->clear = true;
        skybus_oled_clear_screen();
    }
    
    sprintf(buff, "timeRORC : %4d", setting->buttun2_value_timeRORC);
    ssd1306_SetCursor (5,25);
    ssd1306_WriteString (buff, Font_7x10);
}

/** @brief ham hien thi man hinh setting time_overRunOpen
    @return none
*/
void skybus_oled_setting_time_dsitance_display(void)
{
    char buff[100];
    
    if(setting->clear == false)
    {
        setting->clear = true;
        skybus_oled_clear_screen();
    }
    
    sprintf(buff, "distance : %4d", eeprom_()->data[SETTING_DISTANCE]);
    ssd1306_SetCursor (5,35);
    ssd1306_WriteString (buff, Font_7x10);
}
#endif
/**
    @}
*/

/** @group skybus_OLED_SETTING_LOCATION_REMOTE_BUTTON
    @{
*/#ifndef skybus_OLED_SETTING_LOCATION_REMOTE_BUTTON
#define skybus_OLED_SETTING_LOCATION_REMOTE_BUTTON

/** @brief ham setting location button remote runClose
    @return true : da setting xong
            false : chua setting xong
*/
bool skybus_oled_setting_remote_button_runClose(void)
{
    bool ret = false;
    
    /// setting remote button
    remote_()->flag_setting_location[REMOTE_LOCATION_RUN_CLOSE] = remote_()->select_location(&remote_()->location[REMOTE_LOCATION_RUN_CLOSE]);
    
    if(remote_()->flag_setting_location[REMOTE_LOCATION_RUN_CLOSE])
    {
        ret = true;
    }
    
    return ret;
}

/** @brief ham setting location button remote runOpen
    @return true : da setting xong
            false : chua setting xong
*/
bool skybus_oled_setting_remote_button_runOpen(void)
{
    bool ret = false;
    
    /// setting remote button
    remote_()->flag_setting_location[REMOTE_LOCATION_RUN_OPEN] = remote_()->select_location(&remote_()->location[REMOTE_LOCATION_RUN_OPEN]);
    
    if(remote_()->flag_setting_location[REMOTE_LOCATION_RUN_OPEN])
    {
        ret = true;
    }
    
    return ret;
}

/** @brief ham setting location button remote stopButton
    @return true : da setting xong
            false : chua setting xong
*/
bool skybus_oled_setting_remote_button_stopButton(void)
{
    bool ret = false;
    
    /// setting remote button
    remote_()->flag_setting_location[REMOTE_LOCATION_STOP] = remote_()->select_location(&remote_()->location[REMOTE_LOCATION_STOP]);
    
    if(remote_()->flag_setting_location[REMOTE_LOCATION_STOP])
    {
        ret = true;
    }
    
    return ret;
}

/** @brief ham setting location button remote vuongButton
    @return true : da setting xong
            false : chua setting xong
*/
bool skybus_oled_setting_remote_button_vuongButton(void)
{
    bool ret = false;
    
    
    /// setting remote button
    remote_()->flag_setting_location[REMOTE_LOCATION_VUONG] = remote_()->select_location(&remote_()->location[REMOTE_LOCATION_VUONG]);
    
    if(remote_()->flag_setting_location[REMOTE_LOCATION_VUONG])
    {
        ret = true;
    }
    
    return ret;
}

/** @brief ham hien thi setting location button remote
    @return none
*/
void skybus_oled_setting_location_remote_button(void)
{
    char buff[100];
    
    /// clear oled
    if(setting->clear == false)
    {
        setting->clear = true;
        skybus_oled_clear_screen();
    }
    if(remote_()->settingState == REMOTE_LOCATION_IDE)
    {
        HAL_Delay(1000);
        remote_()->settingState = REMOTE_LOCATION_RUN_CLOSE;
    }
    /// display setting
    if(remote_()->settingState == REMOTE_LOCATION_RUN_CLOSE)
    {
        /// result button press
         bool result = false;
        result = skybus_oled_setting_remote_button_runClose();
        
        if(result == true)
        {
            sprintf(buff, "runClose : 1");
            
            /// next setting
            remote_()->settingState = REMOTE_LOCATION_RUN_OPEN;
            
            /// cho nut nhan remote tro ve trang thai unpress
            HAL_Delay(500);
        }
        else
        {
            sprintf(buff, "runClose : 0");
        }
    }
    else if(remote_()->settingState == REMOTE_LOCATION_RUN_OPEN)
    {
        /// result button press
        bool result = false; 
        result = skybus_oled_setting_remote_button_runOpen();
        
        if(result == true)
        {
            sprintf(buff, "runOpen : 1");
            
            /// next setting
            remote_()->settingState = REMOTE_LOCATION_STOP;
            
            /// cho nut nhan remote tro ve trang thai unpress
            HAL_Delay(500);
        }
        else
        {
            sprintf(buff, "runOpen : 0");
        }
    }
    else if(remote_()->settingState == REMOTE_LOCATION_STOP)
    {
        /// result button press
        bool result = false;
        result = skybus_oled_setting_remote_button_stopButton();
        
        if(result == true)
        {
            sprintf(buff, "stop : 1");
            
            /// next setting
            remote_()->settingState = REMOTE_LOCATION_VUONG;
            
            /// cho nut nhan remote tro ve trang thai unpress
            HAL_Delay(500);
        }
        else
        {
            sprintf(buff, "stop : 0");
        }
    }
    else if(remote_()->settingState == REMOTE_LOCATION_VUONG)
    {
        /// result button press
        bool result = false;
        result = skybus_oled_setting_remote_button_vuongButton();
        
        if(result == true)
        {
            sprintf(buff, "lock : 1");
            
            /// next setting
            remote_()->settingState = REMOTE_LOCATION_SAVE_EEPROM;
        }
        else
        {
            sprintf(buff, "lock : 0");
        }
    }
    else if(remote_()->settingState == REMOTE_LOCATION_SAVE_EEPROM)
    {
        /// save setting remote location
        remote_()->saveEeprom_Callback();
        
        /// dua ve man hinh chinh
        setting->mode = MODE_IDE;
        
        /// reset bien settingState
        remote_()->settingState = REMOTE_LOCATION_IDE;
        
        remote_()->settingState = REMOTE_LOCATION_SETTING_DONE;
    }
    else if(remote_()->settingState == REMOTE_LOCATION_SETTING_DONE)
    {
        sprintf(buff, "DONE !!!    ");
        
    }
    
    /// display oled all
    if(remote_()->settingState == REMOTE_LOCATION_SETTING_DONE)
    {
        ssd1306_SetCursor (5,25);ssd1306_WriteString (buff, Font_7x10);
        HAL_Delay(1000);
        
        remote_()->enable_setting = false;
        
//        /// dua ve man hinh chinh
//        setting->mode = MODE_IDE;
//        
//        /// reset bien settingState
//        remote_()->settingState = REMOTE_LOCATION_IDE;
    }
    else
    {
        ssd1306_SetCursor (5,25);ssd1306_WriteString (buff, Font_7x10);
    }
}

#endif
/**
    @}
*/

/** @group skybus_OLED_SETTING_VARIABLES_MOTOR
    @{
*/#ifndef skybus_OLED_SETTING_VARIABLES_MOTOR
#define skybus_OLED_SETTING_VARIABLES_MOTOR

/** @brief 
    @return 
*/
void skybus_oled_setting(void)
{
    if(setting->mode == MODE_IDE)
    {
        skybus_oled_page_display();
//        if(remote_()->enable_setting == false)
//        {
//            skybus_oled_page_display();
//        }
//        else
//        {
//            skybus_oled_setting_location_remote_button();
//        }
    }
    else if(setting->mode == TIME_STOP_DEFAULF)
    {
        skybus_oled_setting_timeStop_display();
    }
    else if(setting->mode == TIME_OVER_CURRENT_RUN_CLOSE)
    {
        skybus_oled_setting_time_overRunClose_display();
    }
    else if(setting->mode == TIME_OVER_CURRENT_RUN_OPEN)
    {
        skybus_oled_setting_time_overRunOpen_display();
    }
    else if(setting->mode == CURRENT_DEFAULT)
    {
        skybus_oled_setting_overCurrent_display();
    }
    else if(setting->mode == TIME_ON_COMPARE_CURRENT_SETTNG)
    {
        skybus_oled_setting_time_onCompareCurrent_display();
    }
    else if(setting->mode == SETTING_TIME_RORC)
    {
        skybus_oled_setting_time_RORC_display();
    }
    else if(setting->mode == SETTING_DISTANCE)
    {
        skybus_oled_setting_time_dsitance_display();
    }
    else if(setting->mode == SETTING_LOCATION_REMOTE)
    {
        skybus_oled_setting_location_remote_button();
    }
}

#endif
/**
    @}
*/

/** @group __skybus_OLED_HANDLE_CALLBACK
    @{
*/#ifndef __skybus_OLED_HANDLE_CALLBACK
#define __skybus_OLED_HANDLE_CALLBACK

/** @brief ham callback handle oled
    @return none
*/
void skybus_oled_handle_callback(void)
{
    skybus_oled_setting();
    
    ssd1306_UpdateScreen(); //display
}
#endif
/**
    @}
*/
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

