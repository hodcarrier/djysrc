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
.syntax         unified
.thumb
.file   "cortex-m4.S"


/*
@----����������---------------------------------------------------------------
@����: ���浱ǰ�̵߳������ģ������л�,ר���ڽ���CN_SLEEP_L3֮ǰ����.
@����: running_vm����ǰ�̵߳Ŀ��ƿ�ָ�룬��&vm->stack��
@����: SaveRamL3, ��ִ�й�step1���ڴ汣�浽flash��.
@����: EntrySleep, ����͹���.
@����:  ��
@����ԭ��: void __LP_BSP_AsmSaveReg(struct tagThreadVm *running_vm��
                    bool_t (*SaveRamL3)(void),
                    void (*EntrySleep)(void))
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __LP_BSP_AsmSaveReg
.thumb
.thumb_func
__LP_BSP_AsmSaveReg:
    mrs     r3,xpsr
    orr     r3,#0x01000000      /* xpsr��T��־�������������ֹ���λ�� */
    push    {r3}                /* ����xpsr */
    push    {lr}                /* ����PC,�˳�L3�͹��ĺ�,���ӱ��������ص�ַ����������*/
    push    {r0-r3,r12,lr}      /* ����r0-r3,r12,lr */
    push    {r4-r11}
    str     sp,[r0]             /* �����������ջָ�뵽vm->stack */
    push    {r2}
    blx      r1                  /* ���ñ���RAM������ʧ�洢���ĺ���*/
    pop     {r2}
    mov 	r0, #0x4			/* ѡ�����߷�ʽ:L3 */
    blx      r2                  /* ���ý���SLEEP3�ĺ��� */
    bx      lr

/*
@----ָ�����ʵ����ʱ����-------------------------------------------------------
@����:  ָ����ʱ
@����:  timeCoreClock��ʱ������Ӧ���ں�ʱ����
@����:  ��
@����ԭ��:  void __asm_delay_cycle(u64 timeCoreClock)
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __asm_delay_cycle
.thumb
.thumb_func
__asm_delay_cycle:
    subs    r0 ,#6      /*��ȥ�����������*/
    lsr     r0 ,r0, #2
 loop:
    subs    r0 ,#1
    nop
    bne     loop
    cmp    r1 ,#0
    beq    end
    subs    r1 ,#1
    mov     r0 ,#0xffffffff
    b       __asm_delay_cycle
end:
    bx      lr

.end