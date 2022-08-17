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
//����ģ��: CPU���趨��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ����CPU��������Ķ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu peripheral register definition"//CPU�������ݽṹ����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_PERIPHERAL_REGISTER_DEFINITION == false )
//#warning  " cpu_peripheral_register_definition  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_PERIPHERAL_REGISTER_DEFINITION    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
//dma��ؼĴ���
struct DMA_StReg volatile * const pg_dma1_reg
                        = (struct DMA_StReg *)0x40020000;
struct DMA_StReg volatile * const pg_dma2_reg
                        = (struct DMA_StReg *)0x40020400;

//dma1�ĸ�ͨ���Ĵ���
struct DMA_StChannelReg volatile * const pg_dma1_channel1_reg
                        = (struct DMA_StChannelReg *)0x40020008;
struct DMA_StChannelReg volatile * const pg_dma1_channel2_reg
                        = (struct DMA_StChannelReg *)0x4002001c;
struct DMA_StChannelReg volatile * const pg_dma1_channel3_reg
                        = (struct DMA_StChannelReg *)0x40020030;
struct DMA_StChannelReg volatile * const pg_dma1_channel4_reg
                        = (struct DMA_StChannelReg *)0x40020044;
struct DMA_StChannelReg volatile * const pg_dma1_channel5_reg
                        = (struct DMA_StChannelReg *)0x40020058;
struct DMA_StChannelReg volatile * const pg_dma1_channel6_reg
                        = (struct DMA_StChannelReg *)0x4002006c;
struct DMA_StChannelReg volatile * const pg_dma1_channel7_reg
                        = (struct DMA_StChannelReg *)0x40020080;
//dma2�ĸ�ͨ���Ĵ���
struct DMA_StChannelReg volatile * const pg_dma2_channel1_reg
                        = (struct DMA_StChannelReg *)0x40020408;
struct DMA_StChannelReg volatile * const pg_dma2_channel2_reg
                        = (struct DMA_StChannelReg *)0x4002041c;
struct DMA_StChannelReg volatile * const pg_dma2_channel3_reg
                        = (struct DMA_StChannelReg *)0x40020430;
struct DMA_StChannelReg volatile * const pg_dma2_channel4_reg
                        = (struct DMA_StChannelReg *)0x40020444;
struct DMA_StChannelReg volatile * const pg_dma2_channel5_reg
                        = (struct DMA_StChannelReg *)0x40020458;

//gpio��ؼĴ�������
struct GPIO_StReg volatile * const pg_gpio_rega
                        = (struct GPIO_StReg *)0x40010800;
struct GPIO_StReg volatile * const pg_gpio_regb
                        = (struct GPIO_StReg *)0x40010c00;
struct GPIO_StReg volatile * const pg_gpio_regc
                        = (struct GPIO_StReg *)0x40011000;
struct GPIO_StReg volatile * const pg_gpio_regd
                        = (struct GPIO_StReg *)0x40011400;
struct GPIO_StReg volatile * const pg_gpio_rege
                        = (struct GPIO_StReg *)0x40011800;
struct GPIO_StReg volatile * const pg_gpio_regf
                        = (struct GPIO_StReg *)0x40011c00;
struct GPIO_StReg volatile * const pg_gpio_regg
                        = (struct GPIO_StReg *)0x40012000;

struct AFIO_StReg volatile * const pg_afio_reg
                        = (struct AFIO_StReg *)0x40010000;
struct GPIO_StReg volatile * const pg_gpio_reg [7] = {
                                (struct GPIO_StReg *)0x40010800,
                                (struct GPIO_StReg *)0x40010c00,
                                (struct GPIO_StReg *)0x40011000,
                                (struct GPIO_StReg *)0x40011400,
                                (struct GPIO_StReg *)0x40011800,
                                (struct GPIO_StReg *)0x40011c00,
                                (struct GPIO_StReg *)0x40012000,
                               };

//������ؼĴ���
struct st_usart_reg volatile * const pg_uart1_reg
                        = (struct st_usart_reg *)0x40013800;
struct st_usart_reg volatile * const pg_uart2_reg
                        = (struct st_usart_reg *)0x40004400;
struct st_usart_reg volatile * const pg_uart3_reg
                        = (struct st_usart_reg *)0x40004800;
struct st_usart_reg volatile * const pg_uart4_reg
                        = (struct st_usart_reg *)0x40004c00;
struct st_usart_reg volatile * const pg_uart5_reg
                        = (struct st_usart_reg *)0x40005000;

//rcc(��λ��ʱ�ӿ��ƼĴ���)
struct st_rcc_reg volatile * const pg_rcc_reg  = (struct st_rcc_reg *)0x40021000;

//Ƭ��flash���ƼĴ���
struct st_inflash_fpec_reg volatile * const pg_inflash_fpec_reg
                                = (struct st_inflash_fpec_reg *)0x40022000;
struct st_inflash_ob_reg volatile * const pg_inflash_ob_reg
                                = (struct st_inflash_ob_reg *)0x1FFFF800;
