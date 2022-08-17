/**
  ******************************************************************************
  * @file    usbh_pipes.c
  * @author  MCD Application Team
  * @version V3.2.2
  * @date    07-July-2015
  * @brief   This file implements functions for opening and closing Pipes
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

/* Includes ------------------------------------------------------------------*/
// #include "usbh_pipes.h"
#include "../inc/usbh_pipes.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_LIB_CORE
* @{
*/
  
/** @defgroup USBH_PIPES
  * @brief This file includes opening and closing Pipes
  * @{
  */ 

/** @defgroup USBH_PIPES_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_PIPES_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_PIPES_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_PIPES_Private_Variables
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBH_PIPES_Private_Functions
  * @{
  */ 
static uint16_t USBH_GetFreePipe (USBH_HandleTypeDef *phost);
extern s32 USBH_DeviceFromHub(USBH_HandleTypeDef *pHost);


/**
  * @brief  USBH_Open_Pipe
  *         Open a  pipe
  * @param  phost: Host Handle
  * @param  pipe_num: Pipe Number
  * @param  dev_address: USB Device address allocated to attached device
  * @param  speed : USB device speed (Full/Low)
  * @param  ep_type: end point type (Bulk/int/ctl)
  * @param  mps: max pkt size
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_OpenPipe  (USBH_HandleTypeDef *phost,
                            uint8_t pipe_num,
                            uint8_t epnum,
                            uint8_t dev_address,
                            uint8_t speed,
                            uint8_t ep_type,
                            uint16_t mps)
{
    USBH_LL_OpenPipe(phost,
                     pipe_num,
                     epnum,
                     dev_address,
                     speed,
                     ep_type,
                     mps);
  
  return USBH_OK; 

}

/**
  * @brief  USBH_ClosePipe
  *         Close a  pipe
  * @param  phost: Host Handle
  * @param  pipe_num: Pipe Number
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_ClosePipe  (USBH_HandleTypeDef *phost,
                            uint8_t pipe_num)
{

  USBH_LL_ClosePipe(phost, pipe_num);
  
  return USBH_OK; 

}

/**
  * @brief  USBH_Alloc_Pipe
  *         Allocate a new Pipe
  * @param  phost: Host Handle
  * @param  ep_addr: End point for which the Pipe to be allocated
  * @retval Pipe number
  */
uint8_t USBH_AllocPipe  (USBH_HandleTypeDef *phost, uint8_t ep_addr)
{
    uint16_t pipe;
    uint32_t *pipes;
  
    pipe =  USBH_GetFreePipe(phost);

    if(USBH_DeviceFromHub(phost))
        pipes = (uint32_t *)phost->pPipes;
    else
        pipes = phost->Pipes;

    if (pipe != 0xFFFF)
    {
        pipes[pipe] = 0x8000 | ep_addr;
    }
    return pipe;
}

/**
  * @brief  USBH_Free_Pipe
  *         Free the USB Pipe
  * @param  phost: Host Handle
  * @param  idx: Pipe number to be freed 
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_FreePipe  (USBH_HandleTypeDef *phost, uint8_t idx)
{
    uint32_t *pipes;

    if(USBH_DeviceFromHub(phost))
        pipes = (uint32_t *)phost->pPipes;
    else
        pipes = phost->Pipes;

    if(idx < 11)
    {
        pipes[idx] &= 0x7FFF;
    }
    return USBH_OK;
}

/**
  * @brief  USBH_GetFreePipe
  * @param  phost: Host Handle
  *         Get a free Pipe number for allocation to a device endpoint
  * @retval idx: Free Pipe number
  */
static uint16_t USBH_GetFreePipe (USBH_HandleTypeDef *phost)
{
    uint8_t idx = 0;
    uint32_t *pipes;

    if(USBH_DeviceFromHub(phost))
        pipes = (uint32_t *)phost->pPipes;
    else
        pipes = phost->Pipes;
  
    for (idx = 0 ; idx < 11 ; idx++)
    {
        if ((pipes[idx] & 0x8000) == 0)
        {
            return idx;
        }
    }

    printf("\r\n: warn : usbs%02x :  no free pipes\r\n", phost->id);
    return (0xFFFF);
}
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


