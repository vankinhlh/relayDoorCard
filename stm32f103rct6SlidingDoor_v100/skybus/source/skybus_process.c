
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
#include "skybus_process.h"
#include "skybus_remote.h"
#include "skybus_relayMotor.h"
#include "skybus_timeOut.h"
#include "skybus_eeprom.h"
#include "skybus_setting.h"
#include "skybus_opto.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static skybus_process_t* sys_process;
extern skybus_relayMotor_t * motor;
extern __IO uint16_t adc_value[1];
extern skybus_opto_t   opto; 

//debug_memory_t debugMemory;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @group skybus_SYS_TIMER_CONTROL
    @{
*/#ifndef skybus_SYS_TIMER_CONTROL
#define skybus_SYS_TIMER_CONTROL


/** @brief ham control motor via remote One Button
    @return none
*/
void skybus_process_One_button_for_remote(void)
{
    /// nhan nut dao chieu dong co
    if((remote_()->apply[REMOTE_LOCATION_RUN_CLOSE] == true || remote_()->apply[REMOTE_LOCATION_RUN_OPEN] == true) && remote_()->apply[REMOTE_LOCATION_STOP] == false)
    {
        if(sys_process->remote_button_delay == false)
        {
            /// reset bien remote apply
            remote_()->apply[REMOTE_LOCATION_RUN_CLOSE] = false;
            remote_()->apply[REMOTE_LOCATION_RUN_OPEN] = false;
            /// set bien remote One button apply
            sys_process->remoteOneButton_apply = true;
            
            /// lan dau runMotor bang remote sau khi reset
            if(sys_process->first_runMotor_remote == false)
            {
                sys_process->first_runMotor_remote = true;
                sys_process->remoteOneButton_motorDirect = RELAY_MOTOR_RUN_OPEN;
            }
            else
            {
                /// khi motor dung bang CBQ thi lan nhan nut tiep theo se chay vao
                if(sys_process->reason_motorStop == OVER_CBQ)
                {
                    sys_process->remoteOneButton_motorDirect = RELAY_MOTOR_RUN_CLOSE;
                    
                    /// sau khi chay vao bang OVER CBQ thi reset reason de bam chay ra
                    sys_process->reason_motorStop = REASON_IDE;
                }
                else
                {
                    /// tinh toan chieu motor
                    sys_process->remoteOneButton_motorDirect ++;
                    
                    /// reset reason cho lan chay tiep theo
                    sys_process->reason_motorStop = REASON_IDE;
                }

                /// set bien cho phep delay remote button
                sys_process->remote_button_delay = true;
                
                if(sys_process->remoteOneButton_motorDirect > 2)
                {
                    sys_process->remoteOneButton_motorDirect = 1;
                }
            }
        }
    }
    
    /// nhan nut reset he thong
    if(remote_()->apply[REMOTE_LOCATION_VUONG] == true && remote_()->apply[REMOTE_LOCATION_STOP] == false)
    {
        /// set bien remote One button vuong
        sys_process->remoteOneButton_vuong = true;
    }
    
    /// nhan nut ngung motor
    if(remote_()->apply[REMOTE_LOCATION_STOP] == true)
    {
        sys_process->remoteOneButton_stop = true;
    }
    
    /// bat dau tinh thoi gian delay remote button
    if(sys_process->remote_button_delay == true)
    {
        if(tO()->get(500, TIME_OUT_DELAY_REMOTE_BUTTON))
        {
            sys_process->remote_button_delay = false;
        }
    }
}

/** @brief ham rorc motor
    @return none
*/
static void skybus_process_motorRORC(void)
{
    if(sys_process->remote_motor_runOpen == true || sys_process->opto_motor_runOpen == true)
    {
        /// reset bien motor rorc bang remote
        sys_process->remote_motor_runOpen = false;
        
        /// reset bien motor rorc bang opto
        sys_process->opto_motor_runOpen = false;
        
        /// set bien compare time rorc
        sys_process->rorc_time_comapre = true;
        
        /// neu time < 50 thi khong rorc
        if(eeprom_()->data[SETTING_TIME_RORC] > 50)
        {
            /// apply motor runClose
            motor->motorStates = motor->relayMotor_control(RELAY_MOTOR_RUN_CLOSE);
        }
    }

    /// compare time rorc
    if(sys_process->rorc_time_comapre == true)
    {
        if(tO()->get(eeprom_()->data[SETTING_TIME_RORC], TIME_OUT_RORC))
        {
            /// reset bien compare time rorc
            sys_process->rorc_time_comapre = false;
            
            /// set bien rorc 
            sys_process->motor_rorc = true;
        }
    }
    
    /// apply motor runOpen
    if(sys_process->motor_rorc == true)
    {
        /// reset bien motor rorc cho lan chay ra tiep theo
        sys_process->motor_rorc = false;
        
        motor->motorStates = motor->relayMotor_control(RELAY_MOTOR_RUN_OPEN);
        
        /// gan thoi gian runMotor display oled
        sys_process->time_runMotor = eeprom_()->data[TIME_STOP_DEFAULF];
        
        /// onCompare current
        sys_process->onCompare = true;
        
        /// reset flag runMotor done
        sys_process->runDone = false;
        
        /// reset time onCompare current
        tO()->rst(TIME_ON_COMPARE_CURRENT);
        
    }

}

/** @brief ham control motor total via remote and opto
    @return relayMotor_states
*/
relayMotor_states skybus_process_remoteControlMotor(void)
{
    
  #if (USE_ONE_BUTTON == 1)
    
    bool value_opto1 = false;
    bool value_opto2 = false;
    
    /// kiem tra nut co nhan
    if(skybus_opto_one_button_direct() == true)
    {
        /// lan dau runMotor bang opto sau khi reset
        if(sys_process->first_runMotor_opto == false)
        {
            sys_process->first_runMotor_opto = true;
            value_opto2 = true;
        }
        else
        {
            /// ktra gia tri nut nhan
            if(opto.OPTO4.direct == 1)
            {
                value_opto1 = true;
            }
            else
            {
                value_opto2 = true;
            }
        }
    }
    
    skybus_process_One_button_for_remote();

  #else
    bool value_opto1 = skybus_opto_runClose();
    bool value_opto2 = skybus_opto_runOpen();
  #endif
/////////////////////////////////////////////////////////////////////////////////////////    
    #if (USE_ONE_BUTTON == 1)
            if(sys_process->remoteOneButton_motorDirect == RELAY_MOTOR_RUN_CLOSE && 
               sys_process->remoteOneButton_apply == true
            )
            {
                /// reset bien remote One button apply
                sys_process->remoteOneButton_apply = false;
    #else
        #if (USE_APPLY_LOCATOIN_REMOTE == 1)
            if(remote_()->apply[REMOTE_LOCATION_RUN_CLOSE] == true && remote_()->apply[REMOTE_LOCATION_STOP] == false)
            //        remote_()->apply[REMOTE_D0] = false;
            {
        #else
            if(remote_()->D0() == true)
            {
        #endif
    #endif
                /// reset time wait run Motor
                tO()->rst(TIME_MOTOR_RUN_OPEN);
                tO()->rst(TIME_MOTOR_RUN_CLOSE);
                    
                motor->motorStates = motor->relayMotor_control(RELAY_MOTOR_RUN_CLOSE);
                
                /// gan thoi gian runMotor display oled
                sys_process->time_runMotor = eeprom_()->data[TIME_STOP_DEFAULF] + 5000;
                
                /// onCompare current
                sys_process->onCompare = true;
                
                /// reset flag runMotor done
                sys_process->runDone = false;
                
                /// reset time onCompare current
                tO()->rst(TIME_ON_COMPARE_CURRENT);
            }
    #if (USE_ONE_BUTTON == 1)
            if(sys_process->remoteOneButton_motorDirect == RELAY_MOTOR_RUN_OPEN && 
               sys_process->remoteOneButton_apply == true
            )
            {
                /// reset bien remote One button apply
                sys_process->remoteOneButton_apply = false;
    #else
        #if (USE_APPLY_LOCATOIN_REMOTE == 1)
            if(remote_()->apply[REMOTE_LOCATION_RUN_OPEN] == true && remote_()->apply[REMOTE_LOCATION_STOP] == false)
            //        remote_()->apply[REMOTE_D3] = false;
            {
        #else
            if(remote_()->D3() == true)
            {
        #endif
    #endif
                /// reset time wait run Motor
                tO()->rst(TIME_MOTOR_RUN_OPEN);
                tO()->rst(TIME_MOTOR_RUN_CLOSE);
                
                /// set bien co tin hieu remote motor runOpen
                sys_process->remote_motor_runOpen = true;
            }
    #if (USE_ONE_BUTTON == 1)
            if(sys_process->remoteOneButton_stop == true)
            {
                /// reset bien remote One button stop
                sys_process->remoteOneButton_stop = false;
    #else
        #if (USE_APPLY_LOCATOIN_REMOTE == 1)
            if(remote_()->apply[REMOTE_LOCATION_STOP] == true)
            //        remote_()->apply[REMOTE_D2] = false;
            {
        #else
            if(remote_()->D2() == true)
            {
        #endif
    #endif
                /// set flag runMotor done
                sys_process->runDone = true;
                
                /// set reason motor Stop
                sys_process->reason_motorStop = BUTTON_REMOTE_STOP;
                
                /// stopMotor and resetVariables
                sys_process->stopMotor_resetVariables();
            }
    #if (USE_ONE_BUTTON == 1)
            if(sys_process->remoteOneButton_vuong == true)
            {
                /// reset bien remote One button vuong
                sys_process->remoteOneButton_vuong = false;
    #else
        #if (USE_APPLY_LOCATOIN_REMOTE == 1)
            if(remote_()->apply[REMOTE_LOCATION_VUONG] == true && remote_()->apply[REMOTE_LOCATION_STOP] == false)
            //        remote_()->apply[REMOTE_D1] = false;
        {
        #else
            if(remote_()->D1() == true)
            {
        #endif
    #endif
                NVIC_SystemReset();
            }
///////////////////////////////////////////////////////////////////////////////////////
    if(value_opto1 == true)
    {
        
      #if (USE_ONE_BUTTON == 1)
        
        value_opto1 = false;
        
      #endif
        
        /// reset time wait run Motor
        tO()->rst(TIME_MOTOR_RUN_OPEN);
        tO()->rst(TIME_MOTOR_RUN_CLOSE);
            
        motor->motorStates = motor->relayMotor_control(RELAY_MOTOR_RUN_CLOSE);

        /// gan thoi gian runMotor display oled
        sys_process->time_runMotor = eeprom_()->data[TIME_STOP_DEFAULF] + 5000;

        /// onCompare current
        sys_process->onCompare = true;

        /// reset flag runMotor done
        sys_process->runDone = false;

        /// reset time onCompare current
        tO()->rst(TIME_ON_COMPARE_CURRENT);
    }
    
    if(value_opto2 == true)
    {
        
      #if (USE_ONE_BUTTON == 1)
        
        value_opto2 = false;
        
      #endif
        
        /// reset time wait run Motor
        tO()->rst(TIME_MOTOR_RUN_OPEN);
        tO()->rst(TIME_MOTOR_RUN_CLOSE);
        
        /// set bien motor runOpen bang opto
        sys_process->opto_motor_runOpen = true;
        
    }

    return motor->motorStates;
}

/** @brief ham control system via opto
    @return relayMotor_states
*/
void skybus_process_optoControlSystem(void)
{

    bool value_opto3 = skybus_opto_enableSystem();
    bool value_opto4 = skybus_opto_disableSystem();
    
    if(value_opto3 == true)
    {
        sys_process->enable = true;
        NVIC_SystemReset();
    }
    
    if(value_opto4 == true)
    {
        sys_process->enable = false;
    }
}

/** @brief ham CBQ process
    @return none
*/
void skybus_process_CBQ_process(void)
{
    bool status = skybus_opto_one_button_CBQ();
    
    if(status == true)
    {
        /// enable CBQ
        sys_process->CBQ_enable = true;
        
        /// onCompare current
        sys_process->onCompare = true;
    }
    
    if(sys_process->CBQ_enable == true)
    {
        
        /// reset bien enable CBQ
        sys_process->CBQ_enable = false;
        
        /// ham CBQ process
        if(motor->motorStates == RELAY_MOTOR_RUN_CLOSE)
        {
            /// set apply CBQ
            sys_process->CBQ_apply = true;
            
            /// gan thoi gian runMotor khi cham CBQ
            sys_process->time_runMotor = 2000;
        }
    }
    
    /// apply CBQ
    if(sys_process->CBQ_apply == true)
    {
        if(tO()->get(2000, TIME_OUT_CBQ))
        {
            /// set flag runMotor done
            sys_process->runDone = true;
            
            if(sys_process->stopMotor_resetVariables() == true)
            {
                sys_process->reason_motorStop = OVER_CBQ;
                
            }
        }
        else
        {
            motor->motorStates = motor->relayMotor_control(RELAY_MOTOR_RUN_OPEN);
        }
    }
}

/** @brief ham stop Motor, reset tat ca cac bien
    @return true : da stop Motor, reset bien
            false : dang process
*/
bool skybus_process_stopMotor_resetVariables(void)
{
    bool ret = false;

    motor->motorStates = motor->relayMotor_control(RELAY_MOTOR_RUN_STOP);
    
    /// reset time wait run Motor
    tO()->rst(TIME_MOTOR_RUN_CLOSE);
    tO()->rst(TIME_MOTOR_RUN_OPEN);
    
    /// reset reason motorStop
//    sys_process->reason_motorStop = REASON_IDE;
    
    /// reset flag onCompare current
    sys_process->onCompare = false;
    
    /// reset time onCompare current
    tO()->rst(TIME_ON_COMPARE_CURRENT);
    
    /// reset flag applyCompare current
    sys_process->applyCompare = false;
    
    /// reset flag reset system
    sys_process->flag_reset = false;
    
    /// reset bien remote One button Direct
    if(sys_process->reason_motorStop == TIME_STOP_RUN_OPEN)
    {
        sys_process->remoteOneButton_motorDirect = RELAY_MOTOR_RUN_STOP;
    }
    else if(sys_process->reason_motorStop == TIME_STOP_RUN_CLOSE)
    {
        sys_process->remoteOneButton_motorDirect = RELAY_MOTOR_RUN_CLOSE;
    }
    else if(sys_process->reason_motorStop == TIME_STOP_RUN_OPEN)
    {
        sys_process->remoteOneButton_motorDirect = RELAY_MOTOR_RUN_STOP;
    }
    else if(sys_process->reason_motorStop == CURRENT_STOP_RUN_CLOSE_OVER)
    {
        sys_process->remoteOneButton_motorDirect = RELAY_MOTOR_RUN_STOP;
    }
    else if(sys_process->reason_motorStop == CURRENT_STOP_RUN_OPEN_OVER)
    {
        sys_process->remoteOneButton_motorDirect = RELAY_MOTOR_RUN_CLOSE;
    }
//    /// reset bien apply CBQ
    sys_process->CBQ_apply = false;
    
    /// kiem tra so lan chay vao de reset system
    if(sys_process->reset > 3)
    {
        if(sys_process->runDone == true)
        NVIC_SystemReset();
    }
    
    /// kiem tra motorStates va gia tri cac bien da reset chua
    if  (
        
        motor->motorStates == RELAY_MOTOR_RUN_STOP
        && sys_process->onCompare == false 
        && sys_process->flag_reset == false 
//        && sys_process->remoteOneButton_motorDirect == RELAY_MOTOR_RUN_STOP
        && sys_process->CBQ_apply == false
            
        )
    {
        
        ret = true;
        
    }
    return ret;
}

/** @brief ham kiem soat thoi gian chay motor
    @return skybus_sys_process_states
*/
skybus_sys_process_states skybus_process_timeStop(void)
{
    skybus_sys_process_states process_state;
    relayMotor_states motorStates = skybus_process_remoteControlMotor();
    
    if(motorStates != RELAY_MOTOR_RUN_STOP)
    {
        if(motorStates == RELAY_MOTOR_RUN_CLOSE)
        {
            /// tinh thoi gian motor runClose
            if(tO()->get(eeprom_()->data[TIME_STOP_DEFAULF] + 5000, TIME_MOTOR_RUN_CLOSE))
            {
                /// set flag runMotor done
                sys_process->runDone = true;
                
                if(sys_process->stopMotor_resetVariables() == true)
                {
                    process_state = STATES_STOP;
                    sys_process->reason_motorStop = TIME_STOP_RUN_CLOSE;
                }
            }
            else
            {
                /// set trang thai motor runClose
                process_state = STATES_RUN_CLOSE;
            }
        }
        else if(motorStates == RELAY_MOTOR_RUN_OPEN)
        {
            /// tinh thoi gian motor runOpen
            if(tO()->get((eeprom_()->data[TIME_STOP_DEFAULF]), TIME_MOTOR_RUN_OPEN))
            {
                /// set flag runMotor done
                sys_process->runDone = true;
                
                if(sys_process->stopMotor_resetVariables() == true)
                {
                    process_state = STATES_STOP;
                    sys_process->reason_motorStop = TIME_STOP_RUN_OPEN;
                }
            }
            else
            {
                /// set trang thai motor runOpen
                process_state = STATES_RUN_OPEN;
            }
        }
    }
    
    return process_state;
}
    
/** @brief ham distance
    @return none
*/
void skybus_process_distance(void)
{
     if(motor->motorStates == RELAY_MOTOR_RUN_OPEN)
     {
        sys_process->distance++;
        
        /// limit upper distance
        if(sys_process->distance > DISTANCE_MAX)
        {
            sys_process->distance = DISTANCE_MAX;
        }
     }
     else if(motor->motorStates == RELAY_MOTOR_RUN_CLOSE)
     {
        sys_process->distance--;
        
         // limit lower distance
         if(sys_process->distance < 0)
         {
            sys_process->distance = 0;
         }
     }
     
     if(sys_process->distance > eeprom_()->data[SETTING_DISTANCE])
     {
         /// stop MOTOR for distance
        if(sys_process->stopMotor_resetVariables() == true)
        {
            sys_process->reason_motorStop = DISTANCE;
            
            sys_process->distance -= 10;
        }
     }
}

/** @brief ham dem so lan chay vao de reset system
    @return none
*/
void skybus_process_reset_system(void)
{
    if(motor->motorStates == RELAY_MOTOR_RUN_CLOSE)
    {
        if(sys_process->flag_reset == false)
        {
            sys_process->flag_reset = true;
            sys_process->reset++;
        }
    }

}

/** @brief ham get Offset current
    @param[in] Count    : so lan get
    @param[in] ADCValue : gia tri raw
    @return gia Offset current
*/
uint16_t skybus_process_getOffsetCurrent(uint16_t Count, uint16_t ADCValue)
{
    uint16_t CountRun;
    uint32_t ValueADCSample, ValueADCMedium;
    for(CountRun = 0;CountRun < Count; CountRun++)
    {
        ValueADCSample = ValueADCSample + ADCValue;
        HAL_Delay(25);
        
        HAL_GPIO_TogglePin(ledRun_GPIO_Port, ledRun_Pin);
    }
    ValueADCMedium = ValueADCSample / Count;
    return ValueADCMedium;
//    uint16_t alphaFilter = 300;
//    uint16_t output = ((output * alphaFilter + ADCValue) / (1 + alphaFilter));
//    
//    return output;
}

/** @brief ham get gia tri thuc Offset Curent
    @return none
*/
void skybus_process_get_real_Offset(void)
{
    if(setting_()->currentOffset_eeprom == 0)
    {
        while(sys_process->Offset_retry == false)
        {
            sys_process->currentOffset = sys_process->getOffset(100, adc_value[0]);
            
            /// tinh toan gia tri current hien tai
            sys_process->currentPresent = adc_value[0] - sys_process->currentOffset;

            /// kiem tra gia tri hien tai current
            if(sys_process->currentPresent > 10)
            {
                /// reset bien retry get Offset current
                sys_process->Offset_retry = true;
                /// luu vao eeprom gia tri Offset current
                setting_()->currentOffset_eeprom = sys_process->currentOffset;
                eeprom_()->write(CURRENT_OFFSET, setting_()->currentOffset_eeprom);
            }
        }
    }
}

/** @brief ham kiem tra current motor
    @return none
    @bug : runClose over current off motor ngay khong dao chieu
*/
void skybus_process_currentStop_motor(uint16_t currentSet)
{
    skybus_sys_process_states process_states;
    
    sys_process->currentOffset = setting_()->currentOffset_eeprom;
    
    /// get current hien tai
    sys_process->currentPresent = adc_value[0] - sys_process->currentOffset;
    
    /// limit current
    if(sys_process->currentPresent < 0)
    {
        sys_process->currentPresent = 0;
    }
    
    /// get current set
    sys_process->currentSet = currentSet;
    
    /// cho phep compare current
    if(sys_process->onCompare == true)
    {
        if(tO()->get(eeprom_()->data[TIME_ON_COMPARE_CURRENT_SETTNG], TIME_ON_COMPARE_CURRENT))
        {
            /// reset onCompare current
            sys_process->onCompare = false;
            
            /// ap dung compare current
            sys_process->applyCompare = true;

        }
    }
    
    /// applyComapre current
    if(sys_process->applyCompare == true)
    {
        /// compare current hien voi current set
        if(sys_process->currentPresent > sys_process->currentSet)
        {
            if(motor->motorStates == RELAY_MOTOR_RUN_CLOSE)
            {
                if(sys_process->stopMotor_resetVariables() == true)
                {
                    process_states = STATES_STOP;
                    sys_process->reason_motorStop = CURRENT_STOP_RUN_CLOSE;
                }
            }
            else if(motor->motorStates == RELAY_MOTOR_RUN_OPEN)
            {
                if(sys_process->stopMotor_resetVariables() == true)
                {
                    process_states = STATES_STOP;
                    sys_process->reason_motorStop = CURRENT_STOP_RUN_OPEN;
                }
            }
        }
    }

    /// neu motorStop khi runClose thi runOpen 3000ms
    if(sys_process->reason_motorStop == CURRENT_STOP_RUN_CLOSE)
    {
        /// onCompare current
        sys_process->onCompare = true;
        
        if(tO()->get(eeprom_()->data[TIME_OVER_CURRENT_RUN_CLOSE], TIME_MOTOR_RUNCLOSE_OVER_CURRENT))
        {
            /// set flag runMotor done
            sys_process->runDone = true;
            
            if(sys_process->stopMotor_resetVariables() == true)
            {
                sys_process->reason_motorStop = CURRENT_STOP_RUN_OPEN_OVER;
            }
        }
        else
        {
            motor->motorStates = motor->relayMotor_control(RELAY_MOTOR_RUN_STOP);
        }
    }
    /// neu motorStop khi runOpen thi runClose 200ms
    else if(sys_process->reason_motorStop == CURRENT_STOP_RUN_OPEN)
    {
        /// onCompare current
        sys_process->onCompare = true;
        
        if(tO()->get(eeprom_()->data[TIME_OVER_CURRENT_RUN_OPEN], TIME_MOTOR_RUNOPEN_OVER_CURRENT))
        {
            if(sys_process->stopMotor_resetVariables() == true)
            {
                sys_process->reason_motorStop = CURRENT_STOP_RUN_CLOSE_OVER;
                
                /// set flag runMotor done
                sys_process->runDone = true;
            }
        }
        else
        {
            motor->motorStates = motor->relayMotor_control(RELAY_MOTOR_RUN_CLOSE);
        }
    }
    
}

#endif
/**
    @}
*/

/** @group skybus_SYS_APPLY_LOCATION
    @{
*/#ifndef skybus_SYS_APPLY_LOCATION
#define skybus_SYS_APPLY_LOCATION

/** @brief ham apply remote location button
    @return none
*/
void skybus_sys_process_apply_location_remote_button(void)
{
    
    /// goi ham enable setting location remote
    remote_()->enable_setting_Callback();
    
    if(
        remote_()->location[REMOTE_LOCATION_RUN_CLOSE] != LOCATION_NUMBER_IDLE
    &&  remote_()->location[REMOTE_LOCATION_RUN_OPEN]  != LOCATION_NUMBER_IDLE
    &&  remote_()->location[REMOTE_LOCATION_STOP]      != LOCATION_NUMBER_IDLE
    &&  remote_()->location[REMOTE_LOCATION_VUONG]     != LOCATION_NUMBER_IDLE
      )
    {
        /// apply location remote button setting from eeprom
        /// apply location remote button D0
        remote_()->apply_location_remote_Callback(
            remote_()->location[REMOTE_LOCATION_RUN_CLOSE],
            &remote_()->apply[REMOTE_LOCATION_RUN_CLOSE]
        );
        
        /// apply location remote button D1
        remote_()->apply_location_remote_Callback(
            remote_()->location[REMOTE_LOCATION_RUN_OPEN],
            &remote_()->apply[REMOTE_LOCATION_RUN_OPEN]
        );
        
        /// apply location remote button D2
        remote_()->apply_location_remote_Callback(
            remote_()->location[REMOTE_LOCATION_STOP],
            &remote_()->apply[REMOTE_LOCATION_STOP]
        );
        
        /// apply location remote button D3
        remote_()->apply_location_remote_Callback(
            remote_()->location[REMOTE_LOCATION_VUONG],
            &remote_()->apply[REMOTE_LOCATION_VUONG]
        );
    }
    else
    {
        /// apply location remote button default
        remote_()->apply[REMOTE_LOCATION_RUN_CLOSE] = remote_()->D0();
        remote_()->apply[REMOTE_LOCATION_VUONG]     = remote_()->D1();
        remote_()->apply[REMOTE_LOCATION_STOP]      = remote_()->D2();
        remote_()->apply[REMOTE_LOCATION_RUN_OPEN]  = remote_()->D3();
        
    }
}

#endif
/**
    @}
*/

/** @group skybus_SYSTEM_PROCESS
    @{
*/#ifndef skybus_SYSTEM_PROCESS
#define skybus_SYSTEM_PROCESS

/** @brief ham motor system process
    @return none
*/
void skybus_sys_process(void)
{
    if(setting_()->mode == MODE_IDE && remote_()->enable_setting == false)
    {
        skybus_process_timeStop();
        skybus_process_currentStop_motor(eeprom_()->data[CURRENT_DEFAULT]);
        skybus_process_reset_system();
        skybus_sys_process_apply_location_remote_button();
        skybus_process_motorRORC();
        
      #if (USE_ONE_BUTTON == 1)
        skybus_process_CBQ_process();
      #endif
        
        skybus_process_distance();
        
//        debugMemory.debug++;
//        debugMemory.debug1++;
//        debugMemory.debug2++;
//        debugMemory.debug3++;
//        debugMemory.debug4++;
//        debugMemory.debug5++;
//        debugMemory.debug6++;
    }
}

#endif
/**
    @}
*/
skybus_process_t* skybus_sys_process_init(void)
{
    /// cap vung nho cho thu vien
    sys_process = calloc(sizeof(skybus_process_t), 1);
    /// gan ham
    sys_process->system_process             = skybus_sys_process;
    sys_process->stopMotor_resetVariables   = skybus_process_stopMotor_resetVariables;
    sys_process->getOffset                  = skybus_process_getOffsetCurrent;
    sys_process->app_remote_process         = skybus_sys_process_apply_location_remote_button;
    sys_process->OffsetReal                 = skybus_process_get_real_Offset;
    
    /// enable system
    sys_process->enable = true;
    
    return sys_process;
}
skybus_process_t* sys_process_(void)
{
    return sys_process;
}
/************************ (C) COPYRIGHT SKYBUS *****END OF FILE****************/

