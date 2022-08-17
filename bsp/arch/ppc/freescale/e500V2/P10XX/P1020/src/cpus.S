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
// ģ������:P1020CPU��һЩ���ܺ���
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա:zhangqf_cyg
// ����ʱ��:2013-7-26--����10:15:29
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ˵�������ļ�ʵ��һЩ��C�����в���ʵ�ֵĺ������ر����漰���Ĵ�������
// �����߳��л�����E500v2�ĺ��ĵ��У�������������SC�쳣��ʵ�ֵģ�
// �������ǲ�ʹ��SC�쳣���Ѹ��쳣�ͷŸ������Ҫ����ʹ�ã������ڴ�--TODO
.file   "cpus.S"

#include "E500v2Reg.h"
#include "E500v2RegBits.h"
#include "Guts.h"
/*
@-------------------------------------------------------------------------------
@����: ��λtb����ʹ�ܣ���Ҫ�ǲ���ʱʹ�ã��ð�����ʹ�õ���TBCLK��
@����:  ��()
@����:  ��
@����:  ��
@����ԭ��: void __asm_reset_the_tb(void)
@          use the tbclk
@-------------------------------------------------------------------------------
*/
//hid0---BITS 50 SEL,0 USE BUSCLK/8 to update, 1 USE  TBCLK(rtc)
//    ---BITS 49 EN, 0 DISABLE, 1 ENBAEL
.text
.global __asm_reset_the_tb
__asm_reset_the_tb:

    stwu   sp, -32(sp)
    stw    r3, 8(sp)
    stw    r4, 12(sp)
    stw    r5, 16(sp)
    msync

    mfspr  r3, hid0
    lis    r4, HID0_TBEN@h
    ori    r4, r4, HID0_TBEN@l
    andc   r3, r3, r4 //stop the tb

    isync
    msync
    mtspr  hid0, r3
    isync//OPERATE THE HIDX NEED THE SYNC OPERATION BEFORE AND POST

    xor     r5, r5, r5
    mtspr   tbl_write, r5
    isync
    mtspr   tbu_write, r5
    isync
    //enable the tb
    or      r3, r4,  r3
    isync
    msync
    mtspr  hid0, r3
    isync//OPERATE THE HIDX NEED THE SYNC OPERATION BEFORE AND POST

    lwz  r3,  8(sp)
    lwz  r4,  12(sp)
    lwz  r5,  16(sp)
    lwz  sp,  0(sp)

    blr               //return

 /*
@-------------------------------------------------------------------------------
@����: ��ȡTBʱ��
@����: value_h32, value_l32,�ֱ����ڴ洢TB�ĸ�32λ�͵�32λ
@����:  ��
@����ԭ��: void __asm_get_tb_value(u32 *value_h32, u32 *value_l32)
@-------------------------------------------------------------------------------
*/
.text
.global __asm_get_tb_value
__asm_get_tb_value:

    stwu   sp, -32(sp)
    stw    r6, 8(sp)
    stw    r7,12(sp)
    stw    r8,16(sp)
    msync

 get_tb_loop:
    mfspr   r6, tbu_read
    mfspr   r7, tbl_read
    mfspr   r8, tbu_read
    cmplw   r6, r8
    bgt     get_tb_loop  //������λ���ٶ�һ����

    stw     r6, 0(r3)
    stw     r7, 0(r4)

    lwz    r6, 8(sp)
    lwz    r7, 12(sp)
    lwz    r8, 16(sp)
    lwz    sp, 0(sp)

    //��ȡTBʱ�䲻Ӧ��ͣ������Ȼ��ʱ��Ҫ�����쳣��������ߣ�TICK�����������ϵͳ�ᷢ��
    //̱��
    blr               //return
 /*
@-------------------------------------------------------------------------------
@����: ��ʼ����������
@����: value_tick
@����:  ��
@����ԭ��: void __asm_init_tick(u32 value_tick)
@-------------------------------------------------------------------------------
*/
.text
.global __asm_init_Dtimer
__asm_init_Dtimer:

    stwu  sp, -32(sp)
    stw   r4, 8(sp)
    stw   r5, 12(sp)
    stw   r6, 16(sp)
    stw   r7, 20(sp)
    stw   r8, 24(sp)
    msync

    //diable the tb
    mfspr r6, hid0
    lis   r7, HID0_TBEN@h
    ori   r7, r7, HID0_TBEN@l
    andc  r6, r6, r7

    isync
    msync
    mtspr  hid0, r6
    isync//OPERATE THE HIDX NEED THE SYNC OPERATION BEFORE AND POST

    //set the tick value
    //for the test,make the counter bigger
    mtspr  dec, r3
    isync
    mtspr  decar, r3
    isync

    //no need to set the tcr,the vm will do it itself
    //just enable reload, and tick int will be dealed in asyn signal
    lis    r3,  CN_TCR_DEFAULT@h  //bit37 IE  bit41 ARE
    ori    r3, r3, CN_TCR_DEFAULT@l
    mfspr  r4, tcr
    or     r4, r4, r3
    mtspr  tcr, r4
    isync

    mfspr   r6, hid0
    or      r6, r6, r7
    isync
    msync
    mtspr  hid0, r6
    isync             //OPERATE THE HIDX NEED THE SYNC OPERATION BEFORE AND POST

    lwz   r4, 8(sp)
    lwz   r5, 12(sp)
    lwz   r6, 16(sp)
    lwz   r7, 20(sp)
    lwz   r8, 24(sp)
    lwz   sp, 0(sp)

    blr               //return
/*
@-------------------------------------------------------------------------------
@����:��ȡMC��ؼĴ���,���ڵ������
@����:�洢��ַ
@����:��
@����ԭ��:
@-------------------------------------------------------------------------------
*/
.global __asm_get_mc_registers
__asm_get_mc_registers:
    mfspr r4, mcsrr0
    stw   r4, 0(r3)
    mfspr r4, mcsrr1
    stw   r4, 0x04(r3)
    mfspr r4, mcsr
    stw   r4, 0x08(r3)
    mfspr r4, mcar
    stw   r4, 0x0c(r3)
    mfspr r4, hid0
    stw   r4, 0x10(r3)
    mfspr r4, hid1
    stw   r4, 0x14(r3)
    isync
    msync
    blr

/*
@-------------------------------------------------------------------------------
@����:��࣬��������
@����:void
@����:void
@����ԭ��: void __asm_reboot(void )
@˵�������������������������汾���λ�ã���__check_boot_version
@      �ǵ�windowװϵͳ��ʱ��һֱ����ĳһ�����ܼ���Ȼ����뵽ѡ������λ�ý��棬
@      �����������һ��ѡ����һ��ϵͳ�����ĵط�����ô˵�������˰�
@-------------------------------------------------------------------------------
*/
.global CPU_Reboot
reboot:
    isync
    msync
    wrteei 0x00  //must disable all the unpredict interrupts
    lis r3,  __AsmRamStart@h
    ori r3, r3, __AsmRamStart@l
    mtlr  r3
    isync
    dcbst r0, r3
    msync
    icbi  r0, r3
    msync
    isync
    blr
    b .     //�����ܵ����λ��
    blr     //���ֳ����һ����
/*
@-------------------------------------------------------------------------------
@����:��࣬��λ
@����:void
@����:void
@����ԭ��: void __asm_reset(void )
@˵�����Ǹ����ǰ������ǵ�power�����Ǹ�ʲô��������Ǹ�Ч���������㶮��
@-------------------------------------------------------------------------------
*/
.global CPU_Reset
reset:
    isync
    msync
    wrteei 0x00  //must disable all the unpredict interrupts
//����ȫ�ָ�λģ����и�λ
    lis r3, cn_p1020_guts_rstcr@h
    ori r3, r3, cn_p1020_guts_rstcr@l
    li  r4, cn_p1020_guts_rstcr_hardreset_r
    stw r4, 0(r3)

    msync
    isync
    lwz r4, 0(r3)
    isync
    msync    //OPERATE THE CCSRBAR REGISTER NEED SOME SYNC OPERATION:SYNC AND READ
    b   .  //hung here will do some wait until the power reset
    blr
/*
@-------------------------------------------------------------------------------
@����:��࣬��λ
@����:void
@����:void
@����ԭ��: void __asm_reset(void )
@˵�����Ǹ����ǰ������ǵ�power�����Ǹ�ʲô��������Ǹ�Ч���������㶮��
@-------------------------------------------------------------------------------
*/
.global CPU_RestartSystem
restart_system:
    isync
    msync
    wrteei 0x00  //must disable all the unpredict interrupts
//����ȫ�ָ�λģ����и�λ
    lis r3, cn_p1020_guts_rstcr@h
    ori r3, r3, cn_p1020_guts_rstcr@l
    li  r4, cn_p1020_guts_rstcr_hardreset_r
    stw r4, 0(r3)

    msync
    isync
    lwz r4, 0(r3)
    isync
    msync    //OPERATE THE CCSRBAR REGISTER NEED SOME SYNC OPERATION:SYNC AND READ
    b   .  //hung here will do some wait until the power reset
    blr
/*
@-------------------------------------------------------------------------------
@function:�ҳ�U32�е�һ��Ϊ1��bitλ���Ӹ�λ����λѰ�ң�
@para:para,������������
@return: r3 ���ض�ȡ������
@function modle:unsigned char   __asm_find_f1bits(u32 para)
@-------------------------------------------------------------------------------
*/
.global __asm_find_f1bits
__asm_find_f1bits:
    cntlzw  r3, r3
    blr

/*
@-------------------------------------------------------------------------------
@function:��CCSR�Ĵ�����ȡ���ݻ��� д�����ݣ�8 16 32��
@para:��R3���ݵ�ַ�ͷ��ؽ��������У���R4�������ݣ�д���ʱ��
@return:
@-------------------------------------------------------------------------------
*/
.global  read8
read8:
    isync
    msync
    lbz  r3, 0(r3)
    isync
    msync
    blr
.global  read16
read16:
    isync
    msync
    lhz  r3, 0(r3)
    isync
    msync
    blr
.global  read32
read32:
    isync
    msync
    lwz  r3, 0(r3)
    isync
    msync
    blr

.global  write8
write8:
    isync
    msync
    stb  r4, 0(r3)
    isync
    msync
    blr
.global  write16
write16:
    isync
    msync
    sth  r4, 0(r3)
    isync
    msync
    blr
.global  write32
write32:
    isync
    msync
    stw  r4, 0(r3)
    isync
    msync
    blr

/*
@-------------------------------------------------------------------------------
@function:set the mas3 property of assigned tlb by the address
@para:property: to set to mas3, address, the tlb block in the address
@return:NULL
@function modle:void __AsmSetTlbMas3Pro(u32 property, u32 address)
@-------------------------------------------------------------------------------
*/
.global  __AsmSetTlbMas3Pro
__AsmSetTlbMas3Pro:

    stwu  sp, -16(sp)
    stw   r5,  4(sp)
    msync

    xor  r5, r5, r5
    mtspr   MAS6, r5    /* Assume the current PID and AS are 0 */
    isync
    msync
    tlbsx   0, r4       /* This must succeed */
    isync
    //set to the property we need
    mtspr   MAS3, r3
    isync
    msync
    tlbwe
    isync
    msync

    lwz r5, 4(sp)
    lwz sp, 0(sp)
    blr
/*
@-------------------------------------------------------------------------------
@function:get the mas3 property of assigned tlb by address
@para:address, used to assign the tlb
@return:the property of mas3
@function modle:u32 __AsmGetTlbMas3Pro( u32 address)
@-------------------------------------------------------------------------------
*/
.global  __AsmGetTlbMas3Pro
__AsmGetTlbMas3Pro:

    stwu  sp, -16(sp)
    stw   r4,  4(sp)
    msync
    isync

    xor r4, r4, r4
    mtspr   MAS6, r4    /* Assume the current PID and AS are 0 */
    isync
    msync
    tlbsx   0, r3      /* This must succeed */
    isync
    //get to the property we need
    mfspr   r3, MAS3
    isync

    lwz r4, 4(sp)
    lwz sp, 0(sp)
    blr

.end
/****************************file end*********************************/