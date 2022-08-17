/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include <stdint.h>
#include <stddef.h>
//manual config start
//�˴���д�ֶ����ã�DIDE���������ļ�ʱ�����ᱻ�޸�
//manual config end

#define    CN_RUNMODE_IBOOT                0                //IBOOTģʽ����
#define    CN_RUNMODE_APP                  2                //��IBOOT���ص�APP
#define    CFG_RUNMODE                     CN_RUNMODE_APP           //��IBOOT���ص�APP
#define    CN_RUNMODE_BOOTSELF             1                       //����IBOOT��������ģʽAPP
//*******************************  Configure arp  ******************************************//
#define     CFG_ARP_HASHLEN             32      //ռ��һ��ָ��
#define    CFG_MODULE_ENABLE_ARP           true
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure cpu drive inner flash  ******************************************//
#define CFG_EFLASH_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ������оƬ��
#define CFG_EFLASH_PAGE_SIZE                 512      //Ƭ��flash��ҳ��С����λ�ֽڡ�
#define CFG_EFLASH_SMALL_SECT_PAGE_NUM       256         //Ƭ��flash��С�����У��ж���ҳ��
#define CFG_EFLASH_LARGE_SECT_PAGE_NUM       0         //Ƭ��flash�Ĵ������У��ж���ҳ��
#define CFG_EFLASH_NORMAL_SECT_PAGE_NUM      0         //Ƭ��flash�ı�׼�����У��ж���ҳ��
#define CFG_EFLASH_PLANE_SMALL_SECT_NUM      16         //Ƭ��flash�����洢���У��ж���С������
#define CFG_EFLASH_PLANE_LARGE_SECT_NUM      0         //Ƭ��flash�����洢���У��ж��ٴ�������
#define CFG_EFLASH_PLANE_NORMAL_SECT_NUM     0       //Ƭ��flash�����洢���У��ж��ٱ�׼������
#define CFG_EFLASH_PLANE_NUM                 1         //Ƭ��flash�����洢�������
#define CFG_EFLASH_MAPPED_START_ADDR         0x8000000 //Ƭ��flash��ӳ����ʼ��ַ��
#define    CFG_MODULE_ENABLE_CPU_DRIVE_INNER_FLASH  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_ETH_NETCARD_NAME    "STM32H7_ETH"   //
#define CFG_ETH_LOOP_CYCLE      1000         //�ж�ģʽ������д
#define CFG_ETH_LOOP_ENABLE     true            //
#define CFG_ETH_HARD_MAC_ADDR   true            //
#define CFG_ETH_MAC_ADDR0      00           //��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR1      02           //��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR2      03           //��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR3      04           //��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR4      05           //��ѡ��"Ӳ������Mac��ַ",��������д
#define CFG_ETH_MAC_ADDR5      06           //��ѡ��"Ӳ������Mac��ַ",��������д
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_MAC  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                  //
#define CFG_UART_RECVBUF_LEN            32                  //
#define CFG_UART1_ENABLE                true                //
#define CFG_UART2_ENABLE                false               //
#define CFG_UART3_ENABLE                false               //
#define CFG_UART4_ENABLE                false               //
#define CFG_UART5_ENABLE                false               //
#define CFG_UART6_ENABLE                false               //
#define CFG_UART7_ENABLE                false               //
#define CFG_UART8_ENABLE                false               //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure dhcp  ******************************************//
#define     CFG_DHCPD_ENABLE            false   //
#define     CFG_DHCPC_ENABLE            true    //
#define     CFG_DHCP_RENEWTIME          3600    //����
#define     CFG_DHCP_REBINDTIME         3600    //����
#define     CFG_DHCP_LEASETIME          3600    //����
#define     CFG_DHCPD_IPNUM             0x40    //64
#define     CFG_DHCPD_IPSTART           "192.168.0.2"    //
#define     CFG_DHCPD_SERVERIP          "192.168.0.253"  //
#define     CFG_DHCPD_ROUTERIP          "192.168.0.253"  //
#define     CFG_DHCPD_NETIP             "255.255.255.0"  //
#define     CFG_DHCPD_DNS               "192.168.0.253"  //
#define     CFG_DHCPD_DNSBAK            "192.168.0.253"  //
#define     CFG_DHCPD_DOMAINNAME       "domain"        //
#define    CFG_MODULE_ENABLE_DHCP          true
//*******************************  Configure emflash insatall xip  ******************************************//
#define CFG_EFLASH_XIP_PART_START      6          //������ʼ����д��ţ���Ŵ�0��ʼ����
#define CFG_EFLASH_XIP_PART_END        -1         //����������-1��ʾ���һ�飬�����������6����ʼ����0����÷���ʹ�õĿ�Ϊ0��1��2��3��4��5��
#define CFG_EFLASH_XIP_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
#define CFG_EFLASH_XIPFSMOUNT_NAME   "xip-app"    //�谲װ���ļ�ϵͳ��mount�����֣�NULL��ʾ��flash�������ļ�ϵͳ
#define    CFG_MODULE_ENABLE_EMFLASH_INSATALL_XIP  true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //��ʹѡfalseҲ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure icmp  ******************************************//
#define    CFG_MODULE_ENABLE_ICMP          true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    15      //�¼�������
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼������������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_IDLE_MONITOR_CYCLE  30      //����IDLE�¼���������1/16 CPUռ�ȵ�ʱ��������0=������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //������Զ�����
#define CFG_HANDLE_LIMIT        8   //������Զ�����
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false       //
#define CFG_START_APP_IS_VERIFICATION      true       //
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ�����
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL   //
#define CFG_IBOOT_VERSION_SMALL       00        //xx.xx.__APP����
#define CFG_IBOOT_VERSION_MEDIUM      00        //xx.__.xxAPP����
#define CFG_IBOOT_VERSION_LARGE       01        //__.xx.xxAPP����
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"            //
#define CFG_FORCED_UPDATE_PATH     "/SD/djyapp.bin"           //
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //�����������������ź����ͻ�����
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure microPython113  ******************************************//
#define CFG_PYTHON_STACKSIZE    4096    //
#define CFG_PYTHON_HEAPSIZE     4096    //
#define    CFG_MODULE_ENABLE_MICROPYTHON113  true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure network config  ******************************************//
#define CFG_STATIC_IP       true            //
#define CFG_SELECT_NETCARD  "STM32H7_ETH"   //������ѡ�е��������������õ�������ͬ
#define CFG_MY_IPV4         "192.168.0.179" //
#define CFG_MY_SUBMASK      "255.255.255.0" //
#define CFG_MY_GATWAY       "192.168.0.1"   //
#define CFG_MY_DNS          "192.168.0.1"   //
#define CFG_MY_DNSBAK       "192.168.0.1"   //
#define    CFG_MODULE_ENABLE_NETWORK_CONFIG  true
//*******************************  Configure ppp  ******************************************//
#define    CFG_MODULE_ENABLE_PPP           true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure router  ******************************************//
#define    CFG_MODULE_ENABLE_ROUTER        true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000      //
#define CFG_ADD_ROUTINE_SHELL      true        //
#define CFG_ADD_EXPAND_SHELL       true        //
#define CFG_ADD_GLOBAL_FUN         false       //
#define CFG_SHOW_ADD_SHEELL        true        //
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure sock  ******************************************//
#define     CFG_SOCKET_NUM              10      //ռһ�� tagItem �ṹ
#define    CFG_MODULE_ENABLE_SOCK          true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure STM32 commond code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER             true    //��Դ����Ŵ�
#define     CFG_TCP_CCBNUM              10      //ռһ�� ָ�� �� struct ClientCB
#define     CFG_TCP_SCBNUM              5       //ռһ�� ָ�� �� struct ServerCB
#define     CFG_TCP_SOCKET_HASH_LEN     10      //����ͨ����IP+port����Ԫ�����socket
#define    CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000  //
#define     CFG_TPL_PROTONUM            5       //ռ��һ�� tagTplProtoItem �ṹ
#define    CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure telnet  ******************************************//
#define     CFG_TELNETD_ENABLE          true    //
#define     CFG_TELNETC_ENABLE          false   //
#define    CFG_MODULE_ENABLE_TELNET        true
//*******************************  Configure tftp  ******************************************//
#define     CFG_TFTP_PATHDEFAULT       "/"   //TFTPĬ�Ϲ���Ŀ¼
#define    CFG_MODULE_ENABLE_TFTP          true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //����ʱ���Ƕ�8��
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure udp  ******************************************//
#define     CFG_UDP_CBNUM               10      //ռ��һ�� tagUdpCB �ṹ
#define     CFG_UDP_HASHLEN             4       //ռ��һ��ָ��
#define     CFG_UDP_PKGMSGLEN        1472       //udp��������
#define     CFG_UDP_BUFLENDEFAULT    0x2000     //8KB
#define    CFG_MODULE_ENABLE_UDP           true
//*******************************  Configure watch dog  ******************************************//
#define CFG_WDT_LIMIT           10      //������������
#define    CFG_MODULE_ENABLE_WATCH_DOG     true
//*******************************  Core Clock  ******************************************//
#define    CFG_CORE_MCLK                   (216.0*Mhz)       //��Ƶ���ں�Ҫ�ã����붨��
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //��������
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //��Ʒ����
#define    PRODUCT_PRODUCT_MODEL           ""                //��Ʒ�ͺ�
#define    PRODUCT_VERSION_LARGE           0                 //�汾��,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //�汾��,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //�汾��,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //��Ʒ�ͺű���
#define    PRODUCT_BRANCH                  ""                //��Ʒ��֧
#define    PRODUCT_PASSWORD                ""                //��Ʒ��Կ
#define    PRODUCT_OTA_ADDRESS             ""                //OTA��������ַ
#define    PRODUCT_BOARD_TYPE              "NUCLEO-H743ZI"   //�������
#define    PRODUCT_CPU_TYPE                "stm32h743xx"     //CPU����


#endif