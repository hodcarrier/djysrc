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
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得书面许可，不允许移植到非
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
// =============================================================================

// 模块描述: TCP测试
// 模块版本: V1.00
// 创建人员: Administrator
// 创建时间: 9:23:45 AM/Jul 7, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <endian.h>

#include <os.h>
#include <sys/socket.h>

#define CN_TCP_SERVERPORT  2048

static u32 sgSndTimes = 0;

#define CN_SNDBUF_LEN 0x1000*20
#define CN_RCVBUF_LEN 0x1000*20   //80KB
ptu32_t tcptstserver(void)
{
    int sockfd;
    int clientfd;
    int multiid;
    int addrlen;
    int i =0;
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    char *sndbuf;
    char *rcvbuf;
    int sndlen;
    s64 sndtotal;
    unsigned int sndprint =0;
    int sndopt;
    u32 sndtimestart;
    u32 sndtimetest;
    int sndspeed;
    int sndkbytes;


    int rcvlen;
    int rcvtotal;
    int rcvtimes;
    s64 rcvtimestart;
    s64 rcvtimeend;
    u32 nrcvtime;

    u32 activebits;
    struct MultiplexSetsCB *writesets;
    u32 badmultiwrite =0;

    struct MultiplexSetsCB *acceptsets;

    //创建发送集合
    writesets = Multiplex_Create(1);
    if(NULL==writesets)
    {
        printf("Create WriteSets failed!\n\r");
    }
    else
    {
        printf("Create WriteSets success!\n\r");
    }

    //创建接收客户端集合
    acceptsets = Multiplex_Create(1);
    if(NULL==acceptsets)
    {
        printf("Create acceptsets failed!\n\r");
    }
    else
    {
        printf("Create acceptsets success!\n\r");
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        printf("socket failed!\n\r");
        return true;
    }
    else
    {
        printf("socket success!\n\r");
    }

    serveraddr.sin_port = htons(CN_TCP_SERVERPORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(-1==bind(sockfd, &serveraddr, sizeof(serveraddr)))
    {
        printf("bind failed!\n\r");
        return true;
    }
    else
    {
        printf("bind success!\n\r");
    }

    if(-1 == listen(sockfd, 100))
    {
        printf("listen failed!\n\r");
        return true;
    }
    else
    {
        printf("listen success!\n\r");
    }

    //测试MULTIIO，不断的监视服务器端
    //设置为非阻塞
//  sndopt = 1;
//  if(0 == setsockopt(sockfd,SOL_SOCKET ,SO_NONBLOCK,&sndopt,4))
//  {
//      printf("Server:Set server noblock success!\n\r");
//  }
//  else
//  {
//      printf("Sever:Set server noblock failed!\n\r");
//  }

    if(Multiplex_AddObject(acceptsets,sockfd,CN_SOCKET_IOACCEPT))
    {
        printf("add server to acceptsets success!\n\r");
    }
    else
    {
        printf("add server to acceptsets failed!\n\r");
    }
    while(1)
    {
        if(sockfd != Multiplex_Wait(acceptsets,NULL, CN_TIMEOUT_FOREVER))
        {
            printf("MultiIo activated err!\n\r");
        }
        else
        {
            printf("MultiIo activated success!\n\r");
        }

        clientfd = accept(sockfd, &clientaddr, &addrlen);
        if(clientfd != -1)
        {
            printf("Got an client:ip = %08x  port = %04x\n\r",\
                         ntohl(clientaddr.sin_addr.s_addr),ntohs(clientaddr.sin_port));
            //关闭NAGLE
            sndopt = 1;
            if(0 == setsockopt(clientfd, IPPROTO_TCP,TCP_NODELAY,&sndopt,4))
            {
                printf("Client:close nagle success!\n\r");
            }
            else
            {
                printf("Client:close nagle  failed!\n\r");
            }

            //设置接收缓冲区
            sndopt = CN_RCVBUF_LEN;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_RCVBUF,&sndopt,4))
            {
                printf("Client:set client rcvbuf success!\n\r");
            }
            else
            {
                printf("Client:set client rcvbuf failed!\n\r");
            }
            //TEST RCV
//          rcvtotal = 0;
//          rcvbuf = M_Malloc(CN_RCVBUF_LEN,CN_TIMEOUT_FOREVER);
//          rcvtimestart = DJY_GetSysTime();
//          while(1)
//          {
//              rcvlen = recv(clientfd,rcvbuf,CN_RCVBUF_LEN,0);
//              if(rcvlen>0)
//              {
//                  rcvtotal+=rcvlen;
//                  rcvtimes++;
//              }
//              else
//              {
//                  printf("Rcv Failed!\n\r");
//              }
//              if(0==rcvtimes%1000)
//              {
//                  rcvtimeend = DJY_GetSysTime();
//                  nrcvtime = (u32)(rcvtimeend - rcvtimestart);
//                  printf("Rcv: Len =0x%08x MBytes,Time = 0x%08x us\n\r",\
//                                rcvtotal/1024/1024,nrcvtime);
//              }
//          }
            //TEST SND
            //设置发送缓冲区
            sndopt = CN_SNDBUF_LEN;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_SNDBUF,&sndopt,4))
            {
                printf("Client:set client sndbuf success!\n\r");
            }
            else
            {
                printf("Client:set client sndbuf failed!\n\r");
            }
            sndbuf = M_Malloc(CN_SNDBUF_LEN,CN_TIMEOUT_FOREVER);
            for(i = 0; i <CN_SNDBUF_LEN; i++)
            {
                sndbuf[i] = 'a'+i%27;
            }
            while(1)
            {
                sndlen = send(clientfd,sndbuf,CN_SNDBUF_LEN,0);
                sgSndTimes++;

                DJY_EventDelay(1000*mS);
            }


            //设置接收缓冲区16KB
            sndopt = 0x4000;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_RCVBUF,&sndopt,4))
            {
                printf("Client:set client rcvbuf success!\n\r");
            }
            else
            {
                printf("Client:set client rcvbuf  failed!\n\r");
            }
            //设置发送低水位,发送低水平为28K
            sndopt = 0x7000;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_SNDLOWAT,&sndopt,4))
            {
                printf("Client:set client sndbuf trig level success!\n\r");
            }
            else
            {
                printf("Client:set client sndbuf trig level  failed!\n\r");
            }
            //设置接收水位，4KB
            sndopt = 0x1000;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_RCVLOWAT,&sndopt,4))
            {
                printf("Client:set client rcvbuf trig level success!\n\r");
                printf("Client:Begin Data Snd Test!\n\r");
            }
            else
            {
                printf("Client:set client rcvbuf trig level  failed!\n\r");
            }
            //设置为非阻塞
            sndopt = 1;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_NONBLOCK,&sndopt,4))
            {
                printf("Client:Set noblock success!\n\r");
            }
            else
            {
                printf("Client:set noblock failed!\n\r");
            }
            //
            if(Multiplex_AddObject(writesets,clientfd,CN_SOCKET_IOWRITE))
            {
                printf("add client to writesets success!\n\r");
            }
            else
            {
                printf("add client to writesets failed!\n\r");
            }

            sndtotal = 0;
            sndtimestart = (u32)DJY_GetSysTime();
            while(1)
            {
                multiid = Multiplex_Wait(writesets,NULL, CN_TIMEOUT_FOREVER);
                if(clientfd == multiid)
                {
                    sndlen = send(clientfd,sndbuf,CN_SNDBUF_LEN,0);
                    sndprint++;
                    if(sndlen >0)
                    {
                        sndtotal += sndlen;
                        if(0 == sndprint%10000)
                        {
                            sndkbytes = sndtotal /1024;
                            sndtimetest = (u32)DJY_GetSysTime();
                            sndtimetest -= sndtimestart;
                            sndspeed = (sndtotal*1000)/sndtimetest;
                            printf("Send Msg:%d kbytes--speed = %d KB/S\n\r",\
                                           sndkbytes,sndspeed);
                        }
//                      DJY_EventDelay(1000*mS);
                    }
                    else
                    {
//                      printf("Client SndSet trigged but could not write!\n\r");
                    }
                }
                else
                {
                    badmultiwrite++;
                }
            }
        }
        else
        {
            printf("Bad Accept!\n\r");
        }
    }

    closesocket(sockfd);

    return 0;
}


bool_t TcpEffectTest(char *param)
{
   u16   evtt_id = CN_EVTT_ID_INVALID;
   evtt_id = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS-4, 0, 1,
        (ptu32_t (*)(void))tcptstserver,NULL, 0x1000, "TCPEffectServer");
    if (evtt_id != CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(evtt_id, NULL, 0, NULL, 0, 0);
    }
    return true;
}


