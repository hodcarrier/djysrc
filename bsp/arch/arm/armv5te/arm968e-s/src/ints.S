/*#include "cpu.h"*/

.equ NOINT,                             0xc0
.equ BOOT_MODE_MASK,                    0x1F
.equ BOOT_MODE_USR,                     0x10
.equ BOOT_MODE_FIQ,                     0x11
.equ BOOT_MODE_IRQ,                     0x12
.equ BOOT_MODE_SVC,                     0x13
.equ BOOT_MODE_ABT,                     0x17
.equ BOOT_MODE_UND,                     0x1B
.equ BOOT_MODE_SYS,                     0x1F
.equ BOOT_FIQ_IRQ_MASK,                 0xC0
.equ BOOT_IRQ_MASK,                     0x80

.equ    INTSTATUS , (0x00802000 + 19 * 4)

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
.globl Int_LowAtomStart
.type Int_LowAtomStart, %function
Int_LowAtomStart:
@    ldr     r1,=0x802044
@    ldr     r0,[r1]
@    mov        r2,#0
@    str     r2,[r1]
    MRS     R0, CPSR
    ORR     R1, R0, #NOINT
    MSR     CPSR_c, R1
    BX      LR

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
.globl Int_LowAtomEnd
.type Int_LowAtomEnd, %function
Int_LowAtomEnd:
@    ldr     r1,=0x802044
@    str     r0,[r1]
    MSR     CPSR_c, R0
    BX      LR

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
.globl Int_HighAtomStart
.type Int_HighAtomStart, %function
Int_HighAtomStart:
    MRS     R0, CPSR
    ORR     R1, R0, #NOINT
    MSR     CPSR_c, R1
    BX      LR

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
.globl Int_HighAtomEnd
.type Int_HighAtomEnd, %function
Int_HighAtomEnd:
    MSR     CPSR_c, R0
    BX      LR
.text
.globl portENABLE_INT
.type portENABLE_INT, %function
portENABLE_INT:
    MRS     R0, CPSR
    BIC     R1, R0, #0xC0
    MSR     CPSR_c, R1
    BX      LR

;/*
; * int portDISABLE_IRQ( void );
; */
.text
.globl portDISABLE_INT
.type portDISABLE_INT, %function
portDISABLE_INT:
    MRS     R2, CPSR
    ORR     R1, R2, #0xC0
    MSR     CPSR_c, R1
    AND     R0, R2, #0xC0
    BX      LR

.text
.globl portENABLE_IRQ
.type portENABLE_IRQ, %function
portENABLE_IRQ:
    MRS     R0, CPSR
    BIC     R1, R0, #0x80
    MSR     CPSR_c, R1
    BX      LR

;/*
; * void portENABLE_FIQ( void );
; */
.text
.globl portENABLE_FIQ
.type portENABLE_FIQ, %function
portENABLE_FIQ:
    MRS     R0, CPSR
    BIC     R1, R0, #0x40
    MSR     CPSR_c, R1
    BX      LR

;/*
; * int portDISABLE_IRQ( void );
; */
.text
.globl portDISABLE_IRQ
.type portDISABLE_IRQ, %function
portDISABLE_IRQ:
    MRS     R2, CPSR
    ORR     R1, R2, #0x80
    MSR     CPSR_c, R1
    AND     R0, R2, #0x80
    BX      LR

;/*
; * int portDISABLE_FIQ( void );
; */
.text
.globl portDISABLE_FIQ
.type portDISABLE_FIQ, %function
portDISABLE_FIQ:
    MRS     R2, CPSR
    ORR     R1, R2, #0x40
    MSR     CPSR_c, R1
    AND     R0, R2, #0x40
    BX      LR

    .global djy_irq_dispatch
    .global djy_fiq_dispatch

    /*.global rt_interrupt_enter*/
    /*.global rt_interrupt_leave*/
    .global djy_switch_interrupt_flag
    .global djy_interrupt_from_thread
    .global djy_interrupt_to_thread

    .macro PUSH_SVC_REG
        SUB     SP, SP, #17 * 4
        STMIA   SP, {R0 - R12}
        MOV     R0, SP
        MRS     R6, SPSR
        STR     LR, [R0, #15*4]
        STR     R6, [R0, #16*4]
        STR     SP, [R0, #13*4]
        STR     LR, [R0, #14*4]
    .endm

    /* Interrupt */
        .align  5
.globl boot_undefined
.type boot_undefined, %function
boot_undefined:
    PUSH_SVC_REG
    BL boot_undefined
    B       .

        .align  5
.globl boot_swi
.type boot_swi, %function
boot_swi:
    PUSH_SVC_REG
    BL boot_swi
    B       .

        .align  5
.globl boot_pabort
.type boot_pabort, %function
boot_pabort:
    PUSH_SVC_REG
    BL boot_pabort
    B       .

        .align  5
.globl boot_dabort
.type boot_dabort, %function
boot_dabort:
    PUSH_SVC_REG
    BL boot_dabort
    B       .

        .align  5
.globl boot_reserved
.type boot_reserved, %function
boot_reserved:
    PUSH_SVC_REG
    BL boot_reserved
    B       .


 .align  5
.globl irq_handler
.globl tg_int_global
.globl SaveExp
.type irq_handler, %function
irq_handler:
    /*��R0-R12,LR�浽SPջ����*/
    STMFD   SP!, {R0-R12,LR}
@    cmp lr,#0xac
@    beq .

    mrs r2, cpsr
    orr r1, r2, #NOINT          @; disable interrupt
    msr cpsr_c, r1

@    mrs r4, spsr
@    and r1,r4,#0x1f
@    cmp r1,#0x11
@    beq .
@    cmp r1,#0x12
@    beq .

@    ldr r5,=tg_int_global
@    ldr r5,[r5,#20]
@    cmp r5,#0
@    bne .

    BL      djy_irq_dispatch

    LDR     R0, =djy_switch_interrupt_flag
    LDR     R1, [R0]
    CMP     R1, #1
    /*BEQ�Ǹ�IF��䣬�������ǻ���ģ�Ҳ����˵������1������rt_hw_context_switch_interrupt_do�������ִ���������*/
    BEQ     djy_context_switch_interrupt_do

    /*��ջ���浯��R0-R12,LR*/
    /*�������rt_hw_context_switch_interrupt_do������ͻ᲻����*/
    LDMFD   SP!, {R0-R12,LR}
    SUBS    PC,  LR, #4

djy_context_switch_interrupt_do:
    /*����ջ֡����ַ�Ӵ�С�����ǣ�pc,lr,r12-r4,r3-r0,spsr*/
    /*SP��������*/
    /*ARM9����û������ջָ�룿�����SPʵ���������õ��Ǳ��жϴ�ϵ��̵߳�ջ*/

@    ldr r1,=0x40d080
@    ldr r2,[r1]
@    sub r2,r2,#1
@    str r2,[r1]

    MOV     R1,  #0
    /*�����ǰ�djy_switch_interrupt_flag��1*/
    STR     R1,  [R0]

    MOV     R1, SP
    /*R0-R3���ĸ��Ĵ���*/
    ADD     SP, SP, #4*4
    /*����R4-R12,LR�Ĵ���*/
    LDMFD   SP!, {R4-R12,LR}
    /*��SPSR��ֵ���͵�R0*/
    MRS     R0,  SPSR
    SUB     R2,  LR, #4

    MSR     CPSR_c, #BOOT_FIQ_IRQ_MASK|BOOT_MODE_SVC
    /*ѹR2,ʵ����R2���ڵ���PC��ֵ*/
    STMFD   SP!, {R2}

@    cmp r2,#0xac
@    beq .

    /*ѹR4-R12,LR*/
    STMFD   SP!, {R4-R12,LR}
    /*R1��ԭ��SP��ֵ������R1-R4*/
    /*�����Ǹ��ӣ��һ���Ϊ��������R1��R4��ֵ��ʵ������R0-R3��ֵ��ֻ�����Ǳ�����R1-R4�������*/
    /*����SPָ���Ѿ��л����̵߳�ջָ���ˣ�֮ǰ����IRQ�Լ���*/
    LDMFD   R1,  {R1-R4}
    /*ʵ��ѹ����Ǳ��г��¼���R0-R3*/
    STMFD   SP!, {R1-R4}
    /*��SPSR��ֵѹ���µ�ջ��*/
    STMFD   SP!, {R0}

    LDR     R4,  =djy_interrupt_from_thread
    LDR     R5,  [R4]


    /*��ջ����ֵ�浽rt_interrupt_from_thread��*/
    STR     SP,  [R5]

    LDR     R6,  =djy_interrupt_to_thread
    LDR     R6,  [R6]
    /*���µ�ջ����ֵ�浽SP����*/
    LDR     SP,  [R6]

    LDMFD   SP!, {R4}
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    /*��ջ���浯��r0-r12,lr,pc������pc����������ת���߳�����ִ����*/
    LDMFD   SP!, {R0-R12,LR,PC}^


.align  5
.globl fiq_handler
.type fiq_handler, %function
fiq_handler:
    STMFD   SP!,{R0-R7,LR}

@    cmp lr,#0xac
@    beq .

@    ldr r5,=tg_int_global
@    ldr r5,[r5,#20]
@    cmp r5,#0
@    bne .

@    nop
@    mrs r4,spsr
@    and r1,r4,#0x1f
@    cmp r1,#0x11
@    beq .
@    cmp r1,#0x12
@    bne abc
@    cmp lr,#0x1000
@    ble abc
@    b .
abc:
    BL      djy_fiq_dispatch
@    LDR R0,=INTSTATUS
@    LDR R0,[R0]
@    LDR     R2, =intertab         /* ȡ�첽�źŵ�ַ */
@    MOV     LR,PC               /* ��ִ��4G�ռ��ڵĺ������� */
@    LDR     PC,[R2]             /* INTN״̬��R0���� */

@    MRS R3, spsr
@    AND R2, R3, #0x1F
@    CMP R2, #BOOT_MODE_IRQ            @; fiq from irq(0x12)
@    BEQ fiq_handler_return

    LDR     R0, =djy_switch_interrupt_flag
    LDR     R1, [R0]
    CMP     R1, #1
    BEQ     djy_context_switch_interrupt_fiq_do

fiq_handler_return:
    LDMFD   SP!,{R0-R7,LR}
    SUBS    PC, LR, #4

djy_context_switch_interrupt_fiq_do:

@    ldr r1,=0x40d080
@    ldr r2,[r1]
@    sub r2,r2,#1
@    str r2,[r1]

    MOV     R1,  #0
    /*�����ǰ�djy_switch_interrupt_flag��1*/
    STR     R1,  [R0]

    MOV     R1, SP             @; pop {R0-R7,LR} but skip R0-R3
    ADD     SP, SP, #4*4
    /*����ֻ��R4-R7,LR������R0-R3*/
    LDMFD   SP!, {R4-R7,LR}

    /*ȡ��SPSR��ֵ*/
    MRS     R0,  SPSR
@    and        r2,r0,#0x1f
@    cmp r2,#0x12
@    bne abcd
@    b .
abcd:
    /*ȡ��PC��ֵ*/
    SUB     R2,  LR, #4        @; Save old task's PC to R2

    MSR     CPSR_c, #BOOT_FIQ_IRQ_MASK|BOOT_MODE_SVC
    /*����һ���л���SP��ֵ����FIQ��ջ�л����̵߳�ջ*/
    STMFD   SP!, {R2}          @; Push old task's PC
    STMFD   SP!, {R4-R12,LR}   @; Push old task's LR,R12-R4
    LDMFD   R1,  {R1-R4}       @; pop old thread R0-R3 to R1-R4
    STMFD   SP!, {R1-R4}       @; Push old thread R0-R3
    STMFD   SP!, {R0}          @; Push old task's CPSR

    LDR     R4,  =djy_interrupt_from_thread
    LDR     R5,  [R4]

    ldr r6,[r5,#4]
    cmp sp,r5
    ble  .
    cmp sp,r6
    bge  .

    STR     SP,  [R5]

    LDR     R6,  =djy_interrupt_to_thread
    LDR     R6,  [R6]
    LDR     SP,  [R6]

    ldr r7,[r6,#4]
    cmp sp,r6
    ble  .
    cmp sp,r7
    bge  .


    LDMFD   SP!, {R4}
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4

    LDMFD   SP!, {R0-R12,LR,PC}^

@ .text
@ .align  2
@ .globl irq_handler
@ .type irq_handler, %function
@ irq_handler:
@     /*��R0-R12,LR�浽SPջ����*/
@     STMFD   SP!,{R0-R12,LR}          /* �����Ĵ���,�Լ����ص�ַ */
@     LDR R0,=INTSTATUS
@     LDR R0,[R0]
@
@     MRS     R1,SPSR
@     STMFD   SP!,{R1}                 /* ����SPSR����֧���ж�Ƕ�� */
@
@     MSR     CPSR,#SF_SVCMODE|SF_IRQ_BIT|SF_FIQ_BIT   /* ����SVCMODE,�Ա������ж�Ƕ�� */
@     STMFD   SP!,{R0,LR}             /* ����LR_SVC,R0�Ǳ���ջ8�ֽڶ��������� */
@     /*LDR     R2,=IRQ_BOTTOM                     ȡ�첽�źŵ�ַ */
@     LDR         R2, =intertab         /* ȡ�첽�źŵ�ַ */
@     MOV     LR,PC               /* ��ִ��4G�ռ��ڵĺ������� */
@     LDR     PC,[R2]             /* INTN��R0����,mode R1���� */
@     LDMFD   SP!,{R0,LR}     /* �ָ�LR_SVC, */
@     MSR     CPSR,#SF_IRQMODE|SF_IRQ_BIT|SF_FIQ_BIT /* ����CPSR,����IRQģʽ����ֹ�ж�,����LR�Ĵ�
@                                         ��������,����IRQ����FIQģʽ����Ҫ��ֹ�ж��Է�Ƕ�ף�
@                                         ֻ���ڽ���SVC״̬������Ƕ�� */
@     LDMFD   SP!,{R0}        /* SPSR->R0 */
@     MSR     SPSR,R0    /* �ָ�SPSR */
@     LDMFD   SP!,{R0-R12,LR}
@     SUBS    PC,LR,#4        /* �˺��жϱ����´� */
@
@ .text
@ .align  2
@ .globl fiq_handler
@ .type fiq_handler, %function
@ fiq_handler:
@   /*��R0-R12,LR�浽SPջ����*/
@     STMFD   SP!,{R0-R12,LR}          /* �����Ĵ���,�Լ����ص�ַ */
@     LDR R0,=INTSTATUS
@     LDR R0,[R0]
@     MRS     R1,SPSR
@     STMFD   SP!,{R1}                 /* ����SPSR_IRQ����֧���ж�Ƕ�� */
@
@     MSR     CPSR,#SF_SVCMODE|SF_IRQ_BIT|SF_FIQ_BIT    /* ����SVCMODE,�Ա������ж�Ƕ�� */
@     STMFD   SP!,{R0,LR}             /* ����LR_SVC,R0�Ǳ���ջ8�ֽڶ��������� */
@
@     /*LDR     R2,=IRQ_BOTTOM                     ȡ�첽�źŵ�ַ */
@     LDR         R2, =intertab+4         /* ȡ�첽�źŵ�ַ */
@
@     MOV     LR,PC               /* ��ִ��4G�ռ��ڵĺ������� */
@     LDR     PC,[R2]             /* INTN��R0���� */
@
@     LDMFD   SP!,{R0,LR}     /* �ָ�LR_SVC, */
@     MSR     CPSR,#SF_FIQMODE|SF_FIQ_BIT|SF_IRQ_BIT   /* ����CPSR,����IRQģʽ����ֹ�ж�,����LR�Ĵ�
@                                         ��������,����IRQ����FIQģʽ����Ҫ��ֹ�ж��Է�Ƕ�ף�
@                                         ֻ���ڽ���SVC״̬������Ƕ�� */
@     LDMFD   SP!,{R0}        /* SPSR->R0 */
@     MSR     SPSR,R0    /* �ָ�SPSR */
@     LDMFD   SP!,{R0-R12,LR}
@     SUBS    PC,LR,#4        /* �˺��жϱ����´� */


.end