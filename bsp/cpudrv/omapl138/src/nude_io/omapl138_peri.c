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
//========================================================
// �ļ�ģ��˵��:
// omapl138���������Լ�������ƼĴ������塣
// �ļ��汾: v1.00
// ������Ա: lst
// ����ʱ��: 2011.06.01

//========================================================
// �����޸ļ�¼(���µķ�����ǰ��):
//  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
//========================================================
//���磺
// V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��
#include "cpu_peri.h"
tagGpioReg volatile * const g_ptGpioReg
                            = (tagGpioReg *)0x01e26000;

tagSysconfig0Reg volatile * const g_ptSysconfig0Reg
                      = (tagSysconfig0Reg *)0x01c14000;
tagSysconfig1Reg volatile * const g_ptSysconfig1Reg
                      = (tagSysconfig1Reg *)0x01e2c000;
tagLpscReg volatile * const g_ptLpsc0Reg = (tagLpscReg *)0x01C10000;
tagLpscReg volatile * const g_ptLpsc1Reg = (tagLpscReg *)0x01E27000;

tagPllReg volatile * const g_ptPll0Reg = (tagPllReg*)0x01C11000;
tagPllReg volatile * const g_ptPll1Reg = (tagPllReg*)0x01E1A000;

tagDdriiReg volatile * const g_ptDdriiReg = (tagDdriiReg*)0xb0000000;

tagEmifaReg volatile * const g_ptEmifaReg = (tagEmifaReg*)0x68000000;

tagSpiReg volatile *const g_ptSpi0Reg = (tagSpiReg *)0x01C41000;
tagSpiReg volatile *const g_ptSpi1Reg = (tagSpiReg *)0x01F0E000;

tag_UartReg volatile *const g_ptUart0Reg = (tag_UartReg *)cn_uart0_base;
tag_UartReg volatile *const g_ptUart1Reg = (tag_UartReg *)cn_uart1_base;
tag_UartReg volatile *const g_ptUart2Reg = (tag_UartReg *)cn_uart2_base;

tagTimerReg volatile * const g_ptTimer0Reg = (tagTimerReg *)0x01c20000;
tagTimerReg volatile * const g_ptTimer1Reg = (tagTimerReg *)0x01c21000;
tagTimerReg volatile * const g_ptTimer2Reg = (tagTimerReg *)0x01f0c000;
tagTimerReg volatile * const g_ptTimer3Reg = (tagTimerReg *)0x01f0d000;

struct st_int_reg volatile * const pg_int_reg
                        = (struct st_int_reg *)0xfffee000;

tagLcdcReg volatile * const g_ptLcdcReg
                        = (tagLcdcReg *)0x01E13000;

