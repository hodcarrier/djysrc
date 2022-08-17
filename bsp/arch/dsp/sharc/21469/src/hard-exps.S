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

.file_attr libGroup="startup";
.file_attr libName="libc";
.file_attr libFunc="___lib_prog_term";
.file_attr FuncName="___lib_prog_term";
#if defined(_ADI_THREADS)
.file_attr libFunc="___lib_start";
.file_attr FuncName="___lib_start";
#endif
.file_attr prefersMem="any";
.file_attr prefersMemNum="50";

#include "sig_glob.h"
#include "lib_glob.h"
#include "cpu_peri_int_line.h"
.global ___lib_RSTI;

.extern _adi_stack_overflowed;
.extern ___real_interrupt_handler;
.extern ___asyn_interrupt_handler;
.extern _pre_start;
.extern _main;

#ifdef ARGV_SUPPORT
.extern __getargv;
.extern __Argv;
#endif
#ifdef __cplusplus
.EXTERN  __ctors;        // CPP: this id is defined in ldf file
                         // which points to start of seg_ctdm section
.EXTERN  __ctorsize;
.SECTION/DM .gdt;
.GLOBAL  ___eh_gdt;      /* gdt section start marker. Needs to be in CRT. */
.VAR     ___eh_gdt[1];
#endif
#ifdef PROFILER_SUPPORT
.extern monstartup;
#endif


.GLOBAL  ___lib_prog_term;          // Termination address
.EXTERN  ___lib_setup_c;

#if defined(_ADI_THREADS)
.GLOBAL  ___lib_start;
#endif

#if !defined(_ADI_THREADS)
// The interrupt vector table is not included in 'mt' variants of the CRT.
// Those are used with VDK, which defines its own interrupt vector table.
// The 'mt' variants of the CRT are not used in the default LDF.

.EXTERN ___nop_interrupt;
.EXTERN ___push_interrupt;
.EXTERN ___interrupt_handler;
.EXTERN ___sys_general_exception;
.EXTERN ___sys_HardStack_exception;
.EXTERN ___sys_COMP_exception;
.EXTERN ___sys_SPORT_exception;
.EXTERN ___sys_Debug_exception;
.EXTERN ___second_stage_reboot;

#define  Monitor_reboot \
   bit clr MODE1 0x1000;                /* Disable interrupt           */ \
   NOP;                                 /* Save R2                     */ \
   JUMP ___second_stage_reboot;     /* jmp to finish setting up    */ \
   NOP;                                 /* Base of int table           */

#define  REAL_INT(irp) \
   bit clr MODE1 0x1000;                /* Disable interrupt           */ \
   JUMP ___real_interrupt_handler(DB);  /* jmp to finish setting up    */ \
   DM(0,I7)=R2;                         /* Save R2                     */ \
   R2=irp;                              /* Base of int table           */

#define  PUSH_INT(irp) \
   bit set MODE1 0x04fc;                /* REG Mode Change             */ \
   JUMP ___push_interrupt(DB);          /* jmp to finish setting up    */ \
   bit clr MODE1 0x1000;                /* must Disable int            */ \
   R2=irp;                              /* Base of int table           */

#define  NOP_INT(irp) \
   bit set MODE1 0x04fc;                /* REG Mode Change             */ \
   JUMP ___nop_interrupt(DB);           /* jmp to finish setting up    */ \
   bit clr MODE1 0x1000;                /* must Disable int            */ \
   R2=irp;                              /* Base of int table           */

#define  Memory_EXCEPTION(irp) \
   NOP;                                 /* nop                         */ \
   JUMP ___sys_general_exception(DB);               /* nop             */ \
   NOP;                                 /* nop                         */ \
   R2=irp;                                  /* nop           */


#define  HardStack_EXCEPTION(irp) \
   NOP;                                 /* nop                         */ \
   JUMP ___sys_HardStack_exception(DB);                 /* nop             */ \
   NOP;                                 /* nop                         */ \
   R2=irp;                                  /* nop           */

#define  COMP_EXCEPTION(irp) \
   NOP;                                 /* nop                         */ \
   JUMP ___sys_COMP_exception(DB);              /* nop             */ \
   NOP;                                 /* nop                         */ \
   R2=irp;                                  /* nop           */

#define  SPORT_EXCEPTION(irp) \
   NOP;                                 /* nop                         */ \
   JUMP ___sys_SPORT_exception(DB);                 /* nop             */ \
   NOP;                                 /* nop                         */ \
   R2=irp;                                  /* nop           */

#define  EXCEPTION(irp) \
   NOP;                                 /* nop                         */ \
   JUMP ___lib_prog_term;               /* nop                         */ \
   NOP;                                 /* nop                         */ \
   NOP;                                 /* nop                         */

#define RESERVED_INTERRUPT NOP;NOP;NOP;NOP

// The default interrupt vector code is defined in section seg_rth
#if defined (__SHORT_WORD_CODE__)
// This code has to be mapped to a normal word section so make sure
// that happens even when building with -swc or default options.
.SECTION/PM/NW         seg_rth;
#else
// Otherwise we don't need any section specifier
.SECTION/PM            seg_rth;
#endif

EMUI:
            RESERVED_INTERRUPT;

___lib_RSTI:         NOP;                   // Not really executed
              JUMP ___lib_start;
              NOP;
              NOP;
___lib_RSTI.end:
IICDI:        Memory_EXCEPTION(cn_int_line_IICDI);    // Access to illegal IOP space

SOVFI:        HardStack_EXCEPTION(cn_int_line_SOVFI);     // status/loop/PC stack overflow

TMZHI:        NOP_INT(cn_int_line_TMZHI);     // high priority timer

SPERRI:       SPORT_EXCEPTION(cn_int_line_SPERRI); // SPORT Error interrupt

BKPI:         EXCEPTION(cn_int_line_BKPI);      // Hardware breakpoint interrupt

            RESERVED_INTERRUPT;

IRQ2I:        REAL_INT(cn_int_line_IRQ2I);

IRQ1I:        Monitor_reboot;

IRQ0I:        NOP_INT(cn_int_line_IRQ0I);

P0I:          PUSH_INT(cn_int_line_DAII);       // Peripheral interrupt 0

P1I:          PUSH_INT(cn_int_line_SPII);       // Peripheral interrupt 1

P2I:          PUSH_INT(cn_int_line_GPTMR0I);       // Peripheral interrupt 2

P3I:          PUSH_INT(cn_int_line_SP1I);       // Peripheral interrupt 3

P4I:          PUSH_INT(cn_int_line_SP3I);       // Peripheral interrupt 4

P5I:          PUSH_INT(cn_int_line_SP5I);       // Peripheral interrupt 5

P6I:          PUSH_INT(cn_int_line_SP0I);       // Peripheral interrupt 6

P7I:          PUSH_INT(cn_int_line_SP2I);       // Peripheral interrupt 7

P8I:          PUSH_INT(cn_int_line_SP4I);       // Peripheral interrupt 8

P9I:          PUSH_INT(cn_int_line_EPDM0I);       // Peripheral interrupt 9

P10I:         PUSH_INT(cn_int_line_GPTMR1I);      // Peripheral interrupt 10

P11I:         PUSH_INT(cn_int_line_LP0I);      // Peripheral interrupt 11

P12I:         PUSH_INT(cn_int_line_UARTI);      // Peripheral interrupt 12

P13I:         PUSH_INT(cn_int_line_EPDM1I);      // Peripheral interrupt 13

P14I:         PUSH_INT(cn_int_line_DPII);      // Peripheral interrupt 14

P15I:         PUSH_INT(cn_int_line_MTMI);      // Peripheral interrupt 15

P16I:         PUSH_INT(cn_int_line_LP1I);      // Peripheral interrupt 16

P17I:         PUSH_INT(cn_int_line_P17I);      // Peripheral interrupt 17

P18I:         PUSH_INT(cn_int_line_TWII);      // Peripheral interrupt 18

CB7I:         Memory_EXCEPTION(cn_int_line_CB7I);  // C run-time stack overflow

CB15I:        Memory_EXCEPTION(cn_int_line_CB15I);     // circular buffer #15 overflow

TMZLI:        NOP_INT(cn_int_line_TMZLI);      // low priority timer

FIXI:         COMP_EXCEPTION(cn_int_line_FIXI);      // fixed point overflow

FLTOI:        COMP_EXCEPTION(cn_int_line_FLTOI);     // floating point overflow

FLTUI:        COMP_EXCEPTION(cn_int_line_FLTUI);     // floating point underflow

FLTII:        COMP_EXCEPTION(cn_int_line_FLTII);     // floating point invalid

EMULI:        EXCEPTION(cn_int_line_EMULI);     // Emulator low priority interrupt

SFT0I:        PUSH_INT(cn_int_line_SFT0I);     // user interrupts 0..3

SFT1I:        PUSH_INT(cn_int_line_SFT1I);

SFT2I:        PUSH_INT(cn_int_line_SFT2I);

SFT3I:        PUSH_INT(cn_int_line_SFT3I);

            RESERVED_INTERRUPT;

#else /* _ADI_THREADS */

.section/pm seg_pmco;

#endif /* _ADI_THREADS */

___lib_start:
            CALL ___lib_setup_c;            /* Setup C runtime model */

#ifdef __cplusplus
# ifndef ARGV_SUPPORT
         put(r4);
         put(r8);
         put(r12);
# endif

// _lib_call_ctors is the added code to support the constructor calls
// before the main call. if we are using __ctorsize, then we have to change
// this code.

         i0 = __ctors;
_lib_call_ctors:
         r0 = dm(i0,m6);      // get the address of constructor function.
         r0 = pass r0;
         if eq jump _lib_call_ctors_exit; // check if null
         i13=r0;
// next 5 lines are for call of the constructor
         r2 = i6;
         i6 = i7;
         jump(m13,i13) (db);
         dm(i7,m7)=r2;
         dm(i7,m7)=pc;
         jump _lib_call_ctors;
_lib_call_ctors_exit:

# ifndef ARGV_SUPPORT
         get(r12,1);    // Restore the param regs
         get(r8,2);
         get(r4,3);
         modify(i7,3);
# endif
#endif // __cplusplus

#ifdef PROFILER_SUPPORT
     // Profiler support
         r2 = i6;
         i6 = i7;
         JUMP  monstartup (DB);
         dm(i7,m7)=r2;
         dm(i7,m7)=pc;
#endif

#ifdef ARGV_SUPPORT
         // Initialise argc and argv for the call to main()
         R2=I6;
         I6=I7;
         JUMP __getargv (DB); DM(I7,M7)=R2; DM(I7,M7)=PC;
         R4 = R0;
         R8 =  __Argv;
#endif

// **IMPORTANT**  The compiler assumes that i6-2==i7 when main starts. (i.e.
// the stack pointer, i7 has been decremented by two from it's first value,
// NOT that the frame pointer i6 has been incremented by 2).  This normally
// happens in ___lib_setup_args, but if you change this startup file,
//  ___lib_setup_args might not be called. In this case, add a
// MODIFY(i7,-2) instruction before _pre_start is called

#ifdef MAIN_RTS
            CJUMP _pre_start (DB);             // Begin C program
//            CJUMP _main (DB);             // Begin C program
            DM(I7,M7)=R2;
            DM(I7,M7)=PC;
#else
            JUMP _pre_start;                   // Begin C program
//            JUMP _main;                   // Begin C program
#endif
.___lib_start.end:

___lib_prog_term:
            IDLE;
            JUMP ___lib_prog_term;        // Stay put
.___lib_prog_term.end:

.ENDSEG;

#ifdef _ADI_SWFA
.SECTION/PM       seg_pmda;
.GLOBAL __21160_anomaly_write_location;
.VAR __21160_anomaly_write_location;      // A memory location we can write to
                                          // to flush the FIFO.
#endif
.ENDSEG;