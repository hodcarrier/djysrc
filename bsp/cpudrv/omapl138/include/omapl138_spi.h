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
/*========================================================*
 * �ļ�ģ��˵��:                                                 *
 * omapl138��spi�������ļ����ṩ��spi�ڳ�ʼ���������������      *
 * �ļ��汾: v1.00                                               *
 * ������Ա: lst,kongds,huangcw                              *
 * ����ʱ��: 2011.06.01                                          *

 *========================================================*
 * �����޸ļ�¼(���µķ�����ǰ��):                               *
 *  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>              *
 *========================================================*
* ���磺
*  V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��
*
*/
#ifndef __CPU_PERI_SPI_H__
#define __CPU_PERI_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

//spi0 base address 0x01C41000
//spi1 base address 0x01F0E000
typedef struct
{
  vu32 GCR0;        // 0x0000
  vu32 GCR1;        // 0x0004
  vu32 INT;         // 0x0008
  vu32 LVL;         // 0x000C
  vu32 FLG;         // 0x0010
  vu32 PC0;         // 0x0014
  vu32 PC1;         // 0x0018
  vu32 PC2;         // 0x001C
  vu32 PC3;         // 0x0020
  vu32 PC4;         // 0x0024
  vu32 PC5;         // 0x0028
  vu32 RSVD0[3];       // 0x002C
  vu32 DAT0;        // 0x0038
  vu32 DAT1;        // 0x003C
  vu32 BUF;         // 0x0040
  vu32 EMU;         // 0x0044
  vu32 DELAY;       // 0x0048
  vu32 DEF;         // 0x004C
  vu32 FMT0;        // 0x0050
  vu32 FMT1;        // 0x0054
  vu32 FMT2;        // 0x0058
  vu32 FMT3;        // 0x005C
  vu32 INTVEC0;        // 0x0060
  vu32 INTVEC1;        // 0x0064
}tagSpiReg;

#define CN_SPI_CSMODE_CS    0
#define CN_SPI_CSMODE_IN    1
#define CN_SPI_CSMODE_OUT   2

// defines for SPIGCR0.
#define CN_SPI_RESET        0x00000001   // bit 0

// defines for SPIGCR1.
#define CN_SPI_ENABLE       0x01000000   // bit 24
#define CN_SPI_LOOPBACK     0x00010000   // bit 16
#define CN_SPI_POWERDOWN    0x00000100   // bit 8
#define CN_SPI_CLKMOD       0x00000002   // bit 1
#define CN_SPI_MASTER       0x00000001   // bit 0

// defines for SPIPC0, 1, 2, 3, 4, 5.
#define SOMI         (0x00000800)   // bit 11
#define SIMO         (0x00000400)   // bit 10
#define CLK          (0x00000200)   // bit 9
#define ENA          (0x00000100)   // bit 8
#define all_cs       (0x000000ff)   // bit0~7

// defines for SPIDAT1.
#define CSHOLD       (0x10000000)   // bit 28
#define CSNR         (0x00010000)   // bit 16
#define MASK_TXDATA  (0x0000FFFF)

// defines for SPIBUF.
#define RXEMPTY      (0x80000000)   // bit 31
#define TXFULL       (0x20000000)   // bit 29

// defines for SPIDEF.
#define CSDEF0       (0x00000001)   // bit 0

// defines for SPIFMT0.
#define SHIFTDIR           (0x00100000)   // bit 20
#define POLARITY           (0x00020000)   // bit 17
#define PHASE              (0x00010000)   // bit 16
#define SHIFT_PRESCALE     (8)   // bit 8
#define SHIFT_CHARLEN      (0)   // bit 0

//-----------------------------------------------------------------------------
// Public Typedefs
//-----------------------------------------------------------------------------

typedef enum
{
   SPI_MODE_SLAVE = 0,
   SPI_MODE_MASTER
} tagSpiMode;

typedef enum
{
   SPI_3PIN = 0,
   SPI_4PIN_CS,
   SPI_4PIN_EN,
   SPI_5PIN
} tagSpiOption;

typedef enum
{
   SPI_SHIFT_MSB = 0,
   SPI_SHIFT_LSB
} tagSpiShiftDir;

typedef struct
{
   tagSpiMode tagMode;
   tagSpiOption tagPinOption;
   tagSpiShiftDir tagShiftDir;
   u8 polarity;
   u8 phase;
   u32 freq;
   u8 char_len;
} tagSpiConfig;

bool_t Spi_Init(volatile tagSpiReg * tpSpi, tagSpiConfig *tagpInConfig);
void Spi_ActiveCS(volatile tagSpiReg * const tagpSpi,ufast_t ufCsNo);
void Spi_InActiveCS(volatile tagSpiReg * const tpSpi,ufast_t ufCsNo);
u32 Spi_TxRx(volatile tagSpiReg * const tpSpi,u8 *u8pSrc,u32 u32SrcLen,
                u8 *u8pDst,u32 u32DstLen,u32 u32DstOffset);

#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_SPI_H__*/
