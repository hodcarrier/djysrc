/**
  ******************************************************************************
  * @file    usbh_hid_mouse.h 
  * @author  MCD Application Team
  * @version V3.2.2
  * @date    07-July-2015
  * @brief   This file contains all the prototypes for the usbh_hid_mouse.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef _USBH_HID_TOUCH_H_
#define _USBH_HID_TOUCH_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_HID_CLASS
  * @{
  */

/** @defgroup USBH_HID_MOUSE
  * @brief This file is the Header file for usbh_hid_mouse.c
  * @{
  */ 


/** @defgroup USBH_HID_MOUSE_Exported_Types
  * @{
  */ 

typedef struct _HID_TouchInfo
{
  uint16_t              wX;
  uint16_t              wY;
  uint8_t               bButton;
}
tagHID_TouchInfo;

/**
  * @}
  */ 

/** @defgroup USBH_HID_MOUSE_Exported_Defines
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_HID_MOUSE_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_HID_MOUSE_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_HID_MOUSE_Exported_FunctionsPrototype
  * @{
  */ 
USBH_StatusTypeDef USBH_HID_TouchInit(USBH_HandleTypeDef *phost);
USBH_StatusTypeDef USBH_HID_TouchDeInit(USBH_HandleTypeDef *pHost);

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* _USBH_HID_TOUCH_H_ */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
