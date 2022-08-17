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
//����ģ��: BSP
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ����cortex-mϵ��CPU��systick��Ϊϵͳticksʱ��Դ��Ĭ�Ϲ�ѡ����dynamic-tick
//         ģʽ�������͹���Ҫ��ߵĳ��ϣ�Ӧʹ�õ͹��Ķ�ʱ�������֮��
//����˵��:
//1. ����: 2019-08-11
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include <stdint.h>
#include <djyos.h>
#include <hard-exp.h>
#include <int.h>
#include <int_hard.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

u32 Get_SysTickCnt();
void Set_SysTickEnd(u32 value);
void  DJY_ScheduleIsr(u32 inc_ticks);


extern struct IntMasterCtrl  tg_int_global;
extern void __DJY_ScheduleAsynSignal(void);

#define CFG_TICKMODE_DYNAMIC        true
#define CFG_REAL_CRITICAL           100     //�����뵱ǰtick�ж�ʱ��С�ڴ� uS �������޸�tick�ж�ʱ��

static s32 s_gCurrentTicks = 1;     //��ǰtick�������ڡ�
static s32 s_gCriticalCycle;        //CFG_REAL_CRITICAL��Ӧ�Ķ�ʱ������ֵ
static s32 s_gTicksLimit;           //32bit��ʱ�����ζ�ʱ���ticks��
extern s64  g_s64OsTicks;


__attribute__((weak)) void __DjyInitTick(void)
{
    s_gTicksLimit = (s32)((s64)0xffffffff/26000000);
    s_gCriticalCycle =(u32)((u64)1000);

}

//------------------------------------------------------------------------------
//���ܣ�����CPU�����߻����¼�״̬��ʹCPU�ڹ��жϺ�ȷʵ�ܱ������ڹ��ж��ڼ�Ļ����¼�
//     ���жϻ�ദ�����źţ����ѣ����ۻ����¼������� WFE��ARM��ָ�����CPU���ܲ�ͬ��
//     ֮ǰ����֮��
//��������
//���أ���
//ע�����������͵ر����ڵ͹��Ŀ��ơ�
//------------------------------------------------------------------------------
void CleanWakeupEvent(void)
{
//    asm("WFE");
//    asm("SVE");

// __asm__ __volatile__ ("sev" : : : "memory");
// __asm__ __volatile__ ("wfe" : : : "memory");
// #define wfi()  __asm__ __volatile__ ("wfi" : : : "memory")

//    __asm__ volatile ("SEV");
//    __asm__ volatile ("SEV");

//    __SEV();
//    __WFE();
}

// =============================================================================
// ����:��Cortex Mϵͳ�ں��У��ں�ϵͳ��ʱ��SYSTICK��ʱ�ж������쳣���������������첽�ź�
//     ��ں��������±��� g_s64OsTicks ������һ��tick�����Ϊ1��tick��
// ����:��
// ����:��
// =============================================================================
void Exp_SystickTickHandler(void)
{
    g_bScheduleEnable = false;
    tg_int_global.nest_asyn_signal++;

//  g_s64OsTicks += s_gCurrentTicks;

    DJY_ScheduleIsr(s_gCurrentTicks);
    s_gCurrentTicks = 1;
    tg_int_global.nest_asyn_signal--;
    g_bScheduleEnable = true;
}

// =============================================================================
// ���ܣ���ȡ��tick������ϵͳʱ�䣬���bsp���ʹ����ר�ö�ʱ���ṩϵͳʱ�ӣ�����Բ��ṩ
//      ����������ȡʱ���� g_s64OsTicks ���϶�ʱ���ĵ�ǰ����ֵ��
//      ��ȡ��ǰ����ֵ�ر�Ҫע�⣬����ڹ��첽�ź��ڼ���ñ��������ڼ䷢���˶�ʱ�����أ�
//      ����ǡ���ڱ������ڹ��жϺ����ڶ� cycle ֮ǰ�����˶�ʱ�����أ�����Ҫ������ʱ����ת
//      �����⡣
// ��������
// ���أ���ǰus��
// =============================================================================
__attribute__((weak))   u64 __DjyGetSysTime(void)
{
    s64 current;
    atom_low_t atom_low;
//  u32 tick_cnt;
    atom_low = Int_LowAtomStart();
    current = g_s64OsTicks;
//    tick_cnt = Get_SysTickCnt();  //BK7251 �޷�������ʱ����ǰֵ
    Int_LowAtomEnd(atom_low);

//  return (current*CN_CFG_TICK_US + (tick_cnt*1000*1000)/26*1000*1000);
    return (current*CN_CFG_TICK_US);
}


//----�����´�ticks�ж�ʱ��-----------------------------------------------------
//���ܣ��㶨tickģʽ�£�����tick��������1��tick����ֱ�ӷ���1����ʱ������ά�ֲ��䡣
//     ��̬tickģʽ�£�ͨ���޸Ķ�ʱ���������ѵ���tick�����Ϊ����Ticks���������ԭ����
//     ���ζ�ʱ����������жϼ����Ϊ1������Ticks�ĺ��壬�Ǵ��ϴ�tick�ж����𣬶�ʱ��
//     �ǳ����������ߵģ������Ҫ���Ǵ��ϴ�tick�ж�������ʱ���Ѿ��ߵ�ʱ�䣬�������ඨʱ��
//     �в�ͬ�Ĵ�����
//     ��ʱ���������ࣺ
//     1����0�������趨ֵ��Ȼ�������жϣ���ʱ����0��ʼ���¼�ʱ�����ֶ�ʱ����ֻ��Ҫ�޸���
//        ��ֵ���ɣ����뿼�ǵ�ǰ�Ѿ��߹���ʱ�䡣���´��жϼ����Ϊ1�Ĺ�����tick�жϵ�ISR
//        ����ɡ�
//     2������cortex-m��24λsystick��ʱ������reload��ʼ��������0��Ȼ���reload���¿�ʼ
//        �����ģ��������굱��tick�󣬻�Ҫ��reload���õ�1��tick���ر�ע�⣬���ö�ʱ��
//        ʱ��Ҫ��ȥ���ϴ�ISR�ж�������ʱ���Ѿ��߹���ʱ�䡣
//
//��������
//���أ���ǰʱ��
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 Tick_SetNextTimeTick(s32 Ticks)
{
//  s32 temp;
    if(Ticks > s_gTicksLimit)
        Ticks = s_gTicksLimit;
    if(s_gCurrentTicks == Ticks)
        return Ticks;


    //tick�жϿ쵽�ˣ�Ϊ�ܹ������ٽ������µ����⣬�������ı�tickʱ��
    //���ڵ��ñ�����ʱû�йر�ʵʱ�жϣ���ʵʱ�жϵ�ISRִ�в��ܴ��� CFG_REAL_CRITICAL ΢��
    //�ر�ע�ͣ�BK7251 ���۶�ʱ������pwm�����޷�������ǰ����ֵ�����޷�ʵ�ִ˹��ܡ�
    //ֻ��ֱ����������tick��
    //���۷�����Ӧ��û������

//  temp = Get_SysTickCnt();
//  if((temp +s_gCriticalCycle) <(s_gCurrentTicks*26*1000/1000*1000))
//  {
    Set_SysTickEnd(Ticks*(CN_CFG_TICK_US*1000/1000)*26);
    s_gCurrentTicks = Ticks;
//  }

    return s_gCurrentTicks;
}