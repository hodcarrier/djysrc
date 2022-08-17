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
//����ģ��:  �ж�ģ��
//���ߣ�     lst
 //�汾��      V1.0.1
//�ļ�����:  �ж�ģ���еĻ�ಿ��
 //����˵��:  ��
 //�޶���ʷ:
 //3. ����:2010-3-06
 //   ����:lst
 //  �°汾�ţ�1.0.2
 //   �޸�˵��:     stmfd   sp!,{r0-r3,lr}         ����lr_svc,
 //           ��Ϊ  stmfd   sp!,{r0,lr}             ����lr_svc,
 //2. ����:2010-01-04
 //   ����:lst
 //  �°汾�ţ�1.0.1
 //  �޸�˵��: ��gcc��ֲ��keil�������޸Ĵ���
 //1. ����:2009-03-10
 //   ����:lst
 //  �°汾�ţ�1.0.0
 //   �޸�˵��: ԭʼ�汾

.cpu    arm920t
.fpu    softvfp
.file   "ints.S"

/* *********************** ********** ***********************
 *��2440�н���ʹ��fiq�ж�,������ֻ��һ���жϱ����ó�fiq,����,û���κ�ֱ�ӵ���Ϣ
 *�������������Ӧ�ĸ��ж�.FIQģʽ�жϺżĴ�������ֻ��ͨ��F_ISPR��ȷ���ж�Դ��
 *ARM9û��ֱ�ӷ���ǰ��0������ָ�ֻ����ѭ����λ�ķ�����ȷ���ж�Դ�����ʱ�俪
 *���ܴ󣬹�2440��FIQ��ú������壬���djyos��2440�汾����ʹ��FIQ�ж�.
*********************** ********** *********************** */

.equ    SF_MODEMASK,    0x1f
.equ    SF_IRQ_BIT,             0x80
.equ    SF_FIQ_BIT,             0x40
.equ    SF_USERMODE,    0x10
.equ    SF_SYSMODE,             0x1f
.equ    SF_FIQMODE,             0x11
.equ    SF_IRQMODE,             0x12
.equ    SF_SVCMODE,             0x13
.equ    SF_ABORTMODE,   0x17
.equ    SF_UNDEFMODE,   0x1b

.equ    INTOFFSET, 0x4a000014

/* cpsr��IFλ��0������ȫ���ж� */
.section .text.int_enable_arm_int, "ax", %progbits
.align 2
.global int_enable_arm_int
int_enable_arm_int:
        mrs r0, cpsr
        bic r0, r0, #SF_IRQ_BIT|SF_FIQ_BIT
        msr cpsr_cxsf, r0
        bx      lr

/* cpsr��IFλ��1����ֹȫ���ж� */
.section .text.int_disable_arm_int, "ax", %progbits
.align 2
.global int_disable_arm_int
int_disable_arm_int:
        mrs  r0, cpsr
        orr  r0, r0, #SF_IRQ_BIT|SF_FIQ_BIT
        msr  cpsr_cxsf, r0
        bx      lr

/* cpsr��Iλ��1����ֹirq�ж� */
.section .text.__int_disable_irq, "ax", %progbits
.align 2
.global __int_disable_irq
__int_disable_irq:
        mrs  r0, cpsr
        orr  r0, r0, #SF_IRQ_BIT
        msr  cpsr_cxsf, r0
        bx      lr

/* cpsr��Iλ��0������irq�ж� */
.section .text.__int_enable_irq, "ax", %progbits
.align 2
.global __int_enable_irq
__int_enable_irq:
        mrs  r0, cpsr
        bic  r0, r0, #SF_IRQ_BIT
        msr  cpsr_cxsf, r0
        bx      lr

/* cpsr��Fλ��1����ֹfiq�ж� */
.section .text.__int_disable_fiq, "ax", %progbits
.align 2
.global __int_disable_fiq
__int_disable_fiq:
        mrs r0,cpsr
        orr r0,r0,#SF_FIQ_BIT
        msr cpsr_cxsf,r0
        bx      lr

/* cpsr��Fλ��0������fiq�ж� */
.section .text.__int_enable_fiq, "ax", %progbits
.align 2
.global __int_enable_fiq
__int_enable_fiq:
        mrs r0, cpsr
        bic r0, r0, #SF_FIQ_BIT
        msr cpsr_cxsf, r0
        bx      lr

/* *********************** ********** ***********************
//----����߼�ԭ�Ӳ���---------------------------------------------------------
//���ܣ�������ǰ���ж�״̬��Ȼ���ֹ���жϡ��߼�ԭ�Ӳ�����ָ�ڼ䲻�����κ�ԭ���ϵĲ�����
//     Int_HighAtomStart--int_high_atom_end��������ʹ�ã��ڱ�������ס�Ĵ�����ڲ�
//     �����������κ�ϵͳ���á�
// ��������
// ���أ�ԭ����ԭ��״̬
//��ע����ֲ����
//-----------------------------------------------------------------------------
*********************** ********** *********************** *
.section .text.Int_HighAtomStart, "ax", %progbits
.global Int_HighAtomStart
Int_HighAtomStart:
        mrs     r0, cpsr
        mov r1, r0
        orr     r1, r1, #SF_IRQ_BIT|SF_FIQ_BIT
        msr     cpsr_c, r1

* *********************** ********** ***********************
//----�뿪�߼�ԭ�Ӳ���---------------------------------------------------------
//���ܣ��ָ������뿪ԭ�Ӳ���ǰ�����ж�״̬��������Ӧ��int_high_atom_start��������
//      Int_HighAtomStart--int_high_atom_end��������ʹ�ã��ڱ�������ס�Ĵ�����ڲ�
//     �����������κ�ϵͳ���á�
//������high��int_high_atom_start�����ԭ��״̬
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
*********************** ********** *********************** *
.section .text.Int_HighAtomEnd, "ax", %progbits
.global Int_HighAtomEnd
Int_HighAtomEnd:
        msr cpsr_c, r0
*/

/* *********************** ********** ***********************
 *�ж���ص�ջ���ţ�
 *1��irq_stack�ű��жϵ������ģ�˳��:lr��r12-r0��cpsr����14��
 *2��svc_stack�����жϷ������������û���д���жϷ�������
 *3��SYS_stack������Ҫ���ж����л������ģ������޹�
*********************** ********** *********************** */
.section .text.start_int, "ax", %progbits
.global HardExp_IrqHandler
HardExp_IrqHandler:
    stmfd   sp!,{r0-r12,lr}          /* �����Ĵ���,�Լ����ص�ַ */
    ldr r0,=INTOFFSET
    ldr r0,[r0]

    mrs     r1,spsr
    stmfd   sp!,{r1}                 /* ����SPSR_irq����֧���ж�Ƕ�� */
    msr     cpsr_c,#SF_SVCMODE|SF_IRQ_BIT   /* ����SVCMODE,�Ա������ж�Ƕ�� */
    stmfd   sp!,{r0,lr}             /* ����lr_svc,r0�Ǳ���ջ8�ֽڶ��������� */

    /*ldr     r2,=irq_bottom                     ȡ�첽�źŵ�ַ */
    ldr         r2, =engine_irq         /* ȡ�첽�źŵ�ַ */

    mov     lr,pc               /* ��ִ��4G�ռ��ڵĺ������� */
    ldr     pc,[r2]             /* intn��r0���� */

    ldmfd   sp!,{r0,lr}     /* �ָ�lr_svc, */
    msr     cpsr_c,#SF_IRQMODE|SF_IRQ_BIT|SF_FIQ_BIT   /* ����cpsr,����IRQģʽ����ֹ�ж�,����lr�Ĵ�
                                        ��������,����irq����fiqģʽ����Ҫ��ֹ�ж��Է�Ƕ�ף�
                                        ֻ���ڽ���svc״̬������Ƕ�� */
    ldmfd   sp!,{r0}        /* spsr->r0 */
    msr     spsr_cxsf,r0    /* �ָ�spsr */
    ldmfd   sp!,{r0-r12,lr}
    subs    pc,lr,#4        /* �˺��жϱ����´� */

.section .text.start_fiq, "ax", %progbits
.global HardExp_FiqHandler
HardExp_FiqHandler:
    b       .
.end