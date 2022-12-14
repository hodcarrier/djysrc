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
//作者:  LiuWei.
//版本：V1.0.0
//文件描述: 矩形运算函数集
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-11-10
//   作者:  LiuWei.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include    "gdd_private.h"

s32 GDD_RectW(const RECT *prc)
{
    return (prc->right - prc->left);
}

s32 GDD_RectH(const RECT *prc)
{
    return (prc->bottom - prc->top);
}

//----设置矩形尺寸-----------------------------------------------------------
//描述: 略
//参数：略
//返回：窗口对象内存内存指针
//------------------------------------------------------------------------------
void    GDD_SetRect(RECT *prc,s32 x,s32 y,s32 w,s32 h)
{
    if(prc!=NULL)
    {
        prc->left   = x;
        prc->top    = y;
        prc->right  = x+w;
        prc->bottom = y+h;
    }
}

//----清空矩形尺寸-----------------------------------------------------------
//描述: 略
//参数：略
//返回：窗口对象内存内存指针
//------------------------------------------------------------------------------
void    GDD_SetRectEmpty(RECT *prc)
{
    if(prc!=NULL)
    {
        prc->left   = 0;
        prc->top    = 0;
        prc->right  = 0;
        prc->bottom = 0;
    }
}

bool_t    GDD_IsRectEmpty(const RECT *prc)
{
    if(prc!=NULL)
    {
        if((prc->right - prc->left)<=0)
        {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

bool_t    GDD_CopyRect(RECT *dst,const RECT *src)
{
    if((NULL!=dst) && (NULL!=src))
    {
        dst->left =src->left;
        dst->top =src->top;
        dst->right =src->right;
        dst->bottom =src->bottom;
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/

void    __GDD_OffsetRect(RECT *prc,s32 dx,s32 dy)
{
    prc->left += dx;
    prc->top  += dy;
    prc->right += dx;
    prc->bottom  += dy;
}

bool_t    GDD_OffsetRect(RECT *prc,s32 dx,s32 dy)
{
    if(NULL!=prc)
    {
        __GDD_OffsetRect(prc,dx,dy);
        return TRUE;
    }
    return FALSE;
}

bool_t    GDD_MoveRect(RECT *prc,s32 x,s32 y)
{
    if(NULL!=prc)
    {
        prc->right += x - prc->left;
        prc->bottom  += y - prc->top;
        prc->left += x;
        prc->top  += y;
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/

void    __GDD_InflateRect(RECT *prc,s32 dx,s32 dy)
{
    prc->left   -= dx;
    prc->top    -= dy;
    prc->right  += dx;
    prc->bottom += dy;
}

bool_t    GDD_InflateRect(RECT *prc,s32 dx,s32 dy)
{
    if(NULL!=prc)
    {
        __GDD_InflateRect(prc,dx,dy);
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/

void    __GDD_InflateRectEx(RECT *prc,s32 l,s32 t,s32 r,s32 b)
{
    prc->left   -= l;
    prc->top    -= t;
    prc->right  += r;
    prc->bottom += b;
}

bool_t GDD_InflateRectEx(RECT *prc,s32 l,s32 t,s32 r,s32 b)
{
    if(NULL!=prc)
    {
        __GDD_InflateRectEx(prc,l,t,r,b);
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/

//bool_t    __PtInRect(const RECT *prc,const POINT *pt)
//{
//    if(pt->x < prc->left)   return FALSE;
//    if(pt->x >= prc->right) return FALSE;
//
//    if(pt->y < prc->top)        return FALSE;
//    if(pt->y >= prc->bottom)    return FALSE;
//    return TRUE;
//
//}

bool_t    GDD_PtInRect(const RECT *prc,const POINT *pt)
{
    if((NULL==prc) || (NULL==pt))
        return FALSE;
    else
    {
        if(pt->x < prc->left)
            return FALSE;
        if(pt->x >= prc->right)
            return FALSE;

        if(pt->y < prc->top)
            return FALSE;
        if(pt->y >= prc->bottom)
            return FALSE;
        return TRUE;
    }
}

/*============================================================================*/


