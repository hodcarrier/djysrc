//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms with or without
// modification are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT INDIRECT INCIDENTAL SPECIAL EXEMPLARY OR
// CONSEQUENTIAL DAMAGES (INCLUDING BUT NOT LIMITED TO PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE DATA OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY WHETHER IN
// CONTRACT STRICT LIABILITY OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE EVEN IF ADVISED OF THE
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
//所属模块:中断模块
//作者:lst
//版本：V1.0.0
//文件描述: 定义中断号
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-01-20
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef CPU_PERI_INT_LINE_H_
#define CPU_PERI_INT_LINE_H_

#ifdef __cplusplus
extern "C" {
#endif

/******  Cortex-M Processor Exceptions Numbers *****************************************************************/
//#define  CN_INT_LINE_NonMaskableInt           -14     /*!< 2 Non Maskable Interrupt                                          */
//#define  CN_INT_LINE_HardFault                -13     /*!< 4 Cortex-M Memory Management Interrupt                            */
//#define  CN_INT_LINE_MemoryManagement         -12     /*!< 4 Cortex-M Memory Management Interrupt                            */
//#define  CN_INT_LINE_BusFault                 -11     /*!< 5 Cortex-M Bus Fault Interrupt                                    */
//#define  CN_INT_LINE_UsageFault               -10     /*!< 6 Cortex-M Usage Fault Interrupt                                  */
//#define  CN_INT_LINE_SVCall                   -5      /*!< 11 Cortex-M SV Call Interrupt                                     */
//#define  CN_INT_LINE_DebugMonitor             -4      /*!< 12 Cortex-M Debug Monitor Interrupt                               */
//#define  CN_INT_LINE_PendSV                   -2      /*!< 14 Cortex-M Pend SV Interrupt                                     */
//#define  CN_INT_LINE_SysTick                  -1      /*!< 15 Cortex-M System Tick Interrupt                                 */
///////******  STM32 specific Interrupt Numbers **********************************************************************/
#define  CN_INT_LINE_WWDG                     0       /*!< Window WatchDog Interrupt                                         */
#define  CN_INT_LINE_PVD_AVD                  1       /*!< PVD/AVD through EXTI Line detection Interrupt                     */
#define  CN_INT_LINE_TAMP_STAMP               2       /*!< Tamper and TimeStamp interrupts through the EXTI line             */
#define  CN_INT_LINE_RTC_WKUP                 3       /*!< RTC Wakeup interrupt through the EXTI line                        */
#define  CN_INT_LINE_FLASH                    4       /*!< FLASH global Interrupt                                            */
#define  CN_INT_LINE_RCC                      5       /*!< RCC global Interrupt                                              */
#define  CN_INT_LINE_EXTI0                    6       /*!< EXTI Line0 Interrupt                                              */
#define  CN_INT_LINE_EXTI1                    7       /*!< EXTI Line1 Interrupt                                              */
#define  CN_INT_LINE_EXTI2                    8       /*!< EXTI Line2 Interrupt                                              */
#define  CN_INT_LINE_EXTI3                    9       /*!< EXTI Line3 Interrupt                                              */
#define  CN_INT_LINE_EXTI4                    10      /*!< EXTI Line4 Interrupt                                              */
#define  CN_INT_LINE_DMA1_Stream0             11      /*!< DMA1 Stream 0 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Stream1             12      /*!< DMA1 Stream 1 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Stream2             13      /*!< DMA1 Stream 2 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Stream3             14      /*!< DMA1 Stream 3 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Stream4             15      /*!< DMA1 Stream 4 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Stream5             16      /*!< DMA1 Stream 5 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Stream6             17      /*!< DMA1 Stream 6 global Interrupt                                    */
#define  CN_INT_LINE_ADC                      18      /*!< ADC1 and  ADC2 global Interrupts                                  */
#define  CN_INT_LINE_FDCAN1_IT0               19      /*!< FDCAN1 Interrupt line 0                                           */
#define  CN_INT_LINE_FDCAN2_IT0               20      /*!< FDCAN2 Interrupt line 0                                           */
#define  CN_INT_LINE_FDCAN1_IT1               21      /*!< FDCAN1 Interrupt line 1                                           */
#define  CN_INT_LINE_FDCAN2_IT1               22      /*!< FDCAN2 Interrupt line 1                                           */
#define  CN_INT_LINE_EXTI9_5                  23      /*!< External Line[9:5] Interrupts                                     */
#define  CN_INT_LINE_TIM1_BRK                 24      /*!< TIM1 Break Interrupt                                              */
#define  CN_INT_LINE_TIM1_UP                  25      /*!< TIM1 Update Interrupt                                             */
#define  CN_INT_LINE_TIM1_TRG_COM             26      /*!< TIM1 Trigger and Commutation Interrupt                            */
#define  CN_INT_LINE_TIM1_CC                  27      /*!< TIM1 Capture Compare Interrupt                                    */
#define  CN_INT_LINE_TIM2                     28      /*!< TIM2 global Interrupt                                             */
#define  CN_INT_LINE_TIM3                     29      /*!< TIM3 global Interrupt                                             */
#define  CN_INT_LINE_TIM4                     30      /*!< TIM4 global Interrupt                                             */
#define  CN_INT_LINE_I2C1_EV                  31      /*!< I2C1 Event Interrupt                                              */
#define  CN_INT_LINE_I2C1_ER                  32      /*!< I2C1 Error Interrupt                                              */
#define  CN_INT_LINE_I2C2_EV                  33      /*!< I2C2 Event Interrupt                                              */
#define  CN_INT_LINE_I2C2_ER                  34      /*!< I2C2 Error Interrupt                                              */
#define  CN_INT_LINE_SPI1                     35      /*!< SPI1 global Interrupt                                             */
#define  CN_INT_LINE_SPI2                     36      /*!< SPI2 global Interrupt                                             */
#define  CN_INT_LINE_USART1                   37      /*!< USART1 global Interrupt                                           */
#define  CN_INT_LINE_USART2                   38      /*!< USART2 global Interrupt                                           */
#define  CN_INT_LINE_USART3                   39      /*!< USART3 global Interrupt                                           */
#define  CN_INT_LINE_EXTI15_10                40      /*!< External Line[15:10] Interrupts                                   */
#define  CN_INT_LINE_RTC_Alarm                41      /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
#define  CN_INT_LINE_TIM8_BRK_TIM12           43      /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
#define  CN_INT_LINE_TIM8_UP_TIM13            44      /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
#define  CN_INT_LINE_TIM8_TRG_COM_TIM14       45      /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
#define  CN_INT_LINE_TIM8_CC                  46      /*!< TIM8 Capture Compare Interrupt                                    */
#define  CN_INT_LINE_DMA1_Stream7             47      /*!< DMA1 Stream7 Interrupt                                            */
#define  CN_INT_LINE_FMC                      48      /*!< FMC global Interrupt                                              */
#define  CN_INT_LINE_SDMMC1                   49      /*!< SDMMC1 global Interrupt                                           */
#define  CN_INT_LINE_TIM5                     50      /*!< TIM5 global Interrupt                                             */
#define  CN_INT_LINE_SPI3                     51      /*!< SPI3 global Interrupt                                             */
#define  CN_INT_LINE_UART4                    52      /*!< UART4 global Interrupt                                            */
#define  CN_INT_LINE_UART5                    53      /*!< UART5 global Interrupt                                            */
#define  CN_INT_LINE_TIM6_DAC                 54      /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
#define  CN_INT_LINE_TIM7                     55      /*!< TIM7 global interrupt                                             */
#define  CN_INT_LINE_DMA2_Stream0             56      /*!<   DMA2 Stream 0 global Interrupt                                  */
#define  CN_INT_LINE_DMA2_Stream1             57      /*!<   DMA2 Stream 1 global Interrupt                                  */
#define  CN_INT_LINE_DMA2_Stream2             58      /*!<   DMA2 Stream 2 global Interrupt                                  */
#define  CN_INT_LINE_DMA2_Stream3             59      /*!<   DMA2 Stream 3 global Interrupt                                  */
#define  CN_INT_LINE_DMA2_Stream4             60      /*!<   DMA2 Stream 4 global Interrupt                                  */
#define  CN_INT_LINE_ETH                      61      /*!< Ethernet global Interrupt                                         */
#define  CN_INT_LINE_ETH_WKUP                 62      /*!< Ethernet Wakeup through EXTI line Interrupt                       */
#define  CN_INT_LINE_FDCAN_CAL                63      /*!< FDCAN Calibration unit Interrupt                                  */
#define  CN_INT_LINE_DMA2_Stream5             68      /*!< DMA2 Stream 5 global interrupt                                    */
#define  CN_INT_LINE_DMA2_Stream6             69      /*!< DMA2 Stream 6 global interrupt                                    */
#define  CN_INT_LINE_DMA2_Stream7             70      /*!< DMA2 Stream 7 global interrupt                                    */
#define  CN_INT_LINE_USART6                   71      /*!< USART6 global interrupt                                           */
#define  CN_INT_LINE_I2C3_EV                  72      /*!< I2C3 event interrupt                                              */
#define  CN_INT_LINE_I2C3_ER                  73      /*!< I2C3 error interrupt                                              */
#define  CN_INT_LINE_OTG_HS_EP1_OUT           74      /*!< USB OTG HS End Point 1 Out global interrupt                       */
#define  CN_INT_LINE_OTG_HS_EP1_IN            75      /*!< USB OTG HS End Point 1 In global interrupt                        */
#define  CN_INT_LINE_OTG_HS_WKUP              76      /*!< USB OTG HS Wakeup through EXTI interrupt                          */
#define  CN_INT_LINE_OTG_HS                   77      /*!< USB OTG HS global interrupt                                       */
#define  CN_INT_LINE_DCMI                     78      /*!< DCMI global interrupt                                             */
#define  CN_INT_LINE_CRYP                     79      /*!< CRYP crypto global interrupt                                      */
#define  CN_INT_LINE_HASH_RNG                 80      /*!< Hash and Rng global interrupt                                     */
#define  CN_INT_LINE_FPU                      81      /*!< FPU global interrupt                                              */
#define  CN_INT_LINE_UART7                    82      /*!< UART7 global interrupt                                            */
#define  CN_INT_LINE_UART8                    83      /*!< UART8 global interrupt                                            */
#define  CN_INT_LINE_SPI4                     84      /*!< SPI4 global Interrupt                                             */
#define  CN_INT_LINE_SPI5                     85      /*!< SPI5 global Interrupt                                             */
#define  CN_INT_LINE_SPI6                     86      /*!< SPI6 global Interrupt                                             */
#define  CN_INT_LINE_SAI1                     87      /*!< SAI1 global Interrupt                                             */
#define  CN_INT_LINE_LTDC                     88      /*!< LTDC global Interrupt                                             */
#define  CN_INT_LINE_LTDC_ER                  89      /*!< LTDC Error global Interrupt                                       */
#define  CN_INT_LINE_DMA2D                    90      /*!< DMA2D global Interrupt                                            */
#define  CN_INT_LINE_SAI2                     91      /*!< SAI2 global Interrupt                                             */
#define  CN_INT_LINE_QUADSPI                  92      /*!< Quad SPI global interrupt                                         */
#define  CN_INT_LINE_LPTIM1                   93      /*!< LP TIM1 interrupt                                                 */
#define  CN_INT_LINE_CEC                      94      /*!< HDMI-CEC global Interrupt                                         */
#define  CN_INT_LINE_I2C4_EV                  95      /*!< I2C4 Event Interrupt                                              */
#define  CN_INT_LINE_I2C4_ER                  96      /*!< I2C4 Error Interrupt                                              */
#define  CN_INT_LINE_SPDIF_RX                 97      /*!< SPDIF-RX global Interrupt                                         */
#define  CN_INT_LINE_OTG_FS_EP1_OUT           98      /*!< USB OTG HS2 global interrupt                                      */
#define  CN_INT_LINE_OTG_FS_EP1_IN            99      /*!< USB OTG HS2 End Point 1 Out global interrupt                      */
#define  CN_INT_LINE_OTG_FS_WKUP              100     /*!< USB OTG HS2 End Point 1 In global interrupt                       */
#define  CN_INT_LINE_OTG_FS                   101     /*!< USB OTG HS2 Wakeup through EXTI interrupt                         */
#define  CN_INT_LINE_DMAMUX1_OVR              102     /*!<DMAMUX1 Overrun interrupt                                          */
#define  CN_INT_LINE_HRTIM1_Master            103     /*!< HRTIM Master Timer global Interrupts                              */
#define  CN_INT_LINE_HRTIM1_TIMA              104     /*!< HRTIM Timer A global Interrupt                                    */
#define  CN_INT_LINE_HRTIM1_TIMB              105     /*!< HRTIM Timer B global Interrupt                                    */
#define  CN_INT_LINE_HRTIM1_TIMC              106     /*!< HRTIM Timer C global Interrupt                                    */
#define  CN_INT_LINE_HRTIM1_TIMD              107     /*!< HRTIM Timer D global Interrupt                                    */
#define  CN_INT_LINE_HRTIM1_TIME              108     /*!< HRTIM Timer E global Interrupt                                    */
#define  CN_INT_LINE_HRTIM1_FLT               109     /*!< HRTIM Fault global Interrupt                                      */
#define  CN_INT_LINE_DFSDM1_FLT0              110     /*!<DFSDM Filter1 Interrupt                                            */
#define  CN_INT_LINE_DFSDM1_FLT1              111     /*!<DFSDM Filter2 Interrupt                                            */
#define  CN_INT_LINE_DFSDM1_FLT2              112     /*!<DFSDM Filter3 Interrupt                                            */
#define  CN_INT_LINE_DFSDM1_FLT3              113     /*!<DFSDM Filter4 Interrupt                                            */
#define  CN_INT_LINE_SAI3                     114     /*!< SAI3 global Interrupt                                             */
#define  CN_INT_LINE_SWPMI1                   115     /*!< Serial Wire Interface 1 global interrupt                          */
#define  CN_INT_LINE_TIM15                    116     /*!< TIM15 global Interrupt                                            */
#define  CN_INT_LINE_TIM16                    117     /*!< TIM16 global Interrupt                                            */
#define  CN_INT_LINE_TIM17                    118     /*!< TIM17 global Interrupt                                            */
#define  CN_INT_LINE_MDIOS_WKUP               119     /*!< MDIOS Wakeup  Interrupt                                           */
#define  CN_INT_LINE_MDIOS                    120     /*!< MDIOS global Interrupt                                            */
#define  CN_INT_LINE_JPEG                     121     /*!< JPEG global Interrupt                                             */
#define  CN_INT_LINE_MDMA                     122     /*!< MDMA global Interrupt                                             */
#define  CN_INT_LINE_SDMMC2                   124     /*!< SDMMC2 global Interrupt                                           */
#define  CN_INT_LINE_HSEM1                    125     /*!< HSEM1 global Interrupt                                            */
#define  CN_INT_LINE_ADC3                     127     /*!< ADC3 global Interrupt                                             */
#define  CN_INT_LINE_DMAMUX2_OVR              128     /*!<DMAMUX2 Overrun interrupt                                          */
#define  CN_INT_LINE_BDMA_Channel0            129     /*!< BDMA Channel 0 global Interrupt                                   */
#define  CN_INT_LINE_BDMA_Channel1            130     /*!< BDMA Channel 1 global Interrupt                                   */
#define  CN_INT_LINE_BDMA_Channel2            131     /*!< BDMA Channel 2 global Interrupt                                   */
#define  CN_INT_LINE_BDMA_Channel3            132     /*!< BDMA Channel 3 global Interrupt                                   */
#define  CN_INT_LINE_BDMA_Channel4            133     /*!< BDMA Channel 4 global Interrupt                                   */
#define  CN_INT_LINE_BDMA_Channel5            134     /*!< BDMA Channel 5 global Interrupt                                   */
#define  CN_INT_LINE_BDMA_Channel6            135     /*!< BDMA Channel 6 global Interrupt                                   */
#define  CN_INT_LINE_BDMA_Channel7            136     /*!< BDMA Channel 7 global Interrupt                                   */
#define  CN_INT_LINE_COMP                     137     /*!< COMP global Interrupt                                             */
#define  CN_INT_LINE_LPTIM2                   138     /*!< LP TIM2 global interrupt                                          */
#define  CN_INT_LINE_LPTIM3                   139     /*!< LP TIM3 global interrupt                                          */
#define  CN_INT_LINE_LPTIM4                   140     /*!< LP TIM4 global interrupt                                          */
#define  CN_INT_LINE_LPTIM5                   141     /*!< LP TIM5 global interrupt                                          */
#define  CN_INT_LINE_LPUART1                  142     /*!< LP UART1 interrupt                                                */
#define  CN_INT_LINE_CRS                      144     /*!< Clock Recovery Global Interrupt                                   */
#define  CN_INT_LINE_SAI4                     146     /*!< SAI4 global interrupt                                             */
#define  CN_INT_LINE_WAKEUP_PIN               149     /*!< Interrupt for all 6 wake-up pins                                  */

#define CN_INT_LINE_LAST                     (150)
#ifdef __cplusplus
}
#endif

#endif //__int_line_h__

