/** 
  ******************************************************************************
  * @file    button.h
  * @author  Gremsy Team
  * @version v1.0.0
  * @date    2021
  * @brief   This file contains all the functions prototypes for the  
  *          firmware library.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2011 Gremsy. All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __BUTTON_H
#define __BUTTON_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported define ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/// dinh nghia kieu so dem la ma
typedef enum _tuple_t
{
    EMPTY = 0,
    SINGLE = 1,
    DOUBLE = 2,
    TRIPLE = 3,
    QUAD   = 4,
    QUIN   = 5,
    HEXA   = 6,
    SEP    = 7,
    OCT    = 8,
    NON    = 9,
    DEC    = 10,
    HOLD   = 11,
}tuple_t;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/** @brief  buttonConfiguration
    @return none
*/
void buttonConfiguration(void);

/** @brief		buttonGetCount
 *  param[in]	none
	@return		tuple_t
*/
tuple_t buttonGetCount(void);

/** @brief		buttonPressCallback
 *  param[in]	buttonCount : so lan nut nhan can kiem tra
	@return		none
*/
__weak void buttonPressCallback(tuple_t buttonCount);

/** @brief		buttonProcess
 *  param[in]	none
	@return		none
*/
void buttonProcess(void);

#endif /* __BUTTON_H */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/


