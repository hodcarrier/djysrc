/*#include "cpu.h"*/

.global __asm_reset_thread
.globl djy_switch_interrupt_flag
.globl djy_interrupt_from_thread
.globl djy_interrupt_to_thread


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
@----�����߳�-----------------------------------------------------------------
@����:  ��ʼ���̵߳�ջ���������߳�ִ�к������´����߳�ʱ������ñ�����
@����:  thread_routine�������õ��̺߳���
@               vm���߳������ָ��
@����:  ��ʼ��������ĵ�ǰջָ��
@����ԭ��:void __asm_reset_thread(void (*thread_routine)(void),
@                                   struct  tagThreadVm  *vm);
@-----------------------------------------------------------------------------
.section .text.__asm_reset_thread, "ax", %progbits
.global __asm_reset_thread
__asm_reset_thread:
    LDR     R2,[R1,#4]          @ȡ�����ջ��ָ��
    LDR     R3,=__DJY_VmEngine  @ȡ���������ָ��
    STMFD   R2!,{R3}            @��pc��
    SUB     R2,R2,#13*4         @����13���Ĵ���,��ʼ״̬r1-r12���⣬__vm_engine
                                @���������أ�lrҲ������
                                @__vm_engine�Ĳ������л�������ʱ��service_routine���ָ���r0�У����ݵ���Լ
                                @����r0��ֵ����__vm_engine�����Ĳ�������ֻ�ڴ�������ʼ���߳�������ʱ��Ҫ��
                                @�������߳������Ժ��߳��л�Ҫ����������ʱ������r0��ʵ��ֵ����
    STMFD   R2!,{R0}            @���� thread_routineָ�뵽r0��λ��.
    LDR     R3,=BOOT_MODE_SVC   @ȡcpsr
    TST     R0, #0X01
    BEQ     __ASM_ARM
    ORR     R3, R3, #0X20           @; it's thumb code set T=1
__ASM_ARM:
    STMFD   R2!,{R3}                    @����cpsr
    STR     R2,[R1]             @����vm�ĵ�ǰջָ�뵽vm->stack��
    BX      LR
@ end of __asm_reset_thread
/*
@----��λ���̣߳��л������߳�-------------------------------------------------
@����:  ��old_vm�����������³�ʼ�����´�����״̬��Ȼ���л������̵߳�
@       �������м���ִ�С�
@       ��һ���ڳ�פ�ڴ���߳��д������¼���ɺ󣬲���ɾ�����̣߳����븴λ
@       ���̣߳�Ȼ���л������������̣߳���ʱ�����Ҫ���ñ���������˱�������
@       ����λ���߳�����������ִ�С�
@����:  thread_routine�������õ��̺߳���
@       new_vm��Ŀ���̵߳Ŀ��ƿ�ָ��
@       old_vm������λ�̵߳Ŀ��ƿ�ָ��
@����:  ��
@����ԭ��:void __asm_reset_switch(void (*thread_routine)(void),
@                           struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_reset_switch
.type   __asm_reset_switch, %function
__asm_reset_switch:
    MOV     R5,  R1
    MOV     R1,  R2
    BL      __asm_reset_thread
    LDR     SP, [R5]                @; get new task stack pointer
    LDMFD   SP!, {R4}               @; pop new task cpsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----�������¼�����-----------------------------------------------------------
@����: ϵͳ��ʼ����ɺ��������¼����Ȳ������һ���¼�.
@����: new_vm���״��������߳̿��ƿ�ָ��
@����: ��
@����ԭ��: void __asm_start_thread(struct  tagThreadVm  *new_vm)@
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_start_thread
.type   __asm_start_thread, %function
__asm_start_thread:
    LDR     SP, [R0]                @; get new task stack pointer
    LDMFD   SP!, {R4}               @; pop new task cpsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----����������---------------------------------------------------------------
@����:  ������ԭ�����ģ�ֱ�������µ�������ִ��
@����:  new_vm����������߳̿��ƿ�ָ��
@����:  ��
@����ԭ��: void __asm_turnto_context(struct  tagThreadVm  *new_vm)@
@˵��:  ���¼����,��û�б�Ҫ������¼���������,ֱ���л������¼�����.
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_turnto_context
.type   __asm_turnto_context, %function
__asm_turnto_context:
    LDR     SP, [R0]                @; get new task stack pointer
    LDMFD   SP!, {R4}               @; pop new task cpsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----�������л�---------------------------------------------------------------
@����:  ���浱ǰ�̵߳������ģ��л������̵߳��������м���ִ�С�
@����:  new_vm�������̵߳Ŀ��ƿ�ָ��
@����:  old_vm�������̵߳Ŀ��ƿ�ָ�룬��&vm->stack�������ṩ��������ջָ�룬sp�Ĵ����ĵ�ǰֵ����
@����:  ��
@����ԭ��: void __asm_switch_context(struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_switch_context
.type   __asm_switch_context, %function
__asm_switch_context:
 /*������̣߳��г����߳�*/
    STMFD   SP!, {LR}               @; push pc (lr should be pushed in place of pc)
    STMFD   SP!, {R0-R12, LR}       @; push lr & register file
    MRS     R4, CPSR
    tst     lr, #0x01
    beq     _ARM_MODE
    orr     r4, r4, #0x20           @; it's thumb code set T=1

_ARM_MODE:
    STMFD   SP!, {R4}               @; push cpsr
    STR     SP, [R1]                @; store sp in preempted tasks tcb

    LDR     SP, [R0]                @; get new task stack pointer

    LDMFD   SP!, {R4}               @; pop new task spsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----�ж��е��������л�-------------------------------------------------------
@����:  ���汻�ж��̵߳������ģ��л������̵߳��������м���ִ�С���������Ȼ����
@       �Ϸ�����(���û���int_isr_connect�������ӵ��жϷ�����)�У�����ARM
@       �У�ȴ������svc̬
@����:  new_vm�������̵߳Ŀ��ƿ�ָ��
@����:  old_vm�����ж��̵߳Ŀ��ƿ�ָ��
@����:  ��
@����ԭ��: void __asm_switch_context_int(struct tagThreadVm *new_vm,struct tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_switch_context_int
.type   __asm_switch_context_int, %function
__asm_switch_context_int:
    LDR     R2, =djy_switch_interrupt_flag
    MOV     R3, #1                          @; set flag to 1
    STR     R3, [R2]
    LDR     R2, =djy_interrupt_from_thread   @; set djy_interrupt_from_thread
    /*LDR     R1, [R1]*/
    STR     R1, [R2]
    LDR     R2, =djy_interrupt_to_thread     @; set djy_interrupt_to_thread
    /*LDR     R0, [R0]*/
    STR     R0, [R2]

    BX      LR

.align 2
.global __asm_delay_cycle
.type   __asm_delay_cycle, %function
__asm_delay_cycle:
    subs    r0 ,#6      /*��ȥ�����������*/
    lsr     r0 ,r0, #2  /*ÿ��ѭ��4����е���� ÿ����е���� 1���ں�ʱ��*/
 loop:
    subs    r0 ,#1
    bne     loop
    subs    r1 ,#1
    bne     end
    mov     r0 ,#0xffffffff
    b       __asm_delay_cycle
end:
    bx      lr

.global __asm_bl_fun
.type   __asm_bl_fun, %function
__asm_bl_fun:
    BX   R0


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
.global __LP_BSP_AsmSaveReg
.type   __LP_BSP_AsmSaveReg, %function
__LP_BSP_AsmSaveReg:
    push    {lr}                /* ����PC,�˳�L3�͹��ĺ�,���ӱ��������ص�ַ����������*/
    push    {r0-r3,r12,lr}      /* ����r0-r3,r12,lr */
    push    {r4-r11}
    str     sp,[r0]             /* �����������ջָ�뵽vm->stack */
    push    {r2}
    blx      r1                  /* ���ñ���RAM������ʧ�洢���ĺ���*/
    pop     {r2}
    mov     r0, #0x4            /* ѡ�����߷�ʽ:L3 */
    blx      r2                  /* ���ý���SLEEP3�ĺ��� */
    bx      lr

 /*   BX   LR */


/*
; * void WFI(void);
; */
    .text
    .code 32
    .globl WFI
    .type WFI, %function
WFI:
    MOV R0, #0
    MCR p15, 0, R0, c7, c0, 4
    BX LR

;/*
; * void cp15_enable_alignfault(void)
; */
    .text
    .globl cp15_enable_alignfault
    .type cp15_enable_alignfault, %function
cp15_enable_alignfault:
    MRC p15, 0, R0, c1, c0, 0
    ORR R0, R0, #0x02
    MCR p15, 0, R0, c1, c0, 0
    BX LR

;/*
; * void cp15_disable_alignfault(void)
; */
    .text
    .globl cp15_disable_alignfault
    .type cp15_disable_alignfault, %function
cp15_disable_alignfault:
    MRC p15, 0, R0, c1, c0, 0
    BIC R0, R0, #0x02
    MCR p15, 0, R0, c1, c0, 0
    BX LR

.text
.globl Int_GetPreStatus
.type Int_GetPreStatus, %function
Int_GetPreStatus:
    MRS     R0, SPSR
    and     R0,R0,#0x1f
    BX      LR
.text
.globl Int_GetStatus
.type Int_GetStatus, %function
Int_GetStatus:
    MRS     R0, CPSR
    BX      LR

.text
.globl GetLR
.type GetLR, %function
GetLR:
    mov     R0, LR
    BX      LR

/*���Ӳ�����ʱ��jtag�ڱ�ռ�ã����ñ��������ָ�jtag���ܣ�shell���� initjtag �ĵײ�ʵ�� */
.globl EnJtag
.type EnJtag, %function
EnJtag:
    push    {r0-r3}

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFFFCFF
    and     r1,r2
    ldr     r2,=0x100
    orr     r1,r2
    str     r1,[r0]

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFFF3FF
    and     r1,r2
    ldr     r2,=0x400
    orr     r1,r2
    str     r1,[r0]

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFFCFFF
    and     r1,r2
    ldr     r2,=0x1000
    orr     r1,r2
    str     r1,[r0]

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFF3FFF
    and     r1,r2
    ldr     r2,=0x4000
    orr     r1,r2
    str     r1,[r0]

    pop     {r0-r3}
    BX      LR

.end