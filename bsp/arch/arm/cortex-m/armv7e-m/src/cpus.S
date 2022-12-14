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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:调度器
//作者：lst
//版本：V1.0.0
//文件描述:调度器中与CPU直接相关的汇编代码，主要是线程切换方面的代码。
//其他说明:
//修订历史:
//1. 日期: 2014-01-20
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
// 注: psp并不保存在栈中，而是在vm->stack中!!


.syntax     unified
.thumb
.file   "cpus.S"

/* cortex-m4/7的栈帧设计：
padding————对齐用————————LR_RETURN的bit4 == 0才需要
FPSCR————————LR_RETURN的bit4 == 0才需要
S15~S0————————LR_RETURN的bit4 == 0才需要
xpsr
PC
LR
R12
R3~R0————以上是异常（中断）会自动压的部分
S31~S16————————LR_RETURN的bit4 == 0才需要
R11~R4
padding，为了8字节对齐
LR_RETURN————浮点寄存器压栈标志
*/

/*
@----重置线程-----------------------------------------------------------------
@功能:  初始化线程的栈，并挂上线程执行函数，新创建线程时将会调用本函数
@参数:  thread_routine，被重置的线程函数
@       vm，线程控制块指针
@返回:  初始化结束后的当前栈指针
@函数原型:void * __asm_reset_thread(void (*thread_routine)(struct event_script *),
@                                        struct  tagThreadVm  *vm)@
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __asm_reset_thread
.thumb
.thumb_func
__asm_reset_thread:
    ldr     r2,[r1,#4]          /* 取线程栈顶指针 */
    mov     r12,#0x01000000     /* xpsr的初始值 */
    ldr     r3,=__DJY_VmEngine  /* 取线程引擎指针，即线程入口的PC值 */
    stmfd   r2!,{r3,r12}        /* 存pc，xpsr */
    sub     r2,r2,#15*4         /* 后退15个寄存器,初始状态r0-r12中，除r0外均无意义， */
                                /* __vm_engine函数不返回，lr也无意义 */
                                /* 另外4字节是为了与EXC_RETURN形成栈的8字节对齐*/

    mov     r3,#0xfffffffd      /* 初始值EXC_RETURN*/
    stmfd   r2!,{r3}              /* 压栈lr，用于线程切入时读取是否需弹出fpu寄存器 */

    /* 存在r0中的thread_routine是__vm_engine的参数，切换上下文时，thread_routine将
        恢复到r0中，根据调用约定，r0的值就是__vm_engine函数的参数。 */
    str     r0,[r2,#10*4]        /* 保存 thread_routine指针到r0的位置. */
    str     r2,[r1]             /* 保存vm的当前栈指针到vm->stack中 */
    bx      lr

/*
@----复位老线程，切换到新线程-------------------------------------------------
@功能:  把old_vm的上下文重新初始化到新创建的状态，然后切换到新线程的
@       上下文中继续执行。
@       当一个在常驻内存的线程中处理的事件完成后，不能删除该线程，必须复位
@       该线程，然后切换到其他就绪线程，这时候就需要调用本函数。因此本函数在
@       被复位的线程上下文里面执行。
@参数:  thread_routine，被重置的线程函数
@       new_vm，目标线程的控制块指针
@       old_vm，被复位线程的控制块指针
@返回:  无
@函数原型:void __asm_reset_switch(void (*thread_routine)(void),
@                           struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __asm_reset_switch
.thumb
.thumb_func
__asm_reset_switch:
    ldr     sp,[r2,#4]          /* 取老线程栈顶指针 */
    mov     r12,#0x01000000     /* xpsr的初始值 */
    ldr     r11,=__DJY_VmEngine    /* 取线程引擎指针 */
    push    {r11,r12}           /* 存pc,xpsr */

    sub     sp,sp,#15*4         /* 后退14个寄存器,初始状态r0-r12中，除r0外均无意义，
                                   __vm_engine函数不返回，lr也无意义，至此，完成老线程复位*/
                                   /* 另外4字节是为了与EXC_RETURN形成栈的8字节对齐*/

    mov     r3,#0xfffffffd      /* 初始值EXC_RETURN*/
    push   {r3}            /* 压栈lr，用于线程切入时读取是否需弹出fpu寄存器 */

    mov     r3,#4
    mrs     r3,control
    orr     r3,#4
    msr     control,r3          /* 复位线程时默认无fpu*/

    str     r0,[sp,#10*4]       /* 保存 thread_routine指针到r0的位置. */
    str     sp,[r2]             /* 保存当前栈指针到old_vm->stack中 */

    ldr     sp,[r1]             /* 取得新上下文指针 */
    svc     0                   /* 切入新上下文 */
    bx      lr

/*
@----启动多事件调度-----------------------------------------------------------
@功能: 系统初始化完成后，启动多事件调度并切入第一个事件.
@参数: new_vm，首次启动的线程控制块指针
@返回: 无
@函数原型: void __asm_start_thread(struct  tagThreadVm  *new_vm)@
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __asm_start_thread
.thumb
.thumb_func
__asm_start_thread:
    ldr     r1,=msp_top
    msr     msp,r1              /* 重置主栈指针，主栈将用于异常和中断 */
    mrs     r2, control
    mov     r1,#2
    orr     r1,r2
    ldr     r0,[r0]             /* 取得新上下文指针 */
    msr     psp,r0
    msr     control,r1          /* 切换到特权线程模式，栈指针用psp */
    svc     0                   /* 切入新上下文 */
    bx      lr

/*
@----切入上下文---------------------------------------------------------------
@功能:  不保存原上下文，直接切入新的上下文执行
@参数:  new_vm，待切入的线程控制块指针
@返回:  无
@函数原型: void __asm_turnto_context(struct  tagThreadVm  *new_vm)@
@说明:  当事件完成,就没有必要保存旧事件的上下文,直接切换到新事件即可.
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __asm_turnto_context
.thumb
.thumb_func
__asm_turnto_context:
    ldr     sp,[r0]             /* 取得新上下文指针 */

    svc     0                   /* 切入新上下文 */
    bx      lr

/*
@----上下文切换---------------------------------------------------------------
@功能:  保存当前线程的上下文，切换到新线程的上下文中继续执行。
@参数:  new_vm，切入线程的控制块指针
@参数:  old_vm，切离线程的控制块指针，即&vm->stack。无需提供旧上下文栈指针，sp寄存器的当前值就是
@返回:  无
@函数原型: void __asm_switch_context(struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.text
.align 2
.global __asm_switch_context
.thumb
.thumb_func
__asm_switch_context:
    svc     1                   /* 切入新上下文 */
    bx      lr
/*
@----中断中的上下文切换-------------------------------------------------------
@功能:  保存被中断线程的上下文，切换到新线程的上下文中继续执行。本函数虽然在中
@       断服务函数(非用户用int_isr_connect函数连接的中断服务函数)中，但在ARM
@       中，却运行在svc态
@参数:  new_vm，切入线程的控制块指针
@参数:  old_vm，被中断线程的控制块指针
@返回:  无
@函数原型: void __asm_switch_context_int(struct tagThreadVm *new_vm,struct tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/

.text
.align 2
.global __asm_switch_context_int
.thumb
.thumb_func
__asm_switch_context_int:
    mrs     r2,psp      /* 取被中断线程的psp */
    str     r2,[r1]     /* 把psp写入线程数据结构中 */
    ldr     r2,[r0]     /* 取待切入线程的psp */
    msr     psp,r2      /* 待切入线程的当前栈指针写入psp，中断返回恢复上下文将以此为准 */
    bx      lr

/*
@----保存上下文---------------------------------------------------------------
@功能: 保存当前线程的上下文，但不切换,专用于进入CN_SLEEP_L3之前调用.
@参数: running_vm，当前线程的控制块指针，即&vm->stack。
@参数: SaveRamL3, 把执行过step1的内存保存到flash中.
@参数: EntrySleep, 进入低功耗.
@返回:  无
@函数原型: void __LP_BSP_AsmSaveReg(struct tagThreadVm *running_vm，
                    bool_t (*SaveRamL3)(void),
                    void (*EntrySleep)(void))
@-----------------------------------------------------------------------------
*/

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

.text
.align 2
.global __asm_bl_fun
.thumb
.thumb_func
__asm_bl_fun:
    mov   pc ,r0
    bx    lr


/*
@----汇编传参并执行函数-------------------------------------------------
@功能:
@参数:  ptab，参数列表
@      pflag，参数类型列表
@      old_vm，被复位线程的控制块指针
@返回:  无
@函数原型:extern void __asm_ExecuteCmd(union param *ptab,
@       enum param_typr *pflag,Ex_shell_func fun,u32 num);
@-----------------------------------------------------------------------------
*/

.text
.align 2
.global __asm_ExecuteCmd
.thumb
.thumb_func
__asm_ExecuteCmd:
    PUSH    {R4-R12,lr}  //保存这几个寄存器可能要用到
    sub sp ,#40
     /*通过浮点寄存器传递的参数*/
    mov r4 ,#0          //浮点寄存器计数
    mov r5 ,#0          //双精度浮点寄存器计数
    mov r6 ,#0          //参数总数计数
float_double:           //不能保障调用时没有进行浮点计算所以浮点寄存器也要压栈
   /* 4,5,6,7,8,9,10*/
    ldr    r7   ,[r1,r6] //读类型数组
    and    r7   ,r7, #255 //读类型数组
    cmp    r7   ,#9    //判断类型是否为float
    beq    fd_float
    cmp    r7   ,#10    //判断类型是否为double
    beq    fd_double
    b      fd_next

fd_float:
        lsls    r8 ,r6, #3  //根据变量计数得到参数表的偏移    (0/8/16/24/32...) lsls：左移
        ldr     r7 ,[r0,r8] //读形参值数组
        cmp r4  ,#0
        beq    fd_s0
        cmp r4  ,#1
        beq    fd_s1
        cmp r4  ,#2
        beq    fd_s2
        cmp r4  ,#3
        beq    fd_s3
        cmp r4  ,#4
        beq    fd_s4
        cmp r4  ,#5
        beq    fd_s5
        cmp r4  ,#6
        beq    fd_s6
        cmp r4  ,#7
        beq    fd_s7
        cmp r4  ,#8
        beq    fd_s8
        cmp r4  ,#9
        beq    fd_s9
        cmp r4  ,#10
        beq    fd_s10
        cmp r4  ,#11
        beq    fd_s11
        cmp r4  ,#12
        beq    fd_s12
        cmp r4  ,#13
        beq    fd_s13
        cmp r4  ,#14
        beq    fd_s14
        cmp r4  ,#15
        beq    fd_s15
        cmp r4  ,#16
        beq    fd_s16
        cmp r4  ,#17
        beq    fd_s17
        cmp r4  ,#18
        beq    fd_s18
        cmp r4  ,#19
        beq    fd_s19
    fd_s0:
        vmov.f32        s0, r7
        b fd_end
    fd_s1:
        vmov.f32        s1, r7
        b fd_end
    fd_s2:
        vmov.f32        s2, r7
        b fd_end
    fd_s3:
        vmov.f32        s3, r7
        b fd_end
    fd_s4:
        vmov.f32        s4, r7
        b fd_end
    fd_s5:
        vmov.f32       s5, r7
        b fd_end
    fd_s6:
        vmov.f32        s6, r7
        b fd_end
    fd_s7:
        vmov.f32        s7, r7
        b fd_end
    fd_s8:
        vmov.f32        s8, r7
        b fd_end
    fd_s9:
        vmov.f32        s9, r7
        b fd_end
    fd_s10:
        vmov.f32        s10, r7
        b fd_end
    fd_s11:
        vmov.f32        s9, r7
        b fd_end
    fd_s12:
        vmov.f32        s9, r7
        b fd_end
    fd_s13:
        vmov.f32        s9, r7
        b fd_end
    fd_s14:
        vmov.f32        s9, r7
        b fd_end
    fd_s15:
        vmov.f32        s9, r7
        b fd_end
    fd_s16:
        vmov.f32        s9, r7
        b fd_end
    fd_s17:
        vmov.f32        s9, r7
        b fd_end
    fd_s18:
        vmov.f32        s9, r7
        b fd_end
    fd_s19:
        vmov.f32        s9, r7
fd_end:
    add r4   ,#1
    b  fd_next
fd_double:
        lsls    r8 ,r6, #3  //根据变量计数得到参数表的偏移
        add r5 , r4,#1
        lsr r5 ,#1
        cmp r5  ,#0
        beq     fd_d0
        cmp r5  ,#1
        beq     fd_d1
        cmp r5  ,#2
        beq     fd_d2
        cmp r5  ,#3
        beq     fd_d3
        cmp r5  ,#4
        beq     fd_d4
        cmp r5  ,#5
        beq     fd_d5
        cmp r5  ,#6
        beq     fd_d6
        cmp r5  ,#7
        beq     fd_d7
        cmp r5  ,#8
        beq     fd_d8
        cmp r5  ,#9
        beq     fd_d9
        cmp r5  ,#10
        beq     fd_d10
    fd_d0:
        add      R0  ,R8
        vldr     d0, [r0, 0]
        sub      R0  ,R8
        b fd_dend
    fd_d1:
        add      R0  ,R8
        vldr     d1, [r0, 0]
        sub      R0  ,R8
        b fd_dend
    fd_d2:
        add      R0  ,R8
        vldr     d2, [r0, 0]
        sub      R0  ,R8
        b fd_dend
    fd_d3:
        add      R0  ,R8
        vldr     d3, [r0, 0]
        sub      R0  ,R8
        b fd_dend
    fd_d4:
        add      R0  ,R8
        vldr     d4, [r0, 0]
        sub      R0  ,R8
        b fd_dend
    fd_d5:
        add      R0  ,R8
        vldr     d5, [r0, 0]
        sub      R0  ,R8
        b fd_dend
    fd_d6:
        add      R0  ,R8
        vldr     d6, [r0, 0]
        sub      R0  ,R8
        b fd_dend
    fd_d7:
        add      R0  ,R8
        vldr     d7, [r0, 0]
        sub      R0  ,R8
        b fd_dend
    fd_d8:
        add      R0  ,R8
        LDR      r11,[R0]
        LDR      r12,[R0,#4]
        strd     r11, r12, [sp]
        sub      R0  ,R8
        b fd_dend
    fd_d9:
        add      R0  ,R8
        LDR      r11,[R0]
        LDR      r12,[R0,#4]
        strd     r11, r12, [sp,#8]
        sub      R0  ,R8
        b fd_dend
fd_d10:
        add      R0  ,R8
        LDR      r11,[R0]
        LDR      r12,[R0,#4]
        strd     r11, r12, [sp,#16]
        sub      R0  ,R8
    fd_dend:
        add R4  ,#3
        and r4  ,#254
fd_next:
    add    r6   ,#1
    cmp r6 ,r3
    bne float_double

/*浮点参数传递完毕通用寄存器传参*/
    MOV R4 ,#0   //寄存器传参计数器
    MOV R5 ,#0   //低16位压栈大小计数器高16位第一个压栈参数在表中的偏移
    MOV R6 ,#0   //检索参数总数计数

    mov r7 ,r0   //参数表备份
    mov r8 ,r1   //参数类型表备份
    mov r9 ,r2   //函数地址备份
    mov r10,r3   //参数个数            /* 0,1,2,3,4,5,6，11,12*/
    /*R11保存数组类型 */
    /*R12保存当前类型占字节数 */
R0_R3:
    ldr    r11   ,[r8,r6] //读类型数组
    and    r11   ,r11, #255 //读类型数组
    /*获取该类型占字节大小*/
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
    cmp    r11   ,#9  //判断类型是否为float
    beq    R_next
    cmp    r11   ,#10 //判断类型是否为double
    beq    R_next
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
        mov    r5, r6, lsl#16  //r5的高16位存放第一个压栈参数在表中的偏移，r6的值没有改变
    add_S:
        //变量压栈4字节对齐 //size =8则+8
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
            beq  R_R2
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
    R5  //低16位压栈大小计数器
        //高16位第一个压栈参数在表中的偏移
    r4 保存源sp地址用于函数返回时使用
    R5压栈偏移
    R6 //检索参数总数计数
    mov r7 ,r0   //参数表备份
    mov r8 ,r1   //参数类型表备份
    mov r9 ,r2   //函数地址备份
    mov r10,r3   //参数个数
    R11 参数类型
    R12:
*/

/*通过栈传递的参数*/
    cmp     r5 ,#0 //无需压栈
    bne    Stacks
    BLX     R9
    add sp ,#40
    POP     {R4-R12,lr}//恢复保存的寄存器
    BX      LR       //返回
Stacks:

    mov     r4 ,sp       //备份栈指针在函数返回时恢复
    LSR     r6 ,r5, #16  //得到第一个压栈参数在表中的偏移  LSR：右移
                         //取出r5高16位的值存放到r6,r5的值不变
    mov   r11 ,r10         //lsl:左移
    mov   r10 ,r11,lsl#16   //r10的高16位放参数的总个数，
    add   r10 ,#0       //r10的低16位初始值为0


    and     r5 ,r5, #255 //取出r5的低8位的值存放到r5,r5的值改变
    add     r5 ,#7
    and     r5 ,#0xfffffff8//调整指针八字节对其。
    add     r5 ,#8
    sub     sp ,r5
    mov     r5 ,#0
loop:
    ldr    r11 ,[r8,r6] //读类型数组
    and    r11   ,r11, #255 //读类型数组
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
    cmp    r11   ,#9  //判断类型是否为float
    beq    S_next
    cmp    r11   ,#10 //判断类型是否为double
    beq    S_next
    cmp    r11   ,#11
    beq    S_size_1
    cmp    r11   ,#12
    beq    S_size_4

    S_size_8:
        lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
        ldr     r11 ,[r7,r12] //读数据表
        add     r12 ,#4                  //???
        ldr     r12 ,[r7,r12] //读类型数组
        str     r11, [sp ,r5]
        add     r5,  #4
        str     r12, [sp ,r5]
        add     r5,  #4

        b S_next
    S_size_4:
    S_size_2:
    S_size_1:
        add    r6 ,#1
        LSR  r12 ,r10, #16  //从r10的高16位读参数总数放到r12
        cmp  r6 ,r12
        beq  r6_equal_r12


        ldr    r12 ,[r8,r6] //读类型数组
        and    r12   ,r12, #255 //读类型数组


        and    r11 ,r10, #255
        cmp r11 ,#0
        beq sub_r6_1
        cmp r11 ,#1
        beq sub_r6_2
        cmp r11 ,#2
        beq sub_r6_3
        cmp r11 ,#3
        beq sub_r6_4
        cmp r11 ,#4
        beq sub_r6_5
        cmp r11 ,#5
        beq sub_r6_6


sub_r6_1:
   sub    r6 ,#1
    b  cmp_r5
sub_r6_2:
   sub    r6 ,#2
   b  cmp_r5
sub_r6_3:
   sub    r6 ,#3
   b  cmp_r5
sub_r6_4:
   sub    r6 ,#4
   b  cmp_r5
sub_r6_5:
   sub    r6 ,#5
   b  cmp_r5
sub_r6_6:
   sub    r6 ,#6
   b  cmp_r5

cmp_r5:

        cmp r5 ,#0
        beq  u64_x_u64
        cmp r5 ,#8
        beq  u64_x_u64
        cmp r5 ,#16
        beq  u64_x_u64
        cmp r5 ,#24
        beq  u64_x_u64
        cmp r5 ,#32
        beq  u64_x_u64
        cmp r5 ,#40
        beq  u64_x_u64
        cmp r5 ,#48
        beq  u64_x_u64
        cmp r5 ,#56
        beq  u64_x_u64


       b    S_size_4_bak

 u64_x_u64:
        cmp    r12   ,#9          //float
        beq   float1_double1
        cmp    r12   ,#10          //double
        beq  float1_double1
        cmp    r12   ,#3   //u64
        beq  S_size_8_bak
        cmp    r12   ,#7  //s64
        beq  S_size_8_bak
        b   S_size_4_bak

float1_double1:
        add    R10    ,#1
        and    r11 ,r10, #255
        cmp r11 ,#1
        beq add_r6_1
        cmp r11 ,#2
        beq add_r6_2
                cmp r11 ,#3
        beq add_r6_3
                cmp r11 ,#4
        beq add_r6_4
                cmp r11 ,#5
        beq add_r6_5
                cmp r11 ,#6
        beq add_r6_6


add_r6_1:
 add    R6    ,#1
 b   S_size_4
add_r6_2:
 add    R6    ,#2
 b   S_size_4
 add_r6_3:
 add    R6    ,#3
 b   S_size_4
 add_r6_4:
 add    R6    ,#4
 b   S_size_4
 add_r6_5:
 add    R6    ,#5
 b   S_size_4
 add_r6_6:
 add    R6    ,#6
 b   S_size_4



r6_equal_r12:

    and    r11 ,r10, #255
                 cmp r11 ,#0
        beq sub_r6_1_bak
                cmp r11 ,#1
        beq sub_r6_2_bak
                cmp r11 ,#2
        beq sub_r6_3_bak
                cmp r11 ,#3
        beq sub_r6_4_bak
                cmp r11 ,#4
        beq sub_r6_5_bak
                cmp r11 ,#5
        beq sub_r6_6_bak


sub_r6_1_bak:
   sub    r6 ,#1
    b  S_size_4_bak
sub_r6_2_bak:
   sub    r6 ,#2
   b  S_size_4_bak
sub_r6_3_bak:
   sub    r6 ,#3
   b  S_size_4_bak
sub_r6_4_bak:
   sub    r6 ,#4
   b  S_size_4_bak
sub_r6_5_bak:
   sub    r6 ,#5
   b  S_size_4_bak
sub_r6_6_bak:
   sub    r6 ,#6
   b  S_size_4_bak

S_size_4_bak:


        lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
        ldr     r11 ,[r7,r12] //读类型数组
        str     r11 ,[sp,r5]
        add   r5,  #4
        b S_next


S_size_8_bak:

        lsls    r12 ,r6, #3  //根据变量计数得到参数表的偏移
        ldr     r11 ,[r7,r12] //读类型数组
        str     r11 ,[sp,r5]
        add   r5,  #8
        b S_next

    S_next:
    and     r10 ,#0xffffff00//r10的低8位清0
    add    r6 ,#1
    LSR  r11 ,r10, #16  //从r10的高16位读参数总数放到r11
    cmp r6  ,r11
    bne loop

call_fun:
    /*调用相应的函数*/
    BLX     R9
    mov   sp ,r4
    add sp ,#40
    POP     {R4-R12,lr}//恢复保存的寄存器
    BX      LR       //返回


.end


