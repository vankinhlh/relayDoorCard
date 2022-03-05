/**
  ******************************************************************************
  * @file    button.c
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
#include "button.h"
#include "time.h"
/* Private typedef------------------------------------------------------------------------------*/

/// dinh nghia kiem bien trang thai xu ly cho button
typedef enum _buttonProcessState_t
{
    BUTTON_PROCESS_STATE_IDLE,
    BUTTON_PROCESS_STATE_WAIT,
    BUTTON_PROCESS_STATE_CHECK,
    BUTTON_PROCESS_STATE_HOLD,
    BUTTON_PROCESS_STATE_HOLD_WAIT,
    BUTTON_PROCESS_STATE_NUM,
    BUTTON_PROCESS_STATE_DONE,
    BUTTON_PROCESS_STATE_ERROR,
}buttonProcessState_t;

/// struct chua cac bien chay an cua thu vien button
typedef struct _buttonPrivate_t
{
    /// trang thai hoat dong cua button
	buttonProcessState_t state;

    /// bien luu thoi gian xu ly led
    uint32_t time_loop;

    /// flag press
    bool f_press;

    /// count press
    uint16_t count;

    /// thoi gian nhan
    uint32_t time_press;

    /// thoi gian khong nhan
    uint32_t time_release;

    /// muc hoat dong
    bool active_level;

    uint32_t time_hold;
    uint32_t time_rel;
    uint32_t time_noise;
}buttonPrivate_t;
/* Private define------------------------------------------------------------------------------*/
/* Private macro------------------------------------------------------------------------------*/
/* Private variables------------------------------------------------------------------------------*/
buttonPrivate_t *buttonPri;
/* Private function prototypes------------------------------------------------------------------------------*/
/* Private functions------------------------------------------------------------------------------*/

/** @group __BUTTON_CONFIGURATION
    @{
*/#ifndef __BUTTON_CONFIGURATION
#define __BUTTON_CONFIGURATION
/** @brief  buttonConfiguration
    @return none
*/
void buttonConfiguration(void)
{
	/// cap vung nho cho thu vien
	buttonPri = calloc(1, sizeof(buttonPrivate_t));

	buttonPri->time_hold 	= 500;
	buttonPri->time_rel 	= 500;
	buttonPri->time_noise 	= 50;
}

/** @brief		buttonPressCallback
 *  param[in]	buttonCount : so lan nut nhan can kiem tra
	@return		none
*/
__weak void buttonPressCallback(tuple_t buttonCount)
{

}

#endif
/**
    @}
*/

/** @group __BUTTON_FUNCTION
	@{
*/#ifndef __BUTTON_FUNCTION
#define __BUTTON_FUNCTION
/** @brief		buttonGet
 *  param[in]	none
	@return		GPIO_PinState
*/
static GPIO_PinState buttonGet(void)
{
	return (HAL_GPIO_ReadPin(button_GPIO_Port, button_Pin));
}

/** @brief		buttonGetCount
 *  param[in]	none
	@return		tuple_t
*/
tuple_t buttonGetCount(void)
{
	return buttonPri->count;
}

/** @brief		buttonTuplesGet
 *  param[in]	button : pointer address of struct button
	@return		tuple_t
*/
static tuple_t buttonTuplesGet(buttonPrivate_t *button)
{
	uint32_t timeHold 		= button->time_hold;
	uint32_t timeRelease 	= button->time_rel;

	tuple_t ret = EMPTY;

	if(button->state == BUTTON_PROCESS_STATE_IDLE)
	{
		/// default button param
		button->count = 0;
		button->f_press = false;


		/// reset time button press
		timeReset(&button->time_press);

		/// reset time button unPress
		timeReset(&button->time_release);

		/// next state
		button->state = BUTTON_PROCESS_STATE_WAIT;

	}
	else if(button->state == BUTTON_PROCESS_STATE_WAIT)
	{
		/// check button press
		if(buttonGet() == GPIO_PIN_RESET)
		{
			button->f_press = true;
		}
		else
		{
			/// check button before have press ? count up variables button count
			if(button->f_press == true)
			{
				button->f_press = false;

				/// count up variables button count
				button->count = 1; /// ??????????????????????

				/// reset time button press
				timeReset(&button->time_press);

				/// reset time button unPress
				timeReset(&button->time_release);

				/// next state
				button->state = BUTTON_PROCESS_STATE_CHECK;
			}
		}

		/// in case button press check button press ?
		if(button->f_press == true)
		{
			/// check button press ?
			if(timeGetMs(&button->time_press) > timeHold)
			{
				/// next state
				button->state = BUTTON_PROCESS_STATE_HOLD;
			}
		}
		else
		{
			/// reset time button press
			timeReset(&button->time_press);
		}
	}
	else if(button->state == BUTTON_PROCESS_STATE_CHECK)
	{
		/// check button press ?
		if(buttonGet() == GPIO_PIN_RESET)
		{
			button->f_press = true;
		}
		else
		{
			/// in case button press check button press ?
			if(button->f_press == true)
			{
				button->f_press = false;

				/// count up button variables count
				button->count += 1;
			}
		}

		/// check button Error where press any time
		if(button->count > DEC)
		{
			button->state = BUTTON_PROCESS_STATE_ERROR;
		}

		/// kiem tra nut nhan con giu hay khong
		if(button->f_press == true)
		{
			/// check time button hold
			if(timeGetMs(&button->time_press) > timeHold)
			{
				/// next state
				button->state = BUTTON_PROCESS_STATE_HOLD;
			}


			/// rest time button release
			timeReset(&button->time_release);
		}
		else
		{
			/// kiem tra nut nhan co duoc giu hay khong
			if(timeGetMs(&button->time_release) > timeRelease)
			{
				/// next state
				button->state = BUTTON_PROCESS_STATE_NUM;
			}

			/// reset time button press
			timeReset(&button->time_press);
		}
	}
	else if(button->state == BUTTON_PROCESS_STATE_HOLD)
	{
		ret = HOLD;
		button->count = HOLD;

		/// button callback function
		/// ?????????????????????????
		buttonPressCallback(button->count);

		/// next state
		button->state = BUTTON_PROCESS_STATE_HOLD_WAIT;
	}
	else if(button->state == BUTTON_PROCESS_STATE_HOLD_WAIT)
	{
		/// check button to exit state HOLD
		if(buttonGet() == GPIO_PIN_SET)
		{
			/// next state
			button->state = BUTTON_PROCESS_STATE_DONE;
		}
	}
	else if(button->state == BUTTON_PROCESS_STATE_NUM)
	{
		/// get button count
		ret = (tuple_t)button->count;

		/// button callback function
		/// ?????????????????????????
		buttonPressCallback(button->count);

		/// next state
		button->state = BUTTON_PROCESS_STATE_DONE;
	}
	else if(button->state == BUTTON_PROCESS_STATE_DONE)
	{
		/// next state
		button->state = BUTTON_PROCESS_STATE_IDLE;
	}
	else if(button->state == BUTTON_PROCESS_STATE_ERROR)
	{
		/// next state
		button->state = BUTTON_PROCESS_STATE_IDLE;
	}

	return ret;
}

#endif
/**
	@}
*/

/** @group __BUTTON_PROCESS
	@{
*/#ifndef __BUTTON_PROCESS
#define __BUTTON_PROCESS
/** @brief		buttonPressProcess
 *  param[in]	button : pointer address of struct button
	@return		none
*/
void buttonPressProcess(buttonPrivate_t *button)
{
	uint32_t timeNoise = button->time_noise;

	/// check time process button
//	if(timeGetLoopMs(&button->time_loop) >= timeNoise)
	if(timeGetMs(&button->time_loop) >= timeNoise)
	{
		timeReset(&button->time_loop);
		buttonTuplesGet(button);
	}
}

/** @brief		buttonProcess
 *  param[in]	none
	@return		none
*/
void buttonProcess(void)
{
//	static uint32_t buttonCount = 0;
//	if(buttonGet() == GPIO_PIN_RESET)
//	{
//		buttonCount++;
//	}
	buttonPressProcess(buttonPri);
}

#endif
/**
	@}
*/

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

