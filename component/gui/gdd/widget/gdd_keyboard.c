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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������
// 3. ��������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ��������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: GDD
//����:  zhb.
//�汾��V1.0.0
//�ļ�����: ������̿ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2016-10-08
//   ����:  zhb.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//---------------------------------

#include  "gdd.h"
#include    "../gdd_private.h"
#include "font.h"
#include    <gdd_widget.h>
#include "dbug.h"
static u8 gs_KeyBoardRowsDef=4;
static u8 gs_KeyBoardColsDef=4;
static u8 gs_KeySpaceDef=0;
static u8 gs_KeyVtimeLimitDef=10;
//static char *gs_KeyValSetsDef="123C456��789O000.R";

static u8 gs_KeyBoardRows;
static u8 gs_KeyBoardCols;
static u8 gs_KeySpace;
static u8 gs_KeyWidth,gs_KeyHeight;

static bool_t gs_bKeyDownFlag=false;
static char gs_CurKeyDown;
static s32 gs_KeyBoardId=0;





static HWND key_hwnd[16];


#define ID_KEY_0        0x6000
#define ID_KEY_1        0x6001
#define ID_KEY_2        0x6002
#define ID_KEY_3        0x6003
#define ID_KEY_4        0x6004
#define ID_KEY_5        0x6005
#define ID_KEY_6        0x6006
#define ID_KEY_7        0x6007
#define ID_KEY_8        0x6008
#define ID_KEY_9        0x6009
#define ID_KEY_10       0x600A
#define ID_KEY_11       0x600B
#define ID_KEY_12       0x600C
#define ID_KEY_13       0x600D
#define ID_KEY_14       0x600E
#define ID_KEY_15       0x600F



//-----------------------------------------------------------------------
//���ܣ�������̻�õ�ǰ���µļ�ֵ
//��������
//���أ����µļ�ֵASCIIֵ.
//-----------------------------------------------------------------------------
static u32 __Widget_VirKeyBoardRead(void)
{
      return (u32)gs_CurKeyDown;
}

//---------------------------------------------------------------------------
//���ܣ����ⰴ��������Ϣ��Ӧ����
//������pMsg��������Ϣָ��
//���أ��ɹ�����true,ʧ���򷵻�false��
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardCreate(struct WindowMsg *pMsg)
{
    HWND hwnd;
    VirKeyBoard *pVKB;
    struct KeyBoardPrivate *pkbp;
    char *name;
    char buf[5];
    if(pMsg==NULL)
        return false;
    hwnd=pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    if(pMsg->Param1==0)
    {
       pVKB=(VirKeyBoard *)malloc(sizeof(VirKeyBoard));
       if(pVKB==NULL)
           return false;
       memset(pVKB, 0, sizeof(VirKeyBoard));
       pkbp=(struct KeyBoardPrivate *)malloc(sizeof(struct KeyBoardPrivate));
       if(pkbp==NULL)
           return false;
       memset(pkbp, 0, sizeof(struct KeyBoardPrivate));
       pVKB->cols=gs_KeyBoardRowsDef;
       pVKB->rows=gs_KeyBoardColsDef;
       pVKB->keyspace=gs_KeySpaceDef;
       pVKB->id=gs_KeyBoardId;
       pkbp->key_bak=0;
       pkbp->key_now=0;
       pkbp->vtime_count=0;
       pkbp->vtime_limit=gs_KeyVtimeLimitDef;
       pkbp->read_keyboard=__Widget_VirKeyBoardRead;
       pVKB->pKBP=pkbp;

       GDD_SetWindowPrivateData(hwnd,(ptu32_t)pVKB);
       itoa(gs_KeyBoardId,buf,10);
       name="vir single touch keyboard";
       Keyboard_InstallDevice(name, pkbp);
       gs_KeyBoardId++;
    }
    else
    {

    }
    return true;
}
//----------------------------------------------------------------------------
//���ܣ�������̿ؼ���MSG_PAINT��Ϣ��Ӧ����
//������pMsg��������Ϣָ��
//���أ��ɹ�����true,ʧ���򷵻�false��
//-----------------------------------------------------------------------------
static  bool_t __Widget_VirKeyBoardPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;
    u8 width,height;
    u8 LeftStart,HighStart;
    VBK vbk;
    if(pMsg==NULL)
        return false;
    hwnd=pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    hdc =GDD_BeginPaint(hwnd);
    if(hdc==NULL)
        return false;
    vbk=(VBK)pMsg->Param1;
    if(vbk==NULL)
    {
        gs_KeyBoardRows=gs_KeyBoardRowsDef;
        gs_KeyBoardCols=gs_KeyBoardColsDef;
        gs_KeySpace=gs_KeySpaceDef;
    }
    else
    {
        gs_KeyBoardRows=vbk->rows;
        gs_KeyBoardCols=vbk->cols;
        gs_KeySpace=vbk->keyspace;
    }

    GDD_GetClientRect(hwnd,&rc);
    GDD_SetFillColor(hdc,RGB(255,255,255));
    GDD_FillRect(hdc,&rc);
    width=GDD_RectW(&rc);
    height=GDD_RectH(&rc);
    gs_KeyWidth=(width-(gs_KeyBoardCols+1)*gs_KeySpace)/gs_KeyBoardCols;
    gs_KeyHeight=(height-(gs_KeyBoardRows+1)*gs_KeySpace)/gs_KeyBoardRows;
    LeftStart=gs_KeySpace;
    HighStart=gs_KeySpace;
    //
    key_hwnd[0]=Widget_CreateButton("1",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,\
        LeftStart,HighStart,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_0,(ptu32_t)"1",NULL);

    key_hwnd[1]=Widget_CreateButton("2",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,\
        LeftStart+gs_KeyWidth,HighStart,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_1,(ptu32_t)"2",NULL);

    key_hwnd[2]=Widget_CreateButton("3",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,\
        LeftStart+2*gs_KeyWidth,HighStart,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_2,(ptu32_t)"3",NULL);

    key_hwnd[3]=Widget_CreateButton("x",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,\
        LeftStart+3*gs_KeyWidth,HighStart,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_3,(ptu32_t)"x",NULL);



    key_hwnd[4]=Widget_CreateButton("4",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart,
        HighStart+gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_4,(ptu32_t)"4",NULL);

    key_hwnd[5]=Widget_CreateButton("5",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+gs_KeyWidth,
        HighStart+gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_5,(ptu32_t)"5",NULL);

    key_hwnd[6]=Widget_CreateButton("6",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL, LeftStart+2*gs_KeyWidth,
        HighStart+gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_6,(ptu32_t)"6",NULL);

    key_hwnd[7]=Widget_CreateButton("��",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+3*gs_KeyWidth,
        HighStart+gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_7,(ptu32_t)"��",NULL);



    key_hwnd[8]=Widget_CreateButton("7",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart,
                 HighStart+2*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_8,(ptu32_t)"7",NULL);

    key_hwnd[9]=Widget_CreateButton("8",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+gs_KeyWidth,
                 HighStart+2*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_9,(ptu32_t)"8",NULL);

    key_hwnd[10]=Widget_CreateButton("9",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+2*gs_KeyWidth,
                 HighStart+2*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_10,(ptu32_t)"9",NULL);

    key_hwnd[11]=Widget_CreateButton("��",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+3*gs_KeyWidth,
                 HighStart+2*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_11,(ptu32_t)"��",NULL);



    key_hwnd[12]=Widget_CreateButton("0",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart,
                     HighStart+3*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_12,(ptu32_t)"0",NULL);

    key_hwnd[13]=Widget_CreateButton(".",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+gs_KeyWidth,
                 HighStart+3*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_13,(ptu32_t)".",NULL);

    key_hwnd[14]=Widget_CreateButton("R",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+2*gs_KeyWidth,
                 HighStart+3*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_14,(ptu32_t)"R",NULL);

    key_hwnd[15]=Widget_CreateButton("��",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+3*gs_KeyWidth,
                 HighStart+3*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_15,(ptu32_t)"��",NULL);

    if(key_hwnd[15]==NULL)
    {
        debug_printf("gdd","Create virtual keyboard failed.\r\n");
        return false;
    }

    GDD_EndPaint(hwnd,hdc);
    return true;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//-----------------------------------------------------------------------------
//���ܣ��ж�ĳ�������Ƿ��ܽ�������������룬ֻ��TextBox��RICHTEXTBOX��LISTBOX��LISTVIEW
//�ȿ��Խ��������������.
//������hwnd�����ھ��.
//���أ����򷵻�true,���򷵻�false.
//-----------------------------------------------------------------------------
static bool_t __Widget_IsKeyBoradInputValidControl(HWND hwnd)
{
      return false;
}
#pragma GCC diagnostic pop

//-------------------------------------------------------------------------
//���ܣ�������̿ؼ���MSG_KEY_DOWN��Ϣ��Ӧ����
//������pMsg��������Ϣָ��
//���أ��ɹ�����true,ʧ���򷵻�false��
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardKeyDown(struct WindowMsg *pMsg)
{
     u8 val;
     bool_t ret;
     HWND focus_hwnd=NULL;
     HWND hwnd;
     if(pMsg==NULL)
         return false;
     hwnd =pMsg->hwnd;
     if(hwnd==NULL)
         return false;
    //���µļ�ֵ
    val=pMsg->Param1;
    //���ҵ�ǰ���㴰��
    focus_hwnd=GDD_GetFocusWindow();
    if(focus_hwnd==NULL)
        return false;
    ret=__Widget_IsKeyBoradInputValidControl(focus_hwnd);
    if(ret)
    {
         GDD_SendMessage( focus_hwnd, MSG_KEY_DOWN, val,  0);
         return true;
    }
    else
        return false;

}
//----------------------------------------------------------------------------
//���ܣ�������̿ؼ���MSG_KEY_UP��Ϣ��Ӧ����
//������pMsg��������Ϣָ��
//���أ��ɹ�����true,ʧ���򷵻�false��
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardKeyUp(struct WindowMsg *pMsg)
{
    u8 val;
    bool_t ret;
    HWND focus_hwnd=NULL;
    HWND hwnd;
    if(pMsg==NULL)
         return false;
    hwnd =pMsg->hwnd;
    if(hwnd==NULL)
         return false;
    //���µļ�ֵ
    val=pMsg->Param1;
    //���ҵ�ǰ���㴰��
    focus_hwnd=GDD_GetFocusWindow();
    if(focus_hwnd==NULL)
        return false;
    ret=__Widget_IsKeyBoradInputValidControl(focus_hwnd);
    if(ret)
    {
         GDD_SendMessage( focus_hwnd, MSG_KEY_UP, val,  0);
         return true;
    }
    else
        return false;
}

//---------------------------------------------------------------------------
//���ܣ���ȡ������̰��µļ�ֵ.
//������x�����µ��x��������;
//      y,���µ��y��������;
//���أ����µ��ַ�.
//-----------------------------------------------------------------------------
static char  __Widget_GetKeyDownVal(u16 x,u16 y)
{
    u8 i=0,j=0,idx=0;
    RECT *rect;
    POINT *pt;
    bool_t ret=false;
    HWND hwnd;
    char KeyVal;
    char *pdata;
    rect=malloc(sizeof(RECT));
    if(rect==NULL)
        return '\0';
    pt=malloc(sizeof(POINT));
    if(pt==NULL)
        return '\0';
    pt->x=x;
    pt->y=y;

    for(i=0;i<gs_KeyBoardRows;i++)
    {
        for(j=0;j<gs_KeyBoardCols;j++)
        {
           rect->left=(j*gs_KeyWidth)+(j+1)*gs_KeySpace;
           rect->top=(i*gs_KeyHeight)+(i+1)*gs_KeySpace;
           rect->right=rect->left+gs_KeyWidth;
           rect->bottom=rect->top+gs_KeyHeight;
           ret=GDD_PtInRect(rect,pt);
           if(ret==true)
               break;
        }
    }

    idx=i*gs_KeyBoardCols+j;
    hwnd=key_hwnd[idx];
    if(hwnd!=NULL)
    {
        pdata=(char *)GDD_GetWindowPrivateData(hwnd);
        KeyVal=*pdata;
        return KeyVal;
    }
    else
        return 0;

}

//------------------------------------------------------------------------------
//���ܣ�������̿ؼ���MSG_TOUCH_DOWN��Ϣ��Ӧ����
//������pMsg��������Ϣָ�롣
//���أ��ɹ�����true,ʧ���򷵻�false��
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardTouchDown(struct WindowMsg *pMsg)
{
     HWND hwnd;
     u32 loc;;
     u16 x,y;
     char keyval;
     if(pMsg==NULL)
         return false;
     hwnd =pMsg->hwnd;
     if(hwnd==NULL)
         return false;
     loc=pMsg->Param2;   //��ȡ��������Ϣ
     x=loc;
     y=loc>>16;
     //��ȡ���ĸ�����������
     keyval=__Widget_GetKeyDownVal(x,y);
     gs_bKeyDownFlag=true;
     gs_CurKeyDown=keyval;
     return true;
}

//----���ⰴ������-------------------------------------------------------------
//���ܣ�������̿ؼ���MSG_TOUCH_UP��Ϣ��Ӧ����
//������pMsg��������Ϣָ�롣
//���أ��ɹ�����true,ʧ���򷵻�false��
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardTouchUp(struct WindowMsg *pMsg)
{
     HWND hwnd;
     u32 loc;;
     u16 x,y;
     char keyval;
     if(pMsg==NULL)
         return false;
     hwnd =pMsg->hwnd;
     if(hwnd==NULL)
         return false;
     if(!gs_bKeyDownFlag)
         return false;
     loc=pMsg->Param2;   //��ȡ��������Ϣ
     x=loc;
     y=loc>>16;
     //��ȡ���ĸ�����������
     keyval=__Widget_GetKeyDownVal(x,y);
     gs_bKeyDownFlag=false;
     gs_CurKeyDown=keyval;
     return true;
}

//-----------------------------------------------------------------
//���ܣ�������̿ؼ�MSG_NOTIFY��Ϣ��Ӧ����
 //������pMsg��������Ϣָ�롣
 //���أ��ɹ�����true,ʧ���򷵻�false��
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardNotifyHandle(struct WindowMsg *pMsg)
{
     HWND hwnd,Hwnd;
     u16 event,id;
     char *pdata;
     char keyval;
     if(pMsg==NULL)
         return false;
     hwnd =pMsg->hwnd;
     if(hwnd==NULL)
         return false;
     event=HI16(pMsg->Param1);
     id=LO16(pMsg->Param1);

     id&=0xff;

     Hwnd=key_hwnd[id];
     pdata=(char *)GDD_GetWindowPrivateData(Hwnd);

     keyval=*pdata;



     if(event==MSG_BTN_UP)
     {
        GDD_PostMessage( hwnd, MSG_KEY_UP, (u32) keyval, 0);
     }
     else if(event==MSG_BTN_DOWN)
     {
        GDD_PostMessage( hwnd, MSG_KEY_DOWN, (u32) keyval, 0);
     }
     else
     {
        //  todo
     }
     return true;
}



//Ĭ�����������Ϣ�����������������û���������û�д�������Ϣ��
static struct MsgProcTable s_gVirKeyBoardMsgProcTable[] =
{
    {MSG_KEY_DOWN,__Widget_VirKeyBoardKeyDown},
    {MSG_KEY_UP, __Widget_VirKeyBoardKeyUp},
    {MSG_CREATE,__Widget_VirKeyBoardCreate},
    {MSG_PAINT,__Widget_VirKeyBoardPaint},
    {MSG_TOUCH_DOWN,__Widget_VirKeyBoardTouchDown},
    {MSG_TOUCH_UP,__Widget_VirKeyBoardTouchUp},
    {MSG_NOTIFY,__Widget_VirKeyBoardNotifyHandle}
};

static struct MsgTableLink  s_gVirKeyBoardMsgLink;
// =============================================================================
// ��������: ������̿ؼ�����������
// �������: Text:������̴���Text;
//           Style:������̷�񣬲μ�gdd.h;
//           x:���������ʼλ��x��������(��λ������);
//           y:���������ʼλ��y��������(��λ������);
//           w:������̿���(��λ������);
//           h:������̸߶�(��λ������);
//           hParent:������̸����ھ��;
//           WinId:������̿ؼ�Id;
//           pdata:������̿ؼ�˽�����ݽṹ;
//           UserMsgTableLink:������̿ؼ��û���Ϣ�б��ṹָ�롣
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻��ı�������ʧ���򷵻�NULL��
// =============================================================================
HWND Widget_CreateVirKeyBoard(const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;
    s_gVirKeyBoardMsgLink.MsgNum = sizeof(s_gVirKeyBoardMsgProcTable) / sizeof(struct MsgProcTable);
    s_gVirKeyBoardMsgLink.myTable = (struct MsgProcTable *)&s_gVirKeyBoardMsgProcTable;
    pGddWin=GDD_CreateWindow(Text,WS_CHILD | WS_CAN_FOCUS|Style,x,y,w,h,hParent,WinId,
                            CN_WINBUF_PARENT,pdata,&s_gVirKeyBoardMsgLink);
    if(UserMsgTableLink != NULL)
        GDD_AddProcFuncTable(pGddWin,UserMsgTableLink);
    return pGddWin;
}