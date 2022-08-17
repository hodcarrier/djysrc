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
//����ģ��:������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:����������CPUֱ����صĻ����룬��Ҫ���߳��л�����Ĵ��롣
//����˵��:
//�޶���ʷ:
//1. ����: 2014-01-20
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
// ע: psp����������ջ�У�������vm->stack��!!


.syntax         unified
.cpu            cortex-m3
.fpu            softvfp
.thumb
.file   "cpus.S"

/*
@----�����߳�-----------------------------------------------------------------
@����:  ��ʼ���̵߳�ջ���������߳�ִ�к������´����߳�ʱ������ñ�����
@����:  thread_routine�������õ��̺߳���
@       vm���߳̿��ƿ�ָ��
@����:  ��ʼ��������ĵ�ǰջָ��
@����ԭ��:void * __asm_reset_thread(void (*thread_routine)(struct event_script *),
@                                        struct  tagThreadVm  *vm)@
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __asm_reset_thread
.thumb
.thumb_func
__asm_reset_thread:
    ldr     r2,[r1,#4]          /* ȡ�߳�ջ��ָ�� */
    mov     r12,#0x01000000     /* xpsr�ĳ�ʼֵ */
    ldr     r3,=__DJY_VmEngine  /* ȡ�߳�����ָ�� */
    stmfd   r2!,{r3,r12}        /* ��pc��xpsr */
    sub     r2,r2,#14*4         /* ����14���Ĵ���,��ʼ״̬r0-r12�У���r0��������壬 */
                                /* __vm_engine���������أ�lrҲ������ */
    /* ����r0�е�thread_routine��__vm_engine�Ĳ������л�������ʱ��thread_routine��
       �ָ���r0�У����ݵ���Լ����r0��ֵ����__vm_engine�����Ĳ����� */
    str     r0,[r2,#8*4]        /* ���� thread_routineָ�뵽r0��λ��. */
    str     r2,[r1]             /* ����vm�ĵ�ǰջָ�뵽vm->stack�� */
    bx      lr

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
.text
.align 2
.global __asm_reset_switch
.thumb
.thumb_func
__asm_reset_switch:
    ldr     sp,[r2,#4]          /* ȡ���߳�ջ��ָ�� */
    mov     r12,#0x01000000     /* xpsr�ĳ�ʼֵ */
    ldr     r11,=__DJY_VmEngine    /* ȡ�߳�����ָ�� */
    push    {r11,r12}           /* ��pc,xpsr */
    sub     sp,sp,#14*4         /* ����14���Ĵ���,��ʼ״̬r0-r12�У���r0��������壬
                                   __vm_engine���������أ�lrҲ�����壬���ˣ�������̸߳�λ*/
    str     r0,[sp,#8*4]        /* ���� thread_routineָ��.���ˣ�������߳����� */
    str     sp,[r2]             /* ���浱ǰջָ�뵽old_vm->stack�� */

    ldr     sp,[r1]             /* ȡ����������ָ�� */
@    bl      Int_HalfEnableAsynSignal /* ��Ӧdone������ͷ�� __Int_ResetAsynSignal ���� */
    svc     0                   /* ������������ */
    bx      lr

/*
@----�������¼�����-----------------------------------------------------------
@����: ϵͳ��ʼ����ɺ��������¼����Ȳ������һ���¼�.
@����: new_vm���״��������̵߳Ŀ��ƿ�ָ��
@����: ��
@����ԭ��: void __asm_start_thread(struct  tagThreadVm  *new_vm)@
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __asm_start_thread
.thumb
.thumb_func
__asm_start_thread:
    ldr     r1,=msp_top
    msr     msp,r1              /* ������ջָ�룬��ջ�������쳣���ж� */
    mrs     r2, control
    mov     r1,#2
    orr     r1,r2
    ldr     r0,[r0]             /* ȡ����������ָ�� */
    msr     psp,r0
    msr     control,r1          /* �л�����Ȩ�߳�ģʽ��ջָ����psp */


@    bl      Int_HalfEnableAsynSignal /* �߳������󣬵���Ӧ���������� */
    svc     0                   /* ������������ */
    bx      lr

/*
@----����������---------------------------------------------------------------
@����:  ������ԭ�����ģ�ֱ�������µ�������ִ��
@����:  new_vm���������̵߳Ŀ��ƿ�ָ��
@����:  ��
@����ԭ��: void __asm_turnto_context(struct  tagThreadVm  *new_vm)@
@˵��:  ���¼����,��û�б�Ҫ������¼���������,ֱ���л������¼�����.
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __asm_turnto_context
.thumb
.thumb_func
__asm_turnto_context:
    ldr     sp,[r0]             /* ȡ����������ָ�� */
@    bl      Int_HalfEnableAsynSignal /* ��Ӧdone������ͷ�� Int_SaveAsynSignal ���� */
    svc     0                   /* ������������ */
    bx      lr

/*
@----�������л�---------------------------------------------------------------
@����:  ���浱ǰ�̵߳������ģ��л������̵߳��������м���ִ�С�
@����:  new_vm�������̵߳Ŀ��ƿ�ָ��
@����:  old_vm�������̵߳Ŀ��ƿ�ָ�룬��&vm->stack�������ṩ��������ջָ�룬sp�Ĵ����ĵ�ǰֵ����
@����:  ��
@����ԭ��: void __asm_switch_context(struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __asm_switch_context
.thumb
.thumb_func
__asm_switch_context:
    mrs     r3,xpsr
    orr     r3,#0x01000000      /* xpsr��T��־�������������ֹ���λ�� */
    push    {r3}                /* ����xpsr */
    push    {lr}                /* ����PC,�������߳��л�����ʱ���൱��ԭ�̵߳��� */
                                /* ���������أ�����lr���pc */
    push    {r0-r3,r12,lr}      /* ����r0-r3,r12,lr */
    push    {r4-r11}
    str     sp,[r1]             /* �����������ջָ�뵽old_vm->stack */

    ldr     sp,[r0]             /* ȡ����������ָ�� */
@    bl      Int_HalfEnableAsynSignal
    svc     0                   /* ������������ */
    bx      lr
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

.text
.align 2
.global __asm_switch_context_int
.thumb
.thumb_func
__asm_switch_context_int:
    mrs     r2,psp      /* ȡ���ж��̵߳�psp */
    str     r2,[r1]     /* ��pspд���߳����ݽṹ�� */
    ldr     r2,[r0]     /* ȡ�������̵߳�psp */
    msr     psp,r2      /* �������̵߳ĵ�ǰջָ��д��psp���жϷ��ػָ������Ľ��Դ�Ϊ׼ */
    bx      lr

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

.text
.align 2
.global __asm_reset_cpu
.global __asm_reset_cpu_hot
.thumb
.thumb_func
__asm_reset_cpu:
__asm_reset_cpu_hot:
    mov     r0,#0
    mov     pc,r0

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
    mov     r2 ,6    /*ÿ��ѭ����Ҫ6���ں�ʱ��*/
    udiv    r0 ,r0, r2
loop:
    subs    r0 ,#1
    bne     loop
    subs    r1 ,#1
    bne     end
    mov     r0 ,#0xffffffff
    b       __asm_delay_cycle
end:
    bx      lr


    .text
.align 2
.global __asm_bl_fun
.thumb
.thumb_func
__asm_bl_fun:
    mov   pc ,r0
    bx    lr

/*
@----��ഫ�β�ִ�к���-------------------------------------------------
@����:
@����:  ptab�������б�
@      pflag�����������б�
@      old_vm������λ�̵߳Ŀ��ƿ�ָ��
@����:  ��
@����ԭ��:extern void __asm_ExecuteCmd(union param *ptab,
@       enum param_typr *pflag,Ex_shell_func fun,u32 num);
@-----------------------------------------------------------------------------
*/

.text
.align 2
.global __asm_ExecuteCmd
.thumb
.thumb_func
__asm_ExecuteCmd:

    PUSH    {R4-R12,lr}  //�����⼸���Ĵ�������Ҫ�õ�
/*�������ݵ�ͨ�üĴ�������*/
    MOV R4 ,#0   //�Ĵ������μ�����
    MOV R5 ,#0   //��16λѹջ��С��������16λ��һ��ѹջ�����ڱ��е�ƫ��
    MOV R6 ,#0   //����������������

    mov r7 ,r0   //����������
    mov r8 ,r1   //�������ͱ�����
    mov r9 ,r2   //������ַ����
    mov r10,r3   //��������            /* 0,1,2,3,4,5,6��11,12*/
    /*R11������������ */
    /*R12���浱ǰ����ռ�ֽ��� */
R0_R3:
    ldr    r11   ,[r8,r6] //����������
    and    r11   ,r11, #255 //����������
    /*��ȡ������ռ�ֽڴ�С*/
    cmp    r11   ,#0
    beq    size_1
    cmp    r11   ,#1
    beq    size_2
    cmp    r11   ,#2
    beq    size_4
    cmp    r11   ,#3
    beq    size_8
    cmp    r11   ,#4
    beq    size_1
    cmp    r11   ,#5
    beq    size_2
    cmp    r11   ,#6
    beq    size_4
    cmp    r11   ,#7
    beq    size_8
    cmp    r11   ,#8
    beq    size_4
    cmp    r11   ,#9  //�ж������Ƿ�Ϊfloat
    beq    size_4
    cmp    r11   ,#10 //�ж������Ƿ�Ϊdouble
    beq    size_8
    cmp    r11   ,#11
    beq    size_1
    cmp    r11   ,#12
    beq    size_4

    size_8:
        mov r12 ,#8
        b config_R
    size_4:
        mov r12 ,#4
        b config_R
    size_2:
        mov r12 ,#2
        b config_R
    size_1:
        mov r12 ,#1
        b config_R
    config_R:
        cmp    R4   ,#0
        beq    R_R0
        cmp    R4   ,#1
        beq    R_R1
        cmp    R4   ,#2
        beq    R_R2
        cmp    R4   ,#3
        beq    R_R3
        cmp    R5  ,#0
        bne    add_S
        mov    r5, r6, lsl#16
    add_S:
        //����ѹջ4�ֽڶ��� //size =8��+8
        cmp    r12  ,#8
        bne   Stack_4
        add    R5    ,#8
        b  R_next
        R_R0:
            mov    r0, r6, lsl#3
            ldr    r0   ,[r7,r0]
            add    r4   ,#1
            cmp    r12   ,#8
            bne    R_next
            mov    r1, r6, lsl#3
            add    r1   ,#4
            ldr    r1   ,[r7,r1]
            add    r4   ,#1
            b R_next
        R_R1:
            mov    r1, r6, lsl#3
            ldr    r1   ,[r7,r1]
            add    r4   ,#1
            cmp    r12   ,#8
            beq   R_R2
//          bne    R_next
//          mov    r2, r6, lsl#3
//          add    r2   ,#4
//          ldr    r2   ,[r7,r2]
//          add    r4   ,#1
            b R_next
        R_R2:
            mov    r2, r6, lsl#3
            ldr    r2   ,[r7,r2]
            add    r4   ,#1
            cmp    r12   ,#8
            bne    R_next
            mov    r3, r6, lsl#3
            add    r3   ,#4
            ldr    r3   ,[r7,r3]
            add    r4   ,#1
            b  R_next
        R_R3:
            cmp    r12   ,#8
            beq    Stack_8
            mov    r3, r6, lsl#3
            ldr    r3   ,[r7,r3]
            add    r4   ,#1
            b  R_next
Stack_8:
    mov    r5, r6, lsl#16
    add    R5    ,#8
    add    r4   ,#2
    b  R_next
Stack_4:
    add    R5    ,#4
    b  R_next
Stack_2:
    add    R5    ,#2
    b  R_next
Stack_1:
    add    R5    ,#1
R_next:
    add    r6 ,#1
    cmp r6  ,r10
    bne R0_R3

/*
    R5  //��16λѹջ��С������
        //��16λ��һ��ѹջ�����ڱ��е�ƫ��
    r4 ����Դsp��ַ���ں�������ʱʹ��
    R5ѹջƫ��
    R6 //����������������
    mov r7 ,r0   //����������
    mov r8 ,r1   //�������ͱ�����
    mov r9 ,r2   //������ַ����
    mov r10,r3   //��������
    R11 ��������
    R12:
*/

/*ͨ��ջ���ݵĲ���*/
    cmp     r5 ,#0 //����ѹջ
    bne    Stacks
    BLX     R9
    POP     {R4-R12,lr}//�ָ�����ļĴ���
    BX      LR       //����
Stacks:

    mov     r4 ,sp       //����ջָ���ں�������ʱ�ָ�
    LSR     r6 ,r5, #16  //�õ���һ��ѹջ�����ڱ��е�ƫ��

    mov   r11 ,r10
    mov   r10 ,r6,lsl#16   //r10�ĸ�16λ�ŵ�һ��ѹջ�����ڱ��е�ƫ��
    add   r10 ,r11        //r10�ĵ�16λ�Ų�������

    and     r5 ,r5, #255 //����������
    add     r5 ,#7
    and     r5 ,#0xfffffff8//����ָ����ֽڶ��䡣
    add     r5 ,#8
    sub     sp ,r5
    mov     r5 ,#0
loop_1:
    ldr    r11 ,[r8,r6] //����������
    and    r11   ,r11, #255 //����������
    cmp    r11   ,#0
    beq    S_size_1
    cmp    r11   ,#1
    beq    S_size_2
    cmp    r11   ,#2
    beq    S_size_4
    cmp    r11   ,#3
    beq    S_size_8
    cmp    r11   ,#4
    beq    S_size_1
    cmp    r11   ,#5
    beq    S_size_2
    cmp    r11   ,#6
    beq    S_size_4
    cmp    r11   ,#7
    beq    S_size_8
    cmp    r11   ,#8
    beq    S_size_4
    cmp    r11   ,#9  //�ж������Ƿ�Ϊfloat
    beq    S_size_4
    cmp    r11   ,#10 //�ж������Ƿ�Ϊdouble
    beq    S_size_8
    cmp    r11   ,#11
    beq    S_size_1
    cmp    r11   ,#12
    beq    S_size_4

    S_size_8:
        lsls    r12 ,r6, #3  //���ݱ��������õ���������ƫ��
        ldr     r11 ,[r7,r12] //�����ݱ�
        add     r12 ,#4                  //???
        ldr     r12 ,[r7,r12] //����������
        str     r11, [sp ,r5]
        add     r5,  #4
        str     r12, [sp ,r5]
        add     r5,  #4
        b S_next

    S_size_4:
    S_size_2:
    S_size_1:

        sub    r6 ,#1
        ldr    r11 ,[r8,r6] //����������
        and    r11   ,r11, #255 //����������


        add    r6 ,#2
        and    r12 ,r10, #255 //��r10�ĵ�16λ�����������ŵ�r12
        cmp  r6 ,r12
        beq  S_size_4_bak
        ldr    r12 ,[r8,r6] //����������
        and    r12   ,r12, #255 //����������

        cmp    r11   ,#10     //double
        beq    double_x_double
        cmp    r11   ,#3      //u64
        beq    double_x_double
        cmp    r11   ,#7      //s64
        beq    double_x_double

       //�жϵ�һ��ѹջ�Ĳ���8�ֽڣ��������ֽ�
        LSR  r11 ,r10, #16  //�õ���һ��ѹջ�����ڱ��е�ƫ��
        add  r11 ,#1
        cmp  r6  ,r11
        beq  double_x_double  //�жϵ�һ��ѹջ


        sub    r6 ,#3
        sub   r11 ,#2
        cmp   r6, r11
        beq  S_size_4_bak_bak   //�жϵڶ���ѹջ
        ldr    r11 ,[r8,r6] //����������
        and    r11   ,r11, #255 //����������
        add    r6 ,#3
        cmp    r11   ,#10     //double    //�жϵ�����ѹջ
        beq    S_size_4_bak
        cmp    r11   ,#3      //u64
        beq    S_size_4_bak
        cmp    r11   ,#7      //s64
        beq    S_size_4_bak


        LSR  r11 ,r10, #16  //�õ���һ��ѹջ�����ڱ��е�ƫ��
        sub    r6 ,#4
        sub   r11 ,#1
        cmp   r6, r11
        beq  double_x_double_bak   //�жϵ�����ѹջ
        ldr    r11 ,[r8,r6] //����������
        and    r11   ,r11, #255 //����������
        add    r6 ,#4
        cmp    r11   ,#10     //double  //�жϵ��ĸ�ѹջ
        beq    double_x_double
        cmp    r11   ,#3      //u64
        beq    double_x_double
        cmp    r11   ,#7      //s64
        beq    double_x_double


        LSR  r11 ,r10, #16  //�õ���һ��ѹջ�����ڱ��е�ƫ��
        sub    r6 ,#5
        sub   r11 ,#1
        cmp   r6, r11
        beq  S_size_4_bak_bak_bak   //�жϵ��ĸ�ѹջ
        ldr    r11 ,[r8,r6] //����������
        and    r11   ,r11, #255 //����������
        add    r6 ,#5
        cmp    r11   ,#10     //double  //�жϵ����ѹջ
        beq    S_size_4_bak
        cmp    r11   ,#3      //u64
        beq    S_size_4_bak
        cmp    r11   ,#7      //s64
        beq    S_size_4_bak

        b  double_x_double



       double_x_double:
         cmp    r12   ,#10   //double
         beq  S_size_8_bak
         cmp    r12   ,#3    //u64
         beq  S_size_8_bak
         cmp    r12   ,#7    //s64
         beq  S_size_8_bak
         b   S_size_4_bak

      double_x_double_bak:
         cmp    r12   ,#10   //double
         beq  S_size_8_bak_bak
         cmp    r12   ,#3    //u64
         beq  S_size_8_bak_bak
         cmp    r12   ,#7    //s64
         beq  S_size_8_bak_bak
         add   r6 ,#1
         b   S_size_4_bak_bak

S_size_4_bak:

        sub    r6 ,#1
        lsls    r12 ,r6, #3  //���ݱ��������õ���������ƫ��
        ldr     r11 ,[r7,r12] //����������
        str     r11 ,[sp,r5]
        add   r5,  #4
        b S_next

S_size_4_bak_bak:

        add    r6 ,#2
        lsls    r12 ,r6, #3  //���ݱ��������õ���������ƫ��
        ldr     r11 ,[r7,r12] //����������
        str     r11 ,[sp,r5]
        add   r5,  #4
        b S_next

S_size_4_bak_bak_bak:

        add    r6 ,#4
        lsls    r12 ,r6, #3  //���ݱ��������õ���������ƫ��
        ldr     r11 ,[r7,r12] //����������
        str     r11 ,[sp,r5]
        add   r5,  #4
        b S_next


S_size_8_bak:

        sub    r6 ,#1
        lsls    r12 ,r6, #3  //���ݱ��������õ���������ƫ��
        ldr     r11 ,[r7,r12] //����������
        str     r11 ,[sp,r5]
        add   r5,  #4
        mov r12 ,#0
        str     r12, [sp ,r5]
        add     r5,  #4
        b S_next
S_size_8_bak_bak:

        add    r6 ,#3
        lsls    r12 ,r6, #3  //���ݱ��������õ���������ƫ��
        ldr     r11 ,[r7,r12] //����������
        str     r11 ,[sp,r5]
        add   r5,  #4
        mov r12 ,#0
        str     r12, [sp ,r5]
        add     r5,  #4
        b S_next


    S_next:
    add    r6 ,#1
    and     r11 ,r10, #255 //��r10�ĵ�16λ�����������ŵ�r11
    cmp r6  ,r11
    bne loop_1

call_fun:
    /*������Ӧ�ĺ���*/
    BLX     R9
    mov   sp ,r4
    POP     {R4-R12,lr}//�ָ�����ļĴ���
    BX      LR       //����
.end


