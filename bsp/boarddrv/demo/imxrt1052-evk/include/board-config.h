//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: BSPģ��
//����:  chj.
//�汾��V1.0.0
//�ļ�����: ������������ļ���
//        CPU�ͺ�:MIMXRT1052
//        ����ͺ�:EVK
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2018-05-04
//   ����:  chj
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CPU_MIMXRT1052DVL6B

#define BOARD_FLASH_SIZE    (0x4000000U)

#define Mhz         1000000U
#define CN_XTAL     (24*Mhz)
#define CN_RTC_XTAL (32768U)
#define CN_CFG_MCLK (600*Mhz)  //��Ƶ���ں�Ҫ�ã����붨��
#define CN_CFG_FCLK CN_CFG_MCLK  //cm7������������ʱ��

/*____���¶���tick����____*/
#define CN_CFG_TICK_US 1000  //tick�������usΪ��λ��
#define CN_CFG_TICK_HZ 1000  //�ں�ʱ��Ƶ�ʣ���λΪhz��
#define CN_USE_TICKLESS_MODE    (0U)
#if (!CN_USE_TICKLESS_MODE)
#define CN_CFG_FINE_US 0x000006D  //1/600M,tick����ʱ�����ڣ���uSΪ��λ��32λ������������С����ռ16λ����Ҳ������ticks�������65535uS
#define CN_CFG_FINE_HZ CN_CFG_MCLK  //tick����ʱ��Ƶ�ʣ���CN_CFG_FINE_US�ĵ���
#else
#define CN_CFG_USE_USERTIMER        (0U)//�Ƿ�ʹ��LPTIMER��Ϊϵͳʱ��
#define CN_CFG_TIME_BASE_HZ         CN_CFG_MCLK//(32000U)//(8000U)
#if (!CN_CFG_USE_USERTIMER)
#define CN_CFG_USE_BYPASSTIMER      (0U)//0��ʾ��ʹ����·��ʱ����1��ʾʹ����·��ʱ��
#define CN_CFG_TIME_PRECISION       (500U)/*���ȵ�λ��US*/
#else
#define CN_CFG_USERTIMER_PRESC      (1U)//����ʹ��SYSTICK��ʱ������ָ���û���ʱ���ķ�Ƶ��
#define CN_CFG_FINE_US (0x1F4000U)  //1/32000,tick����ʱ�����ڣ���uSΪ��λ��32λ������������С����ռ16λ����Ҳ������ticks�������65535uS
#define CN_CFG_FINE_HZ (0x831U)  //tick����ʱ��Ƶ�ʣ���CN_CFG_FINE_US�ĵ���
#define CN_CFG_TIME_PRECISION       (200U)/*���ȵ�λ��US*/
#endif
#endif

//CPU�ܹ��������,��ѡ���߿��ܿ�ѡ�Ĳ�����������,�����С��,�ǿ�ѡ��,����������,
//��CPU�ֳ��̶�,���ֳ���BSP��arch.h�ļ��ж���
//�洢����С�˵�����
#define CN_CFG_LITTLE_ENDIAN        0
#define CN_CFG_BIG_ENDIAN           1
#define CN_CFG_BYTE_ORDER          CN_CFG_LITTLE_ENDIAN

#define BOARD_GMAC_PHY_ADDR        1
#define MAX_PIO_INTERRUPT_SOURCES  5

/*ע�⣺����Ĵ�С���˵Ļ������ļ������sdram_sizeҪ����Ӧ�ĸĶ�*/
#define CN_SDRAM_SIZE               (32U)           /*��λ��MB*/

#define CN_CAN_NUM                    1

#define DJY_BOARD       "EVK"

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif


