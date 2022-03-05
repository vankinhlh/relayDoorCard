
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
#include "skybus_opto.h"
#include "skybus_timeOut.h"
#include "skybus_process.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
skybus_opto_t        opto; 
extern skybus_process_t*    sys_process;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @group skybus_OPTO_GET_STATUS
    @{
*/#ifndef skybus_OPTO_GET_STATUS
#define skybus_OPTO_GET_STATUS

/** @brief getStatus opto
    @return true : opto press
            false : opto unpress
*/
static bool skybus_button_get_status(GPIO_TypeDef* GPIOx, uint32_t pin, bool* flag_disable, uint8_t* timeOut)
{
    uint8_t i = 0;
    bool ret = false;
    uint8_t noise_cnt = 100;
    if(*flag_disable == 0)
    {
        /// get button press
        if(!HAL_GPIO_ReadPin(GPIOx, pin))
        {
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
                        if(++*timeOut > 35)
                        {
                            *flag_disable = true;
                            return false;
                        }
                    }
                }
                tO()->rst(OPTO_CHECK);
                *timeOut = 0;
            }
            ret = true;
        }
    }
    return ret;
}

/** @brief getStatus cbq
    @return true : remote button press
            false : remote button unpress
*/
static bool skybus_button_get_status_cbq(GPIO_TypeDef* GPIOx, uint32_t pin, bool* flag_disable, uint8_t* timeOut)
{
    bool ret = false;

    if(*flag_disable == false)
    {
        /// get button press
        if(!HAL_GPIO_ReadPin(GPIOx, pin))
        {
            /// check timeOut
            if(tO()->get(400, OPTO_CHECK))
            {
                if(++*timeOut > 35)
                {
                    *flag_disable = true;
                    return false;
                }
            }
            
            ret = true;
        }
        else
        {
            tO()->rst(OPTO_CHECK);
            *timeOut = 0;
        }
    }
    return ret;
}
#endif

/**
    @}
*/

/** @group skybus_OPTO_PROCESS
    @{
*/#ifndef skybus_OPTO_PROCESS
#define skybus_OPTO_PROCESS

/** @brief opto runClose
    @return true : opto press
            false : opto unpress
*/
bool skybus_opto_runClose(void)
{
    bool ret = false;
    bool status = skybus_button_get_status(IN3_GPIO_Port, IN3_Pin, &opto.OPTO3.disable, &opto.OPTO3.timeOut);
    
    if(status == true && opto.OPTO3.disable == false)
    {
        ret = true;
        opto.OPTO3.count++;
    }
    
    return ret;
}
/** @brief onr button opto CBQ
    @return true : opto press
            false : opto unpress
*/
bool skybus_opto_one_button_CBQ(void)
{
    bool ret = false;
    bool status = skybus_button_get_status_cbq(IN3_GPIO_Port, IN3_Pin, &opto.OPTO3.disable, &opto.OPTO3.timeOut);
    
    if(status == true && opto.OPTO3.disable == false)
    {
        ret = true;
        opto.CBQ_cnt++;
    }
    
    return ret;
}

/** @brief opto runOpen
    @return true : opto press
            false : opto unpress
*/
bool skybus_opto_runOpen(void)
{
    bool ret = false;
    bool status = skybus_button_get_status(IN4_GPIO_Port, IN4_Pin, &opto.OPTO4.disable, &opto.OPTO4.timeOut);
    
    if(status == true && opto.OPTO4.disable == false)
    {
        ret = true;
        opto.OPTO4.count++;
    }
    
    return ret;
}

/** @brief opto enable system
    @return true : opto press
            false : opto unpress
*/
bool skybus_opto_enableSystem(void)
{
    bool ret = false;
    bool status = skybus_button_get_status(IN1_GPIO_Port, IN1_Pin, &opto.OPTO1.disable, &opto.OPTO1.timeOut);
    
    if(status == true && opto.OPTO1.disable == false)
    {
        ret = true;
        opto.OPTO1.count++;
    }
    
    return ret;
}

/** @brief opto disable system
    @return true : opto press
            false : opto unpress
*/
bool skybus_opto_disableSystem(void)
{
    bool ret = false;
    bool status = skybus_button_get_status(IN2_GPIO_Port, IN2_Pin, &opto.OPTO2.disable, &opto.OPTO2.timeOut);
    
    if(status == true && opto.OPTO2.disable == false)
    {
        ret = true;
        opto.OPTO2.count++;
    }
    
    return ret;
}
/** @brief one button opto direct
    @return true : opto press
            false : opto unpress
*/
bool skybus_opto_one_button_direct(void)
{
    uint8_t ret = false;
    
    bool status = skybus_button_get_status(IN4_GPIO_Port, IN4_Pin, &opto.OPTO4.disable, &opto.OPTO4.timeOut);
    
    if(status == true && opto.OPTO4.disable == false)
    {
        if(opto.opto_button_delay == false)
        {
            
            ret = true;
            
            /// khi motor dung bang CBQ thi lan nhan nut tiep theo se chay vao
            if(sys_process->reason_motorStop == OVER_CBQ)
            {
                opto.OPTO4.direct = 1;
            }
            else
            {
                if(sys_process->first_runMotor_opto == true)
                {
                    /// chay xong lan dau moi duoc dem Count RUNOPEN va RUNCLOSE
                    opto.OPTO4.direct++;
                }
            }
            
            /// set bien cho phep delay opto button
            opto.opto_button_delay = true;
            
            if(opto.OPTO4.direct > 2)
            {
                opto.OPTO4.direct = 1;
            }
        }
    }
    
    /// bat dau tinh thoi gian delay opto button
    if(opto.opto_button_delay == true)
    {
        if(tO()->get(500, TIME_OUT_DELAY_OPTO_BUTTON))
        {
            opto.opto_button_delay = false;
        }
    }
    
    return ret;
}

/** @brief opto process
    @return none
*/
void skybus_opto_process(void)
{
    skybus_opto_runClose();
    skybus_opto_runOpen();
    
    skybus_opto_enableSystem();
    skybus_opto_disableSystem();
}
#endif
/**
    @}
*/

/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

