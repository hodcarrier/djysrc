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
//所属模块:键盘
//作者：lst
//版本：V1.0.0
//文件描述: 扫描硬件读按键函数，只需直接给出按键当前值，无须防抖。
//其他说明:
//修订历史:
//2. 日期: 2009-05-29
//   作者: lst
//   新版本号: V1.1.0
//   修改说明: 增加key_hard_init函数
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "keyboard.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Keyboard_TQ2416(const char *dev_name);
//    ModuleInstall_Keyboard_TQ2416(CFG_DM9000_DEV_NAME);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"keyboard hard driver"//板件的键盘驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                   //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                  //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                //初始化时机，可选值：early，medium，later, pre-main。
                                  //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                  //选中该组件时，被依赖组件将强制选中，
                                  //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"GDD"            //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                  //选中该组件时，被依赖组件不会被强制选中，
                                  //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                  //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_KEYBOARD_HARD_DRIVER == false )
//#warning  " keyboard_hard_driver  组件参数未配置，使用默认配置"
//%$#@target = header             //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_KEYBOARD_HARD_DRIVER    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_KEYBOARD_NAME              "TQ2416_KEYBOARD"               //"name",配置键盘名称
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

bool_t key_hard_init(void);
u32 keyboard_scan(void);
//----初始化键盘模块-----------------------------------------------------------
//功能: 初始化一个由windows的键盘和按钮模拟的键盘，该键盘供8个键。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Keyboard_TQ2416(const char *dev_name)
{
    static struct KeyBoardPrivate key_brd;

    key_hard_init();

    key_brd.read_keyboard = keyboard_scan;
    Keyboard_InstallDevice(dev_name,&key_brd);
    key_brd.vtime_limit = 0;
    key_brd.vtime_count = 100;
    key_brd.key_bak = 0;
    key_brd.key_now = 0;
    return true;
}


//----初始化键盘硬件-----------------------------------------------------------
//功能: 如题，初始化后才能扫描键盘
//参数: 无
//返回: 无
//----------------------------------------------------------------------------
bool_t key_hard_init(void)
{
/* K1:GPF7
 * K2:GPG1
 * K3:GPG2
 * K4:GPG3
 */
    struct GPIO_REG *gpio =GPIO_REG_BASE;

    //GPF7
    gpio->GPFCON &= ~((0x3<<14));
    gpio->GPFUDP &= ~((0x3<<14));
    gpio->GPFUDP |=  ((0x2<<14));

    //GPG1,2,3
    gpio->GPFCON &= ~((0x3<<6)|(0x3<<4)|(0x3<<2));
    gpio->GPFUDP &= ~((0x3<<6)|(0x3<<4)|(0x3<<2));
    gpio->GPFUDP |=  ((0x2<<6)|(0x2<<4)|(0x2<<2));


    return true;
}


//----键盘硬件扫描------------------------------------------------------------
//功能: 读取键盘按键状态，每个按键用8bit表示，32位返回值可以存放4个按键。扫描到
//      的第一个按键放在最低8位，第四个按键放在最高8位。扫描到4个按键后，不再继
//      续，若同时按下超过4个按键，只记录4个。
//参数: 无
//返回:  参见 struct KeyBoardPrivate中注释
//----------------------------------------------------------------------------
u32 keyboard_scan(void)
{
    u32 readed;
    uint8_t i = 0;

    readed = 0;
    if( ! (pg_gpio_reg->GPFDAT & 0x80))
    {
        readed |= (u32)(1<<(i<<3));
        i++;
    }
    if( ! (pg_gpio_reg->GPGDAT & 0x2))
    {
        readed |= (u32)(2<<(i<<3));
        i++;
    }
    if( ! (pg_gpio_reg->GPGDAT & 0x4))
    {
        readed |= (u32)(3<<(i<<3));
        i++;
    }
    if( ! (pg_gpio_reg->GPGDAT & 0x8))
    {
        readed |= (u32)(4<<(i<<3));
        i++;
    }
    return(readed);
}
