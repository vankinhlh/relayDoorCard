
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
#include "skybus_timeOut.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static skybus_timeOut_t* timeOut;
uint32_t timeGet[30];
bool flagGetTime[30], flagOverTime[30];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @group __skybus_TIMEOUT_GET_RESET_TIME
    @{
*/#ifndef __skybus_TIMEOUT_GET_RESET_TIME
#define __skybus_TIMEOUT_GET_RESET_TIME

/** @brief ham get task timeOut
    @param[in]  time : time check
    @param[in]  task : task number
    @return percent speed motor
*/
static bool skybus_timeOut_getTimeOut(uint32_t time, uint8_t task)
{
    bool ret = 0;

    if(flagGetTime[task] == false)
    {
        flagGetTime[task] = true;
        timeGet[task] = HAL_GetTick() + time;
        flagOverTime[task] = true;
    }
    if(flagOverTime[task] == true && timeGet[task] < HAL_GetTick())
    {
        timeGet[task] = 0;
        flagOverTime[task] = false;
        flagGetTime[task] = false;
        ret = true;
    }

    return ret;
}
/** @brief ham set task timeOut
    @param[in]  task : task number
    @return percent speed motor
*/
static bool skybus_timeOut_reset(uint8_t task)
{
    bool ret = 0;
    
    timeGet[task] = 0;
    flagOverTime[task] = false;
    flagGetTime[task] = false;
    ret = 1;
    
    return ret;
}

#endif
/**
    @}
*/
/** @group __skybus_TIMEOUT_INIT
    @{
*/#ifndef __skybus_TIMEOUT_INIT
#define __skybus_TIMEOUT_INIT

/** @brief ham cau hinh timeOut
    @return dia chi thu vien timeOut
*/
skybus_timeOut_t* skybus_timeOut_init(void)
{
    /// cap vung nho cho thu vien
    timeOut = calloc(sizeof(skybus_timeOut_t), 1);
    
    /// gan ham
    timeOut->get = skybus_timeOut_getTimeOut;
    timeOut->rst = skybus_timeOut_reset;
    
    return timeOut;
}

skybus_timeOut_t* tO(void)
{
    return timeOut;
}
#endif
/**
    @}
*/
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

