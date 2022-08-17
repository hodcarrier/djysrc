//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块: GDD
//作者:  luost.
//版本：V1.0.0
//文件描述: gdd鼠标显示
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2017-01-14
//   作者:  luost.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include <stdint.h>
#include    "gdd_private.h"
#include <gui\gdd_focus.h>

HWND HWND_Focus=NULL;

//焦点窗口算法：1、初始化后，focus = desktop，若有多个desktop，则第一个。
//2、删除窗口时，焦点交给下一个可获得焦点的兄弟窗口，无则置空。
//3、当焦点为NULL，那cursor窗口寄存在哪里呢？设为隐藏并放到desktop上。
//----设置焦点窗口-------------------------------------------------------------
//描述: 把一个窗口设为焦点窗口，并设为光标窗口的父窗口。
//参数：hwnd: 新的焦点窗口
//返回：true = success, false = failure.
//-----------------------------------------------------------------------------
bool_t GDD_SetFocusWindow(HWND hwnd)
{
    HWND wnd=NULL;
    if(! GDD_IsFocusEnable(hwnd))
        return false;
    if(__GDD_Lock())
    {
        wnd =HWND_Focus;
        HWND_Focus =hwnd;
        GDD_CursorSetHost(hwnd);
        if( GDD_CursorCheckStatus(hwnd) )
            GDD_CursorSetShow();
        else
            GDD_CursorSetHide();
        __GDD_Unlock();
    }
    else
    {
        return false;
    }

    if(wnd!=NULL)
    {
        GDD_SendMessage(wnd,MSG_KILLFOCUS,0,0);
    }
    if(hwnd!=NULL)
    {
        GDD_SendMessage(hwnd,MSG_SETFOCUS,0,0);
    }

    return true;

}

//----获得焦点窗口--------------------------------------------------------------
//描述: 略.
//参数：无.
//返回：焦点窗口.
//------------------------------------------------------------------------------
HWND    GDD_GetFocusWindow(void)
{
    HWND wnd=NULL;

    if(__GDD_Lock())
    {
        wnd =HWND_Focus;
        __GDD_Unlock();
    }
    return wnd;
}

//----判断是否允许设为焦点-----------------------------------------------------
//功能：判断一个窗口是否允许作为输入焦点
//参数：hwnd，窗口句柄
//返回：true = 允许, false = 不允许.
//-----------------------------------------------------------------------------
bool_t GDD_IsFocusEnable(HWND hwnd)
{
    if((hwnd->Style & WS_CAN_FOCUS) == WS_CAN_FOCUS)
        return true;
    else
        return false;
}

//----判断是否为焦点窗口-------------------------------------------------------
//描述: 略.
//参数：无.
//返回：如果指定的窗口是焦点窗口,将返回TRUE,否则返回FALSE.
//------------------------------------------------------------------------------
bool_t  GDD_IsFocusWindow(HWND hwnd)
{
    bool_t res=FALSE;

    if(__GDD_Lock())
    {
        if(hwnd == HWND_Focus)
        {
            res=TRUE;
        }
        __GDD_Unlock();
    }
    return res;

}

//----是否焦点祖先-------------------------------------------------------------
//功能：检查一个窗口是否焦点窗口或其祖先窗口
//参数：hwnd，待检查的窗口
//返回：true = 是祖先窗口，false = 不是
//-----------------------------------------------------------------------------
bool_t GDD_IsFocusAncestor(HWND hwnd)
{
    HWND desktop,current;
    bool_t result = false;
    if(hwnd == NULL)
       return false;
    current = GDD_GetFocusWindow();
    desktop = GDD_GetDesktopWindow( );
    while(1)
    {
        if(hwnd == current)
        {
            result = true;
            break;
        }
        if(current == desktop)
        {
            result = false;
            break;
        }
        current = GDD_GetWindowParent(current);
    }
    return result;
}



