#include "include.h"
#include "sys_rtos.h"
#include "role_launch.h"
#include "mem_pub.h"
#include "rtos_pub.h"
#include "error.h"
#include "uart_pub.h"
#include "wlan_ui_pub.h"

#if RL_SUPPORT_FAST_CONNECT
#include "drv_model_pub.h"
#include "flash_pub.h"
#include "param_config.h"
//#include "wlan_dev.h"
//#include "ef_cfg.h"

#include "ieee802_11_defs.h"

#define RT_NULL (void*)(0)

#define MAX_FASTCONN_RETRY_CNT              1 /*fast connect retry cnt*/
#if (CFG_SUPPORT_RTT)
#define BSSID_INFO_ADDR       (EF_START_ADDR - 0x1000) /*reserve 4k for bssid info*/
#else

#define rt_calloc calloc
#define rt_free free
#define rt_kprintf printk

#endif
#endif

#if CFG_ROLE_LAUNCH

RL_T g_role_launch = {0};
RL_SOCKET_T g_rl_socket = {0};
uint32_t g_mrole_csa_flag = 0;
RL_SOCKET_CACHE_T g_sta_cache = {0};

extern u8* wpas_get_sta_psk(void);

#if RL_SUPPORT_FAST_CONNECT
extern void user_connected_callback(FUNCPTR fn);
#endif

#if RL_SUPPORT_FAST_CONNECT
static char g_rl_sta_key[64];
static char rl_cnt = 0;
void rl_cnt_init(void)
{
    rl_cnt = 0;
}

static void rl_read_bssid_info(RL_BSSID_INFO_PTR bssid_info)
{
#if (CFG_SUPPORT_RTT)
    uint32_t status, addr;
    DD_HANDLE flash_hdl;
    flash_hdl = ddev_open(FLASH_DEV_NAME, &status, 0);
    addr = BSSID_INFO_ADDR;
    ddev_read(flash_hdl, (char *)bssid_info, sizeof(RL_BSSID_INFO_T), addr);
    ddev_close(flash_hdl);
#endif
}

//chenws: 直接在获取ip成功后调用该函数
/*static*/ void rl_write_bssid_info(void)
{
    int i;
    uint8_t temp[4];
    uint8_t *psk;
    uint32_t status, addr, protect_param,protect_flag;
    RL_BSSID_INFO_PTR bssid_info,bssid_rd;
    LinkStatusTypeDef link_status;
    DD_HANDLE flash_hdl;
    uint32_t ssid_len;
    GLOBAL_INT_DECLARATION();


    rl_cnt_init();//when connected success, init cnt
     bssid_info = rt_calloc(1,sizeof(RL_BSSID_INFO_T));
     bssid_rd = rt_calloc(1,sizeof(RL_BSSID_INFO_T));
     if( (bssid_rd == RT_NULL)|| (bssid_info == RT_NULL))
     {
        rt_kprintf("malloc fail\r\n");
        return;
     }
    os_memset(&link_status, 0, sizeof(link_status));
    bk_wlan_get_link_status(&link_status);
    ssid_len = os_strlen(link_status.ssid);
    if(ssid_len > SSID_MAX_LEN)
    {
        ssid_len = SSID_MAX_LEN;
    }
    os_strncpy(bssid_info->ssid,link_status.ssid,ssid_len);
    os_memcpy(bssid_info->bssid, link_status.bssid, 6);
    bssid_info->security = link_status.security;
    bssid_info->channel = link_status.channel;

    psk = wpas_get_sta_psk();
    os_memset(temp, 0, sizeof(temp));
    for(i = 0; i < 32; i++)
    {
        sprintf(temp, "%02x", psk[i]);
        strcat(bssid_info->psk, temp);
    }
    os_strcpy(bssid_info->pwd, g_rl_sta_key);

#if (CFG_SUPPORT_DJYOS)
    wlan_fast_connect_info_write(bssid_info);
#else
    rl_read_bssid_info(bssid_rd);
    if(memcmp(bssid_rd,bssid_info, sizeof(RL_BSSID_INFO_T))!= 0)
    {
        rt_kprintf("=== it's different,write===\r\n");
        flash_hdl = ddev_open(FLASH_DEV_NAME, &status, 0);
        ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, (void *)&protect_flag);
        protect_param = FLASH_PROTECT_NONE;
        ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, (void *)&protect_param);
        addr = BSSID_INFO_ADDR;
        ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, (void *)&addr);
        ddev_write(flash_hdl, bssid_info, sizeof(RL_BSSID_INFO_T),addr);
        ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, (void *)&protect_flag);
        ddev_close(flash_hdl);
    }

#endif
    if(bssid_info)
        rt_free(bssid_info);
    if(bssid_rd)
        rt_free(bssid_rd);
}


static void rl_sta_fast_connect(RL_BSSID_INFO_PTR bssid_info)
{
    network_InitTypeDef_adv_st inNetworkInitParaAdv;

    os_memset(&inNetworkInitParaAdv, 0, sizeof(inNetworkInitParaAdv));

    os_strcpy((char*)inNetworkInitParaAdv.ap_info.ssid, bssid_info->ssid);
    os_memcpy(inNetworkInitParaAdv.ap_info.bssid, bssid_info->bssid, 6);
    inNetworkInitParaAdv.ap_info.security = bssid_info->security;
    inNetworkInitParaAdv.ap_info.channel = bssid_info->channel;
//#if (CFG_SUPPORT_RTT)
    if(bssid_info->security < BK_SECURITY_TYPE_WPA_TKIP)
    {
        os_strcpy((char*)inNetworkInitParaAdv.key, bssid_info->pwd);
        inNetworkInitParaAdv.key_len = os_strlen(bssid_info->pwd);
    }
    else
//#endif
    {
        os_strcpy((char*)inNetworkInitParaAdv.key, bssid_info->psk);
        inNetworkInitParaAdv.key_len = os_strlen(bssid_info->psk);
    }
    inNetworkInitParaAdv.dhcp_mode = DHCP_CLIENT;

    bk_wlan_start_sta_adv(&inNetworkInitParaAdv);
}
#endif

uint32_t rl_launch_sta(void)
{
    uint32_t next_launch_flag = 0;
    RL_ENTITY_T *entity, *pre_entity;
    uint32_t ret = LAUNCH_STATUS_OVER;

    if(NULL == g_role_launch.jl_previous_sta)
    {
        if(g_role_launch.jl_following_sta)
        {
            pre_entity = g_role_launch.jl_following_sta;
            g_role_launch.jl_previous_sta = pre_entity;
            g_role_launch.jl_following_sta = NULL;

            rl_sta_request_start(&pre_entity->rlaunch);

            ret = LAUNCH_STATUS_CONT;
        }
    }
    else
    {
        if(g_role_launch.jl_following_sta)
        {
            entity = g_role_launch.jl_following_sta;
            if(LAUNCH_TYPE_ASAP == entity->launch_type)
            {
                rl_pre_sta_set_cancel();
            }
        }

        pre_entity = g_role_launch.jl_previous_sta;
        if(pre_entity->relaunch_limit
                && (pre_entity->launch_count >= pre_entity->relaunch_limit))
        {
            rl_pre_sta_set_cancel();
        }

        next_launch_flag = rl_sta_may_next_launch();
        if(next_launch_flag)
        {
            rl_pre_sta_stop_launch();

            pre_entity = g_role_launch.jl_previous_sta;
            rl_free_entity(pre_entity);

            pre_entity = g_role_launch.jl_following_sta;
            g_role_launch.jl_previous_sta = pre_entity;
            g_role_launch.jl_following_sta = NULL;

            if(pre_entity)
            {
                rl_sta_request_start(&pre_entity->rlaunch);

                ret = LAUNCH_STATUS_CONT;
            }
        }
        else
        {
            ret = LAUNCH_STATUS_CONT;
        }
    }

    return ret;
}

uint32_t rl_launch_ap(void)
{
    uint32_t cancel_pre_flag = 0;
    uint32_t next_launch_flag = 0;
    RL_ENTITY_T *entity, *pre_entity;
    uint32_t ret = LAUNCH_STATUS_OVER;

    if(NULL == g_role_launch.jl_previous_ap)
    {
        if(g_role_launch.jl_following_ap)
        {
            pre_entity = g_role_launch.jl_following_ap;
            g_role_launch.jl_previous_ap = pre_entity;
            g_role_launch.jl_following_ap = NULL;

            rl_pre_ap_init();
            rl_ap_request_start(&pre_entity->rlaunch);

            ret = LAUNCH_STATUS_CONT;
        }
    }
    else
    {
        if(g_role_launch.jl_following_ap)
        {
            entity = g_role_launch.jl_following_ap;
            if(LAUNCH_TYPE_ASAP == entity->launch_type)
            {
                cancel_pre_flag = 1;
                rl_pre_ap_set_cancel();
            }
        }

        pre_entity = g_role_launch.jl_previous_ap;
        if(pre_entity->relaunch_limit
                && (pre_entity->launch_count >= pre_entity->relaunch_limit))
        {
            cancel_pre_flag = 1;
            rl_pre_ap_set_cancel();
        }

        next_launch_flag = rl_ap_may_next_launch(cancel_pre_flag);
        if(next_launch_flag)
        {
            rl_pre_ap_stop_launch();

            pre_entity = g_role_launch.jl_previous_ap;
            rl_free_entity(pre_entity);

            pre_entity = g_role_launch.jl_following_ap;
            g_role_launch.jl_previous_ap = pre_entity;
            g_role_launch.jl_following_ap = NULL;

            if(pre_entity)
            {
                rl_pre_ap_init();
                rl_ap_request_start(&pre_entity->rlaunch);

                ret = LAUNCH_STATUS_CONT;
            }
        }
        else
        {
            ret = LAUNCH_STATUS_CONT;
        }
    }

    return ret;
}

uint32_t rl_relaunch_chance(void)
{
    ASSERT(rtos_is_oneshot_timer_init(&g_role_launch.rl_timer));
#if (CFG_SUPPORT_ALIOS)
    rtos_start_oneshot_timer(&g_role_launch.rl_timer);
#else
    rtos_oneshot_reload_timer(&g_role_launch.rl_timer);
#endif
    return 0;
}

uint32_t rl_sta_req_is_null(void)
{
    uint32_t ret = 0;
    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();
    if((0 == g_role_launch.jl_previous_sta)
        && (0 == g_role_launch.jl_following_sta))
    {
        ret = 1;
    }
    GLOBAL_INT_RESTORE();

    return ret;
}

uint32_t _sta_request_enter(LAUNCH_REQ *param, FUNC_1PARAM_PTR completion)
{
    uint32_t ret = 0;
    RL_ENTITY_T *entity = 0;
    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();
    if((0 == g_role_launch.jl_previous_ap)
        && (0 == g_role_launch.jl_previous_sta))
    {
        entity = rl_alloc_entity(param, completion);

        g_role_launch.jl_previous_sta = entity;

        JL_PRT("rl_sta_start\r\n");
        rl_sta_request_start(param);

        ret = 1;
    }
    else if(0 == g_role_launch.jl_following_sta)
    {
        entity = rl_alloc_entity(param, completion);

        g_role_launch.jl_following_sta = entity;
        rl_start();
        ret = 2;
    }
    else
    {
        os_printf("cmd queue fill!\n");
        rl_start();
    }

    if(entity
        && (PRE_ENTITY_IDLE == g_role_launch.pre_entity_type))
    {
        g_role_launch.pre_entity_type = PRE_ENTITY_STA;
    }
    GLOBAL_INT_RESTORE();

    return ret;
}

uint32_t _ap_request_enter(LAUNCH_REQ *param, FUNC_1PARAM_PTR completion)
{
    uint32_t ret = 0;
    RL_ENTITY_T *entity = 0;
    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();
    if((0 == g_role_launch.jl_previous_ap)
        && (0 == g_role_launch.jl_previous_sta))
    {
        entity = rl_alloc_entity(param, completion);

        g_role_launch.jl_previous_ap = entity;

        JL_PRT("rl_ap_start\r\n");
        rl_ap_request_start(param);

        ret = 1;
    }
    else if(0 == g_role_launch.jl_following_ap)
    {
        entity = rl_alloc_entity(param, completion);

        g_role_launch.jl_following_ap = entity;
        rl_start();

        ret = 2;
    }
    else
    {
        os_printf("cmd queue fil2!\n");
        rl_start();
    }

    if(entity
        && (PRE_ENTITY_IDLE == g_role_launch.pre_entity_type))
    {
        g_role_launch.pre_entity_type = PRE_ENTITY_AP;
    }
    GLOBAL_INT_RESTORE();

    return ret;
}

void rl_enter_handler(void *left, void *right)
{
    uint8_t *ptr;
    uint32_t ret;
    LAUNCH_REQ *ap_param;
    LAUNCH_REQ *sta_param;
    uint32_t hit_ap, hit_sta;
    FUNC_1PARAM_PTR ap_completion;
    FUNC_1PARAM_PTR sta_completion;

    GLOBAL_INT_DECLARATION();

    JL_PRT("rl_enter_handler\r\n");
    ptr = (uint8_t *)os_malloc(sizeof(*ap_param) + sizeof(*sta_param));
    if(0 == ptr)
    {
        return;
    }
    ap_param = (LAUNCH_REQ *)ptr;
    sta_param = (LAUNCH_REQ *)&((LAUNCH_REQ *)ptr)[1];

    GLOBAL_INT_DISABLE();
    ap_completion = g_rl_socket.ap_completion;
    *ap_param = g_rl_socket.ap_param;
    hit_ap = g_rl_socket.ap_req_flag;

    sta_completion = g_rl_socket.sta_completion;
    *sta_param = g_rl_socket.sta_param;
    hit_sta = g_rl_socket.sta_req_flag;

    os_memset(&g_rl_socket, 0, sizeof(g_rl_socket));
    GLOBAL_INT_RESTORE();

    if(hit_sta)
    {
        JL_PRT("_sta_request_enter\r\n");
        ret = _sta_request_enter(sta_param, sta_completion);
        if(ret)
        {
            g_sta_cache.sta_completion = sta_completion;
            g_sta_cache.sta_param = *sta_param;
            g_sta_cache.sta_req_flag = 1;
        }
    }

    if(hit_ap)
    {
        JL_PRT("_ap_request_enter\r\n");
        _ap_request_enter(ap_param, ap_completion);
    }

    os_free(ptr);
    ptr = 0;
}

uint32_t rl_sta_cache_request_enter(void)
{
    uint32_t ret = 0;
    GLOBAL_INT_DECLARATION();

    JL_PRT("sta_cache_request\r\n");
    GLOBAL_INT_DISABLE();
    if(g_sta_cache.sta_req_flag && (0 == bk_wlan_is_monitor_mode()))
    {
        rl_sta_request_enter(&g_sta_cache.sta_param, g_sta_cache.sta_completion);

        ret = 1;
    }

    os_memset(&g_sta_cache, 0, sizeof(g_sta_cache));

    GLOBAL_INT_RESTORE();

    return ret;
}

void rl_launch_handler(void *left, void *right)
{
    uint32_t ap_ret = LAUNCH_STATUS_OVER;
    uint32_t sta_ret = LAUNCH_STATUS_OVER;

    JL_PRT("rl_launch_handler\r\n");
    switch(g_role_launch.pre_entity_type)
    {
        case PRE_ENTITY_AP:
            ap_ret = rl_launch_ap();
            if(LAUNCH_STATUS_OVER == ap_ret)
            {
                g_role_launch.pre_entity_type = PRE_ENTITY_STA;

                sta_ret = rl_launch_sta();
            }
            break;

        case PRE_ENTITY_STA:
            sta_ret = rl_launch_sta();
            if(LAUNCH_STATUS_OVER == sta_ret)
            {
                g_role_launch.pre_entity_type = PRE_ENTITY_AP;

                ap_ret = rl_launch_ap();
            }
            break;

        case PRE_ENTITY_IDLE:
            break;

        default:
            break;
    }

    if(!((LAUNCH_STATUS_OVER == sta_ret)
        && (LAUNCH_STATUS_OVER == ap_ret)))
    {
        rl_relaunch_chance();
    }
}

void rl_init(void)
{
    OSStatus err = kNoErr;

    JL_PRT("rl_init\r\n");
    err = rtos_init_oneshot_timer(&g_role_launch.rl_timer,
                                    RL_LAUNCH_PERIOD,
                                    (timer_2handler_t)rl_launch_handler,
                                    NULL,
                                    NULL);
    ASSERT(kNoErr == err);

    err = rtos_init_oneshot_timer(&g_role_launch.enter_timer,
                                    RL_ENTER_PERIOD,
                                    (timer_2handler_t)rl_enter_handler,
                                    NULL,
                                    NULL);
    ASSERT(kNoErr == err);
    g_role_launch.rl_timer_flag = RL_TIMER_INIT;

#if RL_SUPPORT_FAST_CONNECT//chenws: 这里在获取IP成功后直接调用rl_write_bssid_info
    //user_connected_callback(rl_write_bssid_info);
#endif
}

void rl_uninit(void)
{
    OSStatus err = kNoErr;

    JL_PRT("rl_uninit\r\n");

    if(RL_TIMER_START == g_role_launch.rl_timer_flag)
    {
        rl_stop();
    }

    err = rtos_deinit_oneshot_timer(&g_role_launch.rl_timer);
    ASSERT(kNoErr == err);

    err = rtos_deinit_oneshot_timer(&g_role_launch.enter_timer);
    ASSERT(kNoErr == err);

    g_role_launch.rl_timer_flag = RL_TIMER_UNINIT;
}

void rl_start(void)
{
    OSStatus err = kNoErr;

    JL_PRT("rl_timer-stop\r\n");
    err = rtos_stop_oneshot_timer(&g_role_launch.rl_timer);
    if(kNoErr != err)
    {
        JL_PRT("ERR:%d\r\n", err);
    }
    g_role_launch.rl_timer_flag = RL_TIMER_STOP;

    JL_PRT("rl_start_timer\r\n");
    err = rtos_start_oneshot_timer(&g_role_launch.rl_timer);
    if(kNoErr != err)
    {
        JL_PRT("err:%d\r\n", err);
    }

    ASSERT(kNoErr == err);

    g_role_launch.rl_timer_flag = RL_TIMER_START;
}

void rl_stop(void)
{
    OSStatus err = kNoErr;

    err = rtos_stop_oneshot_timer(&g_role_launch.rl_timer);
    ASSERT(kNoErr == err);

    JL_PRT("rl_stop\r\n");
    err = rtos_stop_oneshot_timer(&g_role_launch.enter_timer);
    ASSERT(kNoErr == err);

    g_role_launch.rl_timer_flag = RL_TIMER_STOP;
}

RL_ENTITY_T *rl_alloc_entity(LAUNCH_REQ *param, FUNC_1PARAM_PTR completion)
{
    RL_ENTITY_T *entity;

    entity = (RL_ENTITY_T *)os_zalloc(sizeof(RL_ENTITY_T));
    ASSERT(entity);

    entity->completion_cb = completion;
    entity->launch_type = LAUNCH_TYPE_ASAP;
    os_memcpy(&entity->rlaunch, param, sizeof(LAUNCH_REQ));

    return entity;
}

void rl_free_entity(RL_ENTITY_T *d)
{
    os_free(d);
}

uint32_t rl_pre_sta_stop_launch(void)
{
    return 0;
}

uint32_t rl_pre_ap_stop_launch(void)
{
    return 0;
}

uint32_t rl_sta_is_launched(void)
{
    return ((RL_STATUS_MROLE_CHANNEL_SWITCHED == g_mrole_csa_flag)
                || (RL_STATUS_STA_LAUNCHED == rl_pre_sta_get_status()));
}

uint32_t rl_sta_may_next_launch(void)
{
    uint32_t yes = 0;

    if(RL_STATUS_MROLE_CHANNEL_SWITCHED == g_mrole_csa_flag)
    {
        yes = 1;
    }
    else if(g_role_launch.pre_sta_cancel
        && (RL_STATUS_STA_DHCPING == rl_pre_sta_get_status()
                || RL_STATUS_STA_INITING == rl_pre_sta_get_status()
                || RL_STATUS_STA_SCAN_OVER == rl_pre_sta_get_status()
                || RL_STATUS_STA_CONNECTING == rl_pre_sta_get_status()))
    {
        yes = 2;
    }
    else if(RL_STATUS_STA_LAUNCH_FAILED == rl_pre_sta_get_status())
    {
        yes = 3;
    }
    else if(RL_STATUS_STA_LAUNCHED == rl_pre_sta_get_status())
    {
        yes = 4;
    }

    if(RL_STATUS_MROLE_CHANNEL_SWITCHING == g_mrole_csa_flag)
    {
        yes = 0;
    }

    return yes;
}

uint32_t rl_ap_may_next_launch(uint32_t pre_cancel)
{
    uint32_t yes = 0;

    if(0 == pre_cancel)
    {
        yes = 1;
        goto exit_check;
    }

    if(((RL_STATUS_AP_TRANSMITTED_BCN + RL_STATUS_CANCEL) == rl_pre_ap_get_status())
        || (RL_STATUS_AP_LAUNCHED == (rl_pre_ap_get_status() & (~RL_STATUS_CANCEL))))
    {
        yes = 1;
    }

    if(RL_STATUS_MROLE_CHANNEL_SWITCHING == g_mrole_csa_flag)
    {
        yes = 0;
    }

exit_check:
    return yes;
}

uint32_t rl_pre_sta_get_status(void)
{
    return g_role_launch.pre_sta_status;
}

uint32_t rl_pre_ap_get_status(void)
{
    return g_role_launch.pre_ap_status;
}

uint32_t rl_init_csa_status(void)
{
    JL_PRT("rl_init_csa_status\r\n");
    g_mrole_csa_flag = 0;

    return 0;
}

uint32_t rl_set_csa_switching(void)
{
    uint32_t ret = RET_RL_SUCCESS;

    if(g_role_launch.pre_sta_cancel || g_role_launch.pre_ap_cancel)
    {
        ret = RET_RL_FAILURE;
    }
    else
    {
        g_mrole_csa_flag = RL_STATUS_MROLE_CHANNEL_SWITCHING;
    }
    JL_PRT("rl_set_csa_switching:%d\r\n", ret);

    return ret;
}

uint32_t rl_set_csa_switched(void)
{
    g_mrole_csa_flag = RL_STATUS_MROLE_CHANNEL_SWITCHED;
    JL_PRT("rl_set_csa_switched\r\n");

    return RET_RL_SUCCESS;
}

uint32_t rl_pre_sta_set_status(uint32_t status)
{
    uint32_t cancel = 0;
    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();
    if((RL_STATUS_STA_SCANNING == status)
        && ((RL_STATUS_STA_INITING != g_role_launch.pre_sta_status)
                && (RL_STATUS_STA_SCAN_OVER != g_role_launch.pre_sta_status)))
    {
        cancel = 6;
        JL_PRT("exceptional_status:%d:%d\r\n", g_role_launch.pre_sta_status, status);
        goto set_out;
    }

    if((RL_STATUS_STA_SCANNING == status)
        && g_role_launch.pre_sta_cancel)
    {
        cancel = 8;
        JL_PRT("cacel_scanning:%d:%d\r\n", g_role_launch.pre_sta_status, status);
        goto set_out;
    }

    g_role_launch.pre_sta_status = status;
    cancel = g_role_launch.pre_sta_cancel;

set_out:
    GLOBAL_INT_RESTORE();


    return cancel;
}

uint32_t rl_pre_ap_disable_autobcn(void)
{
    if(g_role_launch.pre_ap_cancel)
    {
        mm_hw_ap_disable();
    }

    return g_role_launch.pre_ap_cancel;
}

uint32_t rl_pre_ap_set_status(uint32_t status)
{
    uint32_t cancel = 0;
    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();
    g_role_launch.pre_ap_status = status;
    cancel = g_role_launch.pre_ap_cancel;
    if(cancel)
    {
        g_role_launch.pre_ap_status += RL_STATUS_CANCEL;
    }

    GLOBAL_INT_RESTORE();

    return cancel;
}

void rl_pre_sta_set_cancel(void)
{
    g_role_launch.pre_sta_cancel = 1;
}

void rl_pre_sta_clear_cancel(void)
{
    g_role_launch.pre_sta_cancel = 0;
}

uint32_t fl_get_pre_sta_cancel_status(void)
{
    return g_role_launch.pre_sta_cancel;
}

void rl_pre_ap_set_cancel(void)
{
    g_role_launch.pre_ap_cancel = 1;
}

void rl_pre_ap_clear_cancel(void)
{
    g_role_launch.pre_ap_cancel = 0;
}

uint32_t fl_get_pre_ap_cancel_status(void)
{
    return g_role_launch.pre_ap_cancel;
}

void rl_pre_ap_init(void)
{
    g_role_launch.pre_ap_cancel = 0;
    g_role_launch.pre_ap_status = 0;
}

void rl_pre_sta_init(void)
{
    g_role_launch.pre_sta_cancel = 0;
    g_role_launch.pre_sta_status = 0;
}

void rl_sta_request_start(LAUNCH_REQ *req)
{
    extern void demo_scan_app_init(void);
#if RL_SUPPORT_FAST_CONNECT
    RL_BSSID_INFO_T bssid_info;
    uint32_t ssid_len;
#endif

    ASSERT(req);

    switch(req->req_type)
    {
        case LAUNCH_REQ_STA:
            rl_pre_sta_init();
        #if RL_SUPPORT_FAST_CONNECT

            if(rl_cnt < MAX_FASTCONN_RETRY_CNT)//avoid psk invalid,switch to normal connect
            {
                rl_cnt ++;
                os_memset(&g_rl_sta_key, 0, sizeof(g_rl_sta_key));
                os_strcpy(g_rl_sta_key, req->descr.wifi_key);
#if (CFG_SUPPORT_DJYOS == 1) //chenws: 修改这里支持多ssid和pwd支持快连信息
                int wlan_fast_info_match(char *ssid, char *passwd, RL_BSSID_INFO_PTR bssid_info);
                if (wlan_fast_info_match(req->descr.wifi_ssid, req->descr.wifi_key, &bssid_info)) {
                    bk_printf("fast_connect\r\n");
                    rl_sta_fast_connect(&bssid_info);
                }
                else {
                    bk_printf("normal_connect11\r\n");
                    bk_wlan_start_sta(&req->descr);
                }
#else
                rl_read_bssid_info(&bssid_info);
                ssid_len = os_strlen(bssid_info.ssid);
                if(ssid_len > SSID_MAX_LEN)
                {
                    ssid_len = SSID_MAX_LEN;
                }

                if(os_memcmp(req->descr.wifi_ssid, bssid_info.ssid,ssid_len) == 0
                    && os_strcmp(req->descr.wifi_key, bssid_info.pwd) == 0)
                {
                    bk_printf("fast_connect\r\n");
                    rl_sta_fast_connect(&bssid_info);
                }
                else
                {
                    bk_printf("normal_connect11\r\n");
                    bk_wlan_start_sta(&req->descr);
                }
#endif

            }
            else
            {
                bk_printf("normal_connect22\r\n");
                bk_wlan_start_sta(&req->descr);
            }
            #else
            bk_printf("normal_connect\r\n");
            bk_wlan_start_sta(&req->descr);
            #endif
            break;

        case LAUNCH_REQ_PURE_STA_SCAN:
            rl_pre_sta_init();
#if (!CFG_SUPPORT_ALIOS)
            demo_scan_app_init();
#endif
            break;

        case LAUNCH_REQ_DELIF_STA:
            bk_wlan_stop(BK_STATION);
            break;

        default:
            break;
    }
}

void rl_ap_request_start(LAUNCH_REQ *req)
{
    ASSERT(req);

    switch(req->req_type)
    {
        case LAUNCH_REQ_AP:
            rl_pre_ap_init();
            bk_wlan_start_ap(&req->descr);
            break;

        case LAUNCH_REQ_DELIF_AP:
            bk_wlan_stop(BK_SOFT_AP);
            break;

        default:
            break;
    }
}

void rl_sta_request_enter(LAUNCH_REQ *param, FUNC_1PARAM_PTR completion)
{
    OSStatus err = kNoErr;

    GLOBAL_INT_DECLARATION();

    JL_PRT("rl_sta_request_enter\r\n");
    GLOBAL_INT_DISABLE();
    g_rl_socket.sta_completion = completion;
    g_rl_socket.sta_param = *param;
    g_rl_socket.sta_req_flag = 1;

    JL_PRT("enter_timer-Sstop\r\n");
    err = rtos_stop_oneshot_timer(&g_role_launch.enter_timer);
    if(kNoErr != err)
    {
        JL_PRT("enter_timer-Sstop:0x%x\r\n", err);
    }

    JL_PRT("enter_timer-Sstart\r\n");
    err = rtos_start_oneshot_timer(&g_role_launch.enter_timer);
    if(kNoErr != err)
    {
        JL_PRT("enter_timer-Sstart:0x%x\r\n", err);
    }

    ASSERT(kNoErr == err);
    GLOBAL_INT_RESTORE();
}

void rl_ap_request_enter(LAUNCH_REQ *param, FUNC_1PARAM_PTR completion)
{
    OSStatus err = kNoErr;

    GLOBAL_INT_DECLARATION();

    JL_PRT("rl_ap_request_enter\r\n");

    GLOBAL_INT_DISABLE();
    g_rl_socket.ap_completion = completion;
    g_rl_socket.ap_param = *param;
    g_rl_socket.ap_req_flag = 1;

    JL_PRT("enter_timer-Astop\r\n");
    rtos_stop_oneshot_timer(&g_role_launch.enter_timer);

    JL_PRT("enter_timer-Astart\r\n");
    err = rtos_start_oneshot_timer(&g_role_launch.enter_timer);
    ASSERT(kNoErr == err);
    GLOBAL_INT_RESTORE();
}

#endif // CFG_ROLE_LAUNCH

// eof

