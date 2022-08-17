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
//所属模块: uart驱动程序
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:
//其他说明:
//修订历史:
//1. 日期:2009-03-10
//   作者:  罗侍田.
//   新版本号：1.0.0
//   修改说明: 移植字44b0的1.0.1版
//------------------------------------------------------
#ifndef __CPU_PERI_UART_H__
#define __CPU_PERI_UART_H__

#include "os.h"
#include "cpu_peri.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct         //offset
{
    vu32 UART_CR;       //控制寄存器
    vu32 UART_MR;       //模式寄存器
    vu32 UART_IER;      //中断使能寄存器
    vu32 UART_IDR;      //中断除能寄存器
    vu32 UART_IMR;      //中断屏幕寄存器
    vu32 UART_SR;
    vu32 UART_RHR;
    vu32 UART_THR;
    vu32 UART_BRGR;
    vu32 Reserved1[55];
    vu32 UART_RPR;      /**< \brief (Uart Offset: 0x100) Receive Pointer Register */
    vu32 UART_RCR;      /**< \brief (Uart Offset: 0x104) Receive Counter Register */
    vu32 UART_TPR;      /**< \brief (Uart Offset: 0x108) Transmit Pointer Register */
    vu32 UART_TCR;      /**< \brief (Uart Offset: 0x10C) Transmit Counter Register */
    vu32 UART_RNPR;     /**< \brief (Uart Offset: 0x110) Receive Next Pointer Register */
    vu32 UART_RNCR;     /**< \brief (Uart Offset: 0x114) Receive Next Counter Register */
    vu32 UART_TNPR;     /**< \brief (Uart Offset: 0x118) Transmit Next Pointer Register */
    vu32 UART_TNCR;     /**< \brief (Uart Offset: 0x11C) Transmit Next Counter Register */
    vu32 UART_PTCR;     /**< \brief (Uart Offset: 0x120) Transfer Control Register */
    vu32 UART_PTSR;
}tagUartReg;

#define cn_dma_unused   0
#define cn_dma_used     1

#define CN_UART0        0
#define CN_UART1        1
#define CN_USART0       2
#define CN_USART1       3
#define CN_UART_MAX     3
#define CN_UART_NUM     4

ptu32_t ModuleInstall_UART(ptu32_t para);//串口初始化
u32 UART_SendServiceDirectly(char *str);

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_UART_H__


