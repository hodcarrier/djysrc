/*
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
@����ģ��:  �ж�ģ��
@���ߣ�     lst
@�汾��      V1.00
@��ʼ�汾������ڣ�2009-08-10
@�ļ�����:  �ж�ģ���еĻ�ಿ��
@����˵��:  ��
@�޶���ʷ:
@2. ...
@1. ����:
@   ����:
@   �°汾�ţ�
@   �޸�˵��:
@------------------------------------------------------
*/
.syntax     unified
.cpu        cortex-m3
.fpu        softvfp
.thumb
.file   "ints.S"

/* *********************** ********** ***********************
;ΪʲôҪ����r4-r11�أ�������жϵ����߳�a�����жϷ�����ʹ�����ȼ����߳�b������
;�͵÷��ص��߳�b�У������߳�a��r4-r11���浽�߳�a���������У���������������Ҫʹ��
;r4-r11��Ϊɶ��������ջ����?����Ϊ__asm_switch_context_int������֪��ջ�������
;����si�汾����û����ģ���������dlsp�汾�г�����
*********************** ********** *********************** */
.text
.align 2
.global __start_asyn_signal
.thumb
.thumb_func
__start_asyn_signal:
    push    {r0,lr}             /* r0ֻ��Ϊ������ջָ��8�ֽڶ��������� */
    mrs     r0,psp
    stmfd   r0!,{r4-r11}        /* �����Ĵ���,�Լ����ص�ַ */
    msr     psp,r0
    mrs     r0,ipsr             /* ȡ�쳣�� */
    sub     r0,r0,#16
    bl      __Int_EngineAsynSignal
    pop     {r0,lr}
    mrs     r0,psp
    ldmfd   r0!,{r4-r11}
    msr     psp,r0
    bx      lr

.text
.align 2
.global __start_real
.thumb
.thumb_func
__start_real:
    push    {r0,lr}             /* r0ֻ��Ϊ������ջָ��8�ֽڶ��������� */
//    mrs     r0,psp
//    stmfd   r0!,{r4-r11}        /* �����Ĵ���,�Լ����ص�ַ */
//    msr     psp,r0
    mrs     r0,ipsr             /* ȡ�쳣�� */
    sub     r0,r0,#16
    bl      __Int_EngineReal
    pop     {r0,lr}
//    mrs     r0,psp
//    ldmfd   r0!,{r4-r11}
//    msr     psp,r0
    bx      lr

/*
@----����ͼ�ԭ�Ӳ���---------------------------------------------------------
@���ܣ���ȡ��ǰ�첽�źſ���״̬��Ȼ���ֹ�첽�źš��ͼ���ԭ�Ӳ�����������ֹ�ٽ�
@      ���ݵĲ������ʣ������ܻᱻʵʱ�жϴ�ϡ�
@      Int_LowAtomStart---int_low_atom_end��������ʹ�ã����Բ���Ƕ�ף��ڿ���
@      Ƕ�׵ĵط�������ʹ��int_save_asyn_signal���ر��ж�(����)��
@      ��������������"Int_RestoreAsynSignal--Int_SaveAsynSignal"�Ŀ��ٰ汾��
@      ����������Ƕ�׵����⣬�ڱ�������ס�Ĵ�����ڣ�Ҳ����������
@      int_restore_asyn_signal��int_save_asyn_signal�е��κ�һ����
@��������
@���أ�ԭ��״̬
@��ע����ֲ����
@����ԭ��:atom_low_t Int_LowAtomStart(void)
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global Int_LowAtomStart
.thumb
.thumb_func
Int_LowAtomStart:
    mrs     r0, basepri
    mov     r1, #0xff
    msr     basepri, r1
    bx      lr

/*
@----�뿪�ͼ�ԭ�Ӳ���---------------------------------------------------------
@���ܣ��ָ�����ǰ�첽�ź�����״̬��������Ӧ��int_low_atom_start�������ס�
@      Int_LowAtomStart---int_low_atom_end��������ʹ�ã����Բ���Ƕ�ף��ڿ���
@      Ƕ�׵ĵط�������ʹ��int_save_asyn_signal���ر��ж�(����)��
@      ��������������"Int_RestoreAsynSignal--Int_SaveAsynSignal"�Ŀ��ٰ汾��
@      ����������Ƕ�׵����⣬�ڱ�������ס�Ĵ�����ڣ�Ҳ����������
@      int_restore_asyn_signal��int_save_asyn_signal�е��κ�һ����
@������low��int_low_atom_start�����ԭ��״̬
@���أ���
@��ע����ֲ����
@����ԭ��:void Int_LowAtomEnd(atom_low_t low)
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global Int_LowAtomEnd
.thumb
.thumb_func
Int_LowAtomEnd:
    msr     basepri, r0
    bx      lr

/*
@----����߼�ԭ�Ӳ���---------------------------------------------------------
@���ܣ�������ǰ���ж�״̬��Ȼ���ֹ���жϡ��߼�ԭ�Ӳ�����ָ�ڼ䲻�����κ�ԭ���ϵĲ�����
@    Int_HighAtomStart--int_high_atom_end��������ʹ�ã��ڱ�������ס�Ĵ�����ڲ�
@    �����������κ�ϵͳ���á�
@��������
@���أ�ԭ����ԭ��״̬
@��ע����ֲ����
@����ԭ��:atom_high_t Int_HighAtomStart(void)
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global Int_HighAtomStart
.thumb
.thumb_func
Int_HighAtomStart:
    mrs     r0, primask
    cpsid   i
    bx      lr

/*
@----�뿪�߼�ԭ�Ӳ���---------------------------------------------------------
@���ܣ��ָ������뿪ԭ�Ӳ���ǰ�����ж�״̬��������Ӧ��int_high_atom_start��������
@     Int_HighAtomStart--int_high_atom_end��������ʹ�ã��ڱ�������ס�Ĵ�����ڲ�
@    �����������κ�ϵͳ���á�
@������high��int_high_atom_start�����ԭ��״̬
@���أ���
@��ע����ֲ����
@����ԭ��:void Int_HighAtomEnd(atom_high_t high)
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global Int_HighAtomEnd
.thumb
.thumb_func
Int_HighAtomEnd:
    msr     primask, r0
    bx      lr


.end