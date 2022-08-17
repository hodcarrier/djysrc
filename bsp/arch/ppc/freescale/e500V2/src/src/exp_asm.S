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
// ģ������:�쳣�����Ļ�ಿ�֣��ò�����Ҫ�����쳣�����������Ƭ������
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա:zhangqf_cyg
// ����ʱ��:2013-7-26--����10:15:29
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
.file   "Exp_Asm.S"
#include "E500V2Reg.h"
#include "E500V2RegBits.h"
#include "VMStack.h"
/*
@-----------------------------------------------------------------------------
@����:�жϴ���ǰ�沿�֣������ֳ�����
@����:
@
@-----------------------------------------------------------------------------
*/
//Ҫ����40���Ĵ���
//spָ����ǿ����õ�ջ�ռ�
//sp-160���ջ��ַҲӦ���ǿյ�
//sp---sp-160֮��Ŀռ�պô湻40���Ĵ���
//spsrָ�����ж�ǰ���Ǹ�msr��һ��ķ���Xsrr1�е�
//�ڴ��У����ǲ�Ӧ���޸�r3/r4/r5��ֵ����Щ�ܿ����Ǵ������ǵĲ���
//PC����XSRR0�� SPSRָ����XSRR1�����X��������ʲô�쳣ģʽ��
.macro  push_current_state pc spsr
    subi   sp, sp, CUR_STACK_SIZE //sp-288�����ַ�ռ��ǿյģ����õ���һ��sp
    stw    r31, 0(sp)//�ڴ˴򿪸���֧�֣���Ϊ��Ҫ64λ����,
    stw    r30, 4(sp)
    lis    r31, cn_msr_spe@h   //make sure that the spe is open
    ori    r31, r31, cn_msr_spe@l
    mfmsr  r30
    or     r31, r30, r31
    mtmsr  r31
    isync
    lwz    r31,0(sp)
    lwz    r30,4(sp)

    evstdd r0, R0_OFFSET(sp)  //-288--280��ŵ���r0����������
                  //������Ĵ�������ͨ�Ĵ���֮�����4�ֽڵĿ���
    evstdd r2, R2_OFFSET(sp)
    evstdd r3, R3_OFFSET(sp)
    evstdd r4, R4_OFFSET(sp)
    evstdd r5, R5_OFFSET(sp)
    evstdd r6, R6_OFFSET(sp)
    evstdd r7, R7_OFFSET(sp)
    evstdd r8, R8_OFFSET(sp)
    evstdd r9, R9_OFFSET(sp)
    evstdd r10, R10_OFFSET(sp)
    evstdd r11, R11_OFFSET(sp)
    evstdd r12, R12_OFFSET(sp)
    evstdd r13, R13_OFFSET(sp)
    evstdd r14, R14_OFFSET(sp)
    evstdd r15, R15_OFFSET(sp)
    evstdd r16, R16_OFFSET(sp)
    evstdd r17, R17_OFFSET(sp)
    evstdd r18, R18_OFFSET(sp)
    evstdd r19, R19_OFFSET(sp)
    evstdd r20, R20_OFFSET(sp)
    evstdd r21, R21_OFFSET(sp)
    evstdd r22, R22_OFFSET(sp)
    evstdd r23, R23_OFFSET(sp)
    evstdd r24, R24_OFFSET(sp)
    evstdd r25, R25_OFFSET(sp)
    evstdd r26, R26_OFFSET(sp)
    evstdd r27, R27_OFFSET(sp)
    evstdd r28, R28_OFFSET(sp)
    evstdd r29, R29_OFFSET(sp)
    isync
    xor    r29, r29, r29
    mr    r29, sp
    addi   r29, r29, R30_OFFSET
    isync
    evstdd r30, 0(r29)
    evstdd r31, 8(r29)
    //evstdd r30, R30_OFFSET(sp)
    //evstdd r31, R31_OFFSET(sp) //evstdd connot exceed 248
    mfcr   r6
    stw    r6, CR_OFFSET(sp)     //compute flag register

    mfspr  r6, xer
    stw    r6, XER_OFFSET(sp)    //integer compute register

    mfspr  r6, ctr
    stw    r6, CTR_OFFSET(sp)    //couter register

    mflr   r6
    stw    r6, LR_OFFSET(sp)     //function swith register

    mfspr  r6, \spsr
    stw    r6, MSR_OFFSET(sp)    //cpu exp state register

    mfspr  r7, \pc
    stw    r7, PC_OFFSET(sp)     //cpu pc register
    isync
    msync
    .endm
/*
@-----------------------------------------------------------------------------
@����:�жϴ������沿�֣������ֳ��ָ�
@����:��
@    �����ָ���һЩ��Ҫ�Ĵ��������ǵ������ԣ�
@-----------------------------------------------------------------------------
*/
.macro  pop_current_state pc spsr
    lis  r3, cn_msr_spe@h
    ori  r3, r3, cn_msr_spe@l
    mfmsr  r4
    or   r3, r3, r4
    mtmsr  r3
    isync   //sure that the spe is open
    lwz   r3, PC_OFFSET(sp)//284-288��ŵ���PC���������������
    mtspr  \pc, r3              //cpu pc register

    lwz   r3, MSR_OFFSET(sp)    //cpu exp state register
    mtspr \spsr, r3
    isync

    lwz   r3, LR_OFFSET(sp)
    mtlr r3                      //function switch register
    isync

    lwz   r3, CTR_OFFSET(sp)
    mtctr r3                     //counter register
    isync

    lwz  r3, XER_OFFSET(sp)
    mtspr  xer, r3               //integer register
    isync

    lwz  r3, CR_OFFSET(sp)
    mtcr  r3                     //compute flag register
    isync

    evldd r0, R0_OFFSET(sp)  //-288--280��ŵ���r0����������
                  //������Ĵ�������ͨ�Ĵ���֮�����4�ֽڵĿ���
    evldd r2, R2_OFFSET(sp)
    //evldd r3, R3_OFFSET(sp) //����ָ������ݵ�ʱ��Ҫ�ã�û�б�Ҫ��ô��ָ�
    evldd r4, R4_OFFSET(sp)
    evldd r5, R5_OFFSET(sp)
    evldd r6, R6_OFFSET(sp)
    evldd r7, R7_OFFSET(sp)
    evldd r8, R8_OFFSET(sp)
    evldd r9, R9_OFFSET(sp)
    evldd r10, R10_OFFSET(sp)
    evldd r11, R11_OFFSET(sp)
    evldd r12, R12_OFFSET(sp)
    evldd r13, R13_OFFSET(sp)
    evldd r14, R14_OFFSET(sp)
    evldd r15, R15_OFFSET(sp)
    evldd r16, R16_OFFSET(sp)
    evldd r17, R17_OFFSET(sp)
    evldd r18, R18_OFFSET(sp)
    evldd r19, R19_OFFSET(sp)
    evldd r20, R20_OFFSET(sp)
    evldd r21, R21_OFFSET(sp)
    evldd r22, R22_OFFSET(sp)
    evldd r23, R23_OFFSET(sp)
    evldd r24, R24_OFFSET(sp)
    evldd r25, R25_OFFSET(sp)
    evldd r26, R26_OFFSET(sp)
    evldd r27, R27_OFFSET(sp)
    evldd r28, R28_OFFSET(sp)
    evldd r29, R29_OFFSET(sp)

    isync
    xor   r3, r3, r3
    mr    r3, sp
    addi  r3, r3, R30_OFFSET
    isync
    evldd r30, 0(r3)
    evldd r31, 8(r3)
    evldd  r3, R3_OFFSET(sp)
    isync
    addi sp, sp, CUR_STACK_SIZE
    .endm


/*
@-----------------------------------------------------------------------------
@����:�ⲿ�жϴ���
@����:any way, we must set the msr's ee bit in the stack storage
@    external exception, no nesting permit
@-----------------------------------------------------------------------------
*/
.text
.align 4
.global Default_externalint_exception
Default_externalint_exception:

    push_current_state  srr0,srr1
    // set the vm msr bit in the msr
    lis r3,CN_MSR_THREAD@h
    ori r3,r3, CN_MSR_THREAD@l
    stw r3,MSR_OFFSET(sp)
    //change the stack here, and save the old sp in ther new stack
    lis  r3, irq_top@h
    ori  r3, r3, irq_top@l
    stw   sp, _VM_STACK_FROM_TOP(r3)//save sp
    subi  sp, r3, _IRQ_TOP_OFFSET   //must be 0x10 aligned

    //for the external exception, the ee bit cleard, so the asyn signal also closed
    //the same time
    bl   Exp_EInt

    //open the asyn signal when we recover the msr
    lis   r3, irq_top@h
    ori   r3, r3, irq_top@l
    lwz  sp, _VM_STACK_FROM_TOP(r3)//�޸�SPΪirq_top������
    pop_current_state  srr0 srr1
    isync
    rfi
/*
@-------------------------------------------------------------------------------
@����:tick�жϴ���
@����:��
@
@-------------------------------------------------------------------------------
*/
.text
.align 4
.global Default_tick_exception
Default_tick_exception:

    push_current_state  srr0,srr1
    // set the vm msr bit in the msr
    lis r3,CN_MSR_THREAD@h
    ori r3,r3, CN_MSR_THREAD@l
    stw r3,MSR_OFFSET(sp)
    //change the stack here, and save the old sp in ther new stack
    lis  r3, irq_top@h
    ori  r3, r3, irq_top@l
    stw   sp, _VM_STACK_FROM_TOP(r3)//save sp
    subi  sp, r3, _IRQ_TOP_OFFSET   //must be 0x10 aligned
    //clear the dtimer flag in the tsr
    lis   r3, cn_tsr_dis@h
    ori   r3, r3,  cn_tsr_dis@l
    mtspr tsr, r3
    isync
    //for the external exception, the ee bit cleard, so the asyn signal also closed
    //the same time
    bl   Exp_DTimer

    //open the asyn signal when we recover the msr
    lis   r3, irq_top@h
    ori   r3, r3, irq_top@l
    lwz  sp, _VM_STACK_FROM_TOP(r3)//�޸�SPΪirq_top������
    pop_current_state  srr0 srr1
    isync
    rfi

/*
@-----------------------------------------------------------------------------
@����:sys�쳣����
@����:��
@ ������ӵ���Լ����쳣ջ
@-----------------------------------------------------------------------------
*/
//tlb /isi/ dsi /spe / alignment exp could use this exp handler
//this exception will deal with the normal exception to debug the system
//ofcource, it is the release version,
//it is multi exception handler, which only gather the information at exp
//in the debug version with cw, we will use program exception instead
//any way, you could add more information if you thought it necessary
//most of information for debug the app or the sys
/*
@-----------------------------------------------------------------------------
@����:sys general exception dealer
@����:spr4��ŵ����ж�ʱ���ջָ�룬spr5��ŵ����쳣��
@-----------------------------------------------------------------------------
*/
.text
.align 4
.global Default_sys_general_exception
Default_sys_general_exception:
    b     .
    rfi

/*
@-----------------------------------------------------------------------------
@����:Critical Input �쳣����
@����:ʵʱ�ж�
@-----------------------------------------------------------------------------
*/
.text
.align 4
.global Default_cint_exception
Default_cint_exception:
    b     .
    rfci
/*
@-----------------------------------------------------------------------------
@����:Machine Check �쳣����
@����:spr4��ŵ����ж�ʱ���ջָ�룬spr5��ŵ����쳣��
@-----------------------------------------------------------------------------
*/
.text
.align 4
.global Default_machinecheck_exception
Default_machinecheck_exception:
    b    .
    //any way, we had done what we should done, its time to go back
    rfmci

/*
@-----------------------------------------------------------------------------
@����:DEBUG�쳣����
@����:��
@
@-----------------------------------------------------------------------------
*/
.text
.align 4
.global Default_debug_exception
Default_debug_exception:
    subi sp, sp, 0x12
    stw  r3, 0(sp)
    stw  r4, 4(sp)
    stw  r5, 8(sp)
    //up till now it is the test version to compatile with the CW debug
    //any way, we need to clear the esr and dbsr
    xor  r3, r3, r3
    mtspr esr, r3
    isync

    mfspr r3, dbsr
    mr  r4, r3
    //use to check csrr0 to map the address
    rlwinm r4, r4, 1, 31, 31
    cmplwi r4, 0x00
    bgt   csrr0_delt
    mfspr r4, csrr0    //if dbsr[ide]=0, csrr0 contains the ins that cause exp
    addi  r4, r4, 0x04
    mtspr csrr0, r4
    isync
csrr0_delt:
    mtspr dbsr, r3
    isync
    //now recover now ,it is so simple version
    lwz   r3, 0(sp)
    lwz   r4, 4(sp)
    lwz   r5, 8(sp)
    msync
    addi  sp, sp, 0x12
    rfci//do nothing,restore the dsrr0 and dsrr1

/*
@----�����߳�-------------------------------------------------------------------
@����:  ��ʼ���̵߳�ջ���������߳�ִ�к������´����߳�ʱ������ñ�����
@����:  thread_routine�������õ��̺߳���
@       vm���߳�ָ��
@����:  ��ʼ��������ĵ�ǰջָ��
@����ԭ��:void * __asm_reset_thread(void (*thread_routine)(struct event_script *),
@                                        struct  tagThreadVm  *vm)@
@-------------------------------------------------------------------------------
*/
.align 4
.text
.global __asm_reset_thread
__asm_reset_thread:

    stwu  sp, -32(sp)
    stw   r5, 16(sp)
    stw   r6, 20(sp)
    msync

    //get the vm stack top. then save the value to the stack
    lwz     r5, 0x04(r4)
    subi    r5, r5, CUR_STACK_SIZE
    xor     r6, r6, r6
    stw     r6, CR_OFFSET(r5)    //clear cr
    stw     r6, XER_OFFSET(r5)   //clear xer
    stw     r6, CTR_OFFSET(r5)   //clear counter
    stw     r6, LR_OFFSET(r5)    //clear lr
    lis     r6, CN_MSR_THREAD@h
    ori     r6, r6, CN_MSR_THREAD@l
    stw     r6, MSR_OFFSET(r5)   //reset the msr
    lis     r6, __DJY_VmEngine@h
    ori     r6, r6, __DJY_VmEngine@l
    stw     r6, PC_OFFSET(r5)    //set the pc
    evstdd  r3, R3_OFFSET(r5)    //set the r3(thread_routine) for the parameter of __DJY_VmEngine
    stw     r5, 0(r4)            //save the sp in the member top of vm
    msync
    //recover for the current thread
    lwz r5, 16(sp)
    lwz r6, 20(sp)
    lwz sp, 0(sp)
    blr
/*
@----��λ���̣߳��л������߳�-------------------------------------------------
@����:  ��old_vm�����������³�ʼ�����´�����״̬��Ȼ���л������̵߳�
@       �������м���ִ�С�
@       ��һ���ڳ�פ�ڴ���߳��д������¼���ɺ󣬲���ɾ�����̣߳����븴λ
@       ���̣߳�Ȼ���л������������̣߳���ʱ�����Ҫ���ñ���������˱�������
@       ����λ���߳�����������ִ�С�
@����:  thread_routine�������õ��̺߳���
@       new_vm��Ŀ���̵߳��߳�
@       old_vm������λ���߳�
@����:  ��
@����ԭ��:void __asm_reset_switch(void (*thread_routine)(void),
@                           struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.text
.global __asm_reset_switch
__asm_reset_switch:

    //reset the old vm thread
    //get the vm stack top. then save the value to the stack
    lwz     r7, 0x04(r5)
    subi    r7, r7, CUR_STACK_SIZE
    xor     r6, r6, r6
    stw     r6, CR_OFFSET(r7)    //clear cr
    stw     r6, XER_OFFSET(r7)   //clear xer
    stw     r6, CTR_OFFSET(r7)   //clear counter
    stw     r6, LR_OFFSET(r7)    //clear lr
    lis     r6, CN_MSR_THREAD@h
    ori     r6, r6, CN_MSR_THREAD@l
    stw     r6, MSR_OFFSET(r7)   //reset the msr
    lis     r6, __DJY_VmEngine@h
    ori     r6, r6, __DJY_VmEngine@l
    stw     r6, PC_OFFSET(r7)    //set the pc
    evstdd  r3, R3_OFFSET(r7)    //set the r3(thread_routine) for the parameter of __DJY_VmEngine
    stw     r7, 0(r5)            //save the sp in the member top of vm
    msync
    //ok, now we'd better to recover the new vm stack to the registers.
    lwz     sp, 0(r4)            //get the new sp
    pop_current_state  srr0 srr1 //recover the registers here
    isync
    rfi                          //turn back to the new thread


/*
@----�������¼�����-----------------------------------------------------------
@����: ϵͳ��ʼ����ɺ��������¼����Ȳ������һ���¼�.
@����: new_vm���״��������߳̿��ƿ�ָ��
@����: ��
@����ԭ��: void __asm_start_thread(struct  tagThreadVm  *new_vm)@
@-----------------------------------------------------------------------------
*/
.text
.global __asm_turnto_context
__asm_turnto_context:
.global __asm_start_thread
__asm_start_thread:
    //ok, now we'd better to recover the new vm stack to the registers.
    lwz     sp, 0(r3)            //get the new sp
    pop_current_state  srr0 srr1 //recover the registers here
    isync
    rfi                          //turn back to the new thread
/*
@----�������л�---------------------------------------------------------------
@����:  ���浱ǰ�̵߳������ģ��л������̵߳��������м���ִ�С�
@����:  new_sp���������ĵ�ջָ��
@����:  old_sp���������ĵ�ջָ���ָ�룬��&vm->stack�������ṩ��������ջָ�룬
@              sp�Ĵ����ĵ�ǰֵ����
@����:  ��
@����ԭ��:
void __asm_switch_context(struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)
@-----------------------------------------------------------------------------
*/
.text
.global __asm_switch_context
__asm_switch_context:
    push_current_state  srr0,srr1
    // set the vm msr bit in the msr
    lis r5,CN_MSR_THREAD@h
    ori r5,r5, CN_MSR_THREAD@l
    stw r5,MSR_OFFSET(sp)
    //modified the pc register
    lis r5, ASM_THREAD_BACK@h
    ori r5, r5, ASM_THREAD_BACK@l
    stw r5, PC_OFFSET(sp)
    //save the sp in the old vm stack
    stw sp, 0(r4)
    msync
    //now, pop the new thread
    lwz sp, 0(r3)
    pop_current_state  srr0 srr1 //recover the registers here
    isync
    rfi                          //turn back to the new thread

ASM_THREAD_BACK: //when turn back ,the old thread will run here to turn back
    blr


/*
@----�ж��е��������л�-------------------------------------------------------
@����:  ���汻�ж��̵߳������ģ��л������̵߳��������м���ִ�С���������Ȼ����
@       �Ϸ�����(���û���int_isr_connect�������ӵ��жϷ�����)�У�����ARM
@       �У�ȴ������svc̬
@����:  new_sp���л�Ŀ���߳�
@����:  old_sp�����ж��߳�
@����:  ��
@����ԭ��: void __asm_switch_context_int(struct tagThreadVm *new_vm,struct tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.text
.global __asm_switch_context_int
__asm_switch_context_int:

    stwu sp, -32(sp)
    stw  r5, 16(sp)
    stw  r6, 20(sp)
    msync

    //just save the sp in the old vm stack, and changge it with the new sp
    lis r5, irq_top@h
    ori r5, r5, irq_top@l
    lwz r6, _VM_STACK_FROM_TOP(r5)
    stw r6, 0(r4)
    lwz r6, 0(r3)
    stw r6, _VM_STACK_FROM_TOP(r5)

    lwz  r5, 16(sp)
    lwz  r6, 20(sp)
    lwz  sp, 0(sp)

    blr
/*
@-------------------------------------------------------------------------------
@����: ���ԭ�Ӳ���ԭ��_start
@����:
@����:
@����ԭ��: hight_atom_t Int_HighAtomEnd()
@-------------------------------------------------------------------------------
*/
.text
.global Int_HighAtomStart
Int_HighAtomStart:
    stwu  sp, -32(sp)
    stw   r4, 4(sp)

    mfmsr r3
    lis   r4, (CN_MSR_TRUNK_DIS)@h
    ori   r4, r4, (CN_MSR_TRUNK_DIS)@l
    and   r4, r4, r3
    mtmsr r4

    lwz r4, 4(sp)
    lwz sp, 0(sp)
    blr

/*
@-------------------------------------------------------------------------------
@����: ���ԭ�Ӳ���ԭ��_end
@����:
@����:
@����ԭ��: Int_HighAtomEnd(high_atom_t)
@-------------------------------------------------------------------------------
*/
.text
.global Int_HighAtomEnd
Int_HighAtomEnd:
    stwu  sp, -32(sp)

    mtmsr r3
    isync

    lwz sp, 0(sp)
    blr
/*
@-------------------------------------------------------------------------------
@����: ���ԭ�Ӳ���ԭ��_start:�ͼ�ԭ�Ӳ���
@����:
@����:
@����ԭ��: low_atom_t Int_LowAtomStart()
@-------------------------------------------------------------------------------
*/
.text
.global Int_LowAtomStart
Int_LowAtomStart:

    stwu  sp, -32(sp)
    stw   r4, 4(sp)

    mfmsr r3
    lis   r4, (CN_MSR_ASYN_DIS)@h
    ori   r4, r4, (CN_MSR_ASYN_DIS)@l
    and   r4, r4, r3
    mtmsr r4

    lwz r4, 4(sp)
    lwz sp, 0(sp)
    blr
/*
@-------------------------------------------------------------------------------
@����: ���ԭ�Ӳ���ԭ��_end:�ͼ�ԭ�Ӳ���
@����:
@����:
@����ԭ��:void Int_LowAtomEnd(low_atom_t)
@-------------------------------------------------------------------------------
*/
.text
.global Int_LowAtomEnd
Int_LowAtomEnd:
    stwu  sp, -32(sp)

    mtmsr r3
    isync

    lwz sp, 0(sp)
    blr


/*
@-------------------------------------------------------------------------------
@����: ���첽�ź�
@����:
@����:
@����ԭ��:void Int_CutAsynSignal(void)
@-------------------------------------------------------------------------------
*/
.text
.global Int_CutAsynSignal
Int_CutAsynSignal:

    isync
    msync
    wrteei 0x00
    isync
    msync
    blr
/*
@-------------------------------------------------------------------------------
@����: ���첽�ź�
@����:
@����:
@����ԭ��:void Int_ContactAsynSignal(void)
@-------------------------------------------------------------------------------
*/
.text
.global Int_ContactAsynSignal
Int_ContactAsynSignal:

    isync
    msync
    wrteei 0x01
    isync
    msync
    blr

.end
//***************************file end*************************//





