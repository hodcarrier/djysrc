/**
  ******************************************************************************
  * @file    usbh_hid_parser.c
  * @author  MCD Application Team
  * @version V3.2.2
  * @date    07-July-2015
  * @brief   This file is the HID Layer Handlers for USB Host HID class.
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
//#include "usbh_hid_parser.h"
#include "../inc/usbh_hid_parser.h"
#include "../inc/usbh_hid_report.h"


/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_HID_CLASS
  * @{
  */
  
/** @defgroup USBH_HID_PARSER 
  * @brief    This file includes HID parsers for USB Host HID class.
  * @{
  */ 

/** @defgroup USBH_HID_PARSER_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_HID_PARSER_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_HID_PARSER_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_HID_PARSER_Private_FunctionPrototypes
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBH_HID_PARSER_Private_Variables
  * @{
  */

/**
  * @}
  */ 


/** @defgroup USBH_HID_PARSER_Private_Functions
  * @{
  */ 

/**
  * @brief  HID_ReadItem 
  *         The function read a report item.
  * @param  ri: report item
  * @param  ndx: report index
  * @retval status (0 : fail / otherwise: item value)
  */
uint32_t HID_ReadItem(HID_Report_ItemTypedef *ri, uint8_t ndx)
{
    uint32_t val=0;
    uint32_t x=0;
    uint32_t bofs;
    uint8_t *data = ri->data; // item������
    uint8_t shift = ri->shift;
    uint32_t size;

    /* get the logical value of the item */

    /* if this is an array, wee may need to offset ri->data.*/
    if (ri->count > 0) // TODO: �����֪��ʲô��˼
    {
        /* If app tries to read outside of the array. */
        if (ri->count <= ndx)
        {
            return(0);
        }

        /* calculate bit offset */
        bofs = ndx * ri->size;
        bofs += shift;
        /* calculate byte offset + shift pair from bit offset. */
        data += bofs/8;
        shift = (uint8_t)(bofs%8);
    }

    /* read data bytes in little endian order */
    size = ((ri->size & 0x7) ? (ri->size/8)+1 : (ri->size/8)); // Բ����������bit��תbyte
    for(x = 0; x < size; x++)
    {
        val |= (uint32_t)(*(data+x) << (x*8)); // ��С�˵�����ת��Ϊ��ˣ�USB��������������С��ģʽ
    }

    val = (val >> shift) & ((1 << ri->size)-1); // ����ƫ��

    if(ri->mask)
    {
        val &= ri->mask; // ֻȡmask������
    }

    if (val < ri->logical_min || val > ri->logical_max)
    {
        return(0); //
    }

    /* convert logical value to physical value */
    /* See if the number is negative or not. */
    if ((ri->sign) && (val & (1 << (ri->size-1)))) // val�����λ��1,�������Ǵ����ŵ�
    {
        /* yes, so sign extend value to 32 bits. */
        int vs = (int)((-1 & ~((1 << (ri->size)) - 1)) | val);

        if(ri->resolution == 1)
        {
            return((uint32_t)vs);
        }

        return((uint32_t)(vs * ri->resolution));
    }
    else
    {
        if(ri->resolution == 1)
        {
            return(val);
        }

        return(val * ri->resolution);
    }
}

/**
  * @brief  HID_WriteItem 
  *         The function write a report item.
  * @param  ri: report item
  * @param  ndx: report index
  * @retval status (1: fail/ 0 : Ok)
  */
uint32_t HID_WriteItem(HID_Report_ItemTypedef *ri, uint32_t value, uint8_t ndx)
{
    uint32_t x;
    uint32_t mask;
    uint32_t bofs;
    uint8_t *data=ri->data;
    uint8_t shift=ri->shift;

    if (value < ri->physical_min || value > ri->physical_max)
    {
        return(1);
    }

    /* if this is an array, wee may need to offset ri->data.*/
    if (ri->count > 0)
    {
        /* If app tries to read outside of the array. */
        if (ri->count >= ndx)
        {
            return(0);
        }
        /* calculate bit offset */
        bofs = ndx*ri->size;
        bofs += shift;
        /* calculate byte offset + shift pair from bit offset. */
        data+=bofs/8;
        shift=(uint8_t)(bofs%8);

    }

    /* Convert physical value to logical value. */
    if (ri->resolution != 1)
    {
        value=value/ri->resolution;
    }

    /* Write logical value to report in little endian order. */
    mask=(uint32_t)((1<<ri->size)-1);
    value = (value & mask) << shift;

    for(x=0; x < ((ri->size & 0x7) ? (ri->size/8)+1 : (ri->size/8)); x++)
    {
        *(ri->data+x)=(uint8_t)((*(ri->data+x) & ~(mask>>(x*8))) | ((value>>(x*8)) & (mask>>(x*8))));
    }
    return(0);
}

/**
  * @}
  */ 
void HID_ReportParse(u8 *pReport, u16 dwLen)
{
    u16 i, j;
    u8 type;
    u8 size;
    u8 tag;

    printf("\r\n: info :  : touch report.");
    for(i = 0; i < dwLen; i++)
    {
        type = HID_ITEM_TYPE(pReport[i]);
        tag = HID_ITEM_TAG(pReport[i]);

        if(HID_ITEM_MAIN == type)
        {
            printf("\r\n            <MIAN  :");
            switch (tag)
            {
                case HID_MAIN_INPUT :             printf("INPUT>           "); break;
                case HID_MAIN_OUTPUT :            printf("OUTPUT>          "); break;
                case HID_MAIN_FEATURE :           printf("FEATURE>         "); break;
                case HID_MAIN_COLLECTION :        printf("COLLECTION>      "); break;
                case HID_MAIN_END_COLLECTION :    printf("END COLLECTION>  "); break;
                default :                         printf("UNKONWN>         "); break;
            }
        }
        else if(HID_ITEM_GLOBAL == type)
        {
            printf("\r\n           <GLOBAL:");
            switch (tag)
            {
                case HID_GLOBAL_USAGE_PAGE :      printf("USAGE PAGE>      "); break;
                case HID_GLOBAL_LOGICAL_MIN :     printf("LOGICAL MIN>     "); break;
                case HID_GLOBAL_LOGICAL_MAX :     printf("LOGICAL MAX>     "); break;
                case HID_GLOBAL_PHYSICAL_MIN :    printf("PHYSICAL MIN>    "); break;
                case HID_GLOBAL_PHYSICAL_MAX :    printf("PHYSICAL MAX>    "); break;
                case HID_GLOBAL_UNIT_EXPONENT :   printf("UNIT EXPONENT>   "); break;
                case HID_GLOBAL_UNIT :            printf("UNIT>            "); break;
                case HID_GLOBAL_REPORT_SIZE :     printf("REPORT SIZE>     "); break;
                case HID_GLOBAL_REPORT_ID :       printf("REPORT ID>       "); break;
                case HID_GLOBAL_REPORT_COUNT :    printf("REPORT COUNT>    "); break;
                case HID_GLOBAL_PUSH :            printf("PUSH>            "); break;
                case HID_GLOBAL_POP :             printf("POP>             "); break;
                default :                         printf("UNKONWN>         "); break;
            }
        }
        else if(HID_ITEM_LOCAL == type)
        {
            printf("\r\n             <LOCAL :");
            switch (tag)
            {
                case HID_LOCAL_USAGE :            printf("USAGE>           "); break;
                case HID_LOCAL_USAGE_MIN :        printf("USAGE_MIN>       "); break;
                case HID_LOCAL_USAGE_MAX :        printf("LOGICAL MAX>     "); break;
                case HID_LOCAL_DESIGNATOR_INDEX : printf("DESIGNATOR INDEX>"); break;
                case HID_LOCAL_DESIGNATOR_MIN :   printf("DESIGNATOR MIN>  "); break;
                case HID_LOCAL_DESIGNATOR_MAX :   printf("DESIGNATOR MAX>  "); break;
                case HID_LOCAL_STRING_INDEX :     printf("STRING INDEX>    "); break;
                case HID_LOCAL_STRING_MIN :       printf("STRING MIN>      "); break;
                case HID_LOCAL_STRING_MAX :       printf("STRING MAX>      "); break;
                case HID_LOCAL_DELIMITER :        printf("DELIMITER>       "); break;
                default :                         printf("UNKONWN>         "); break;
            }
        }
        else
        {
            printf("\r\n              <UNKOWN>");
            continue;
        }

        size = HID_ITEM_SIZE(pReport[i]);
        if(size)
        {
            printf("\r\n              <SIZE:%d>", size);
            printf("\r\n              <VAL:");
            for(j = 0; j < size; j++)
            {
                printf("%02x", pReport[++i]);
            }
            printf(">");
        }
        printf("\r\n");
    }

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