/*
 * WPA Supplicant / main() function for UNIX like OSes and MinGW
 * Copyright (c) 2003-2013, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */
#include "includes.h"
#include "common.h"
#include "fst/fst.h"
#include "wpa_supplicant_i.h"
#include "driver_i.h"
#include "main_none.h"
#include "ps.h"
#include <misc/md5.h>
#include "sys_rtos.h"
#include "rtos_pub.h"
#include "error.h"
#include "uart_pub.h"
#include "signal.h"
#include "eloop.h"
#include "config.h"
#if CFG_SUPPOET_BSSID_CONNECT
#include "param_config.h"
#endif
#include "eloop.h"
//#include "wpa_psk_cache.h"
static struct wpa_global *wpa_global_ptr = NULL;
beken_thread_t wpas_thread_handle = NULL;
uint32_t wpas_stack_size = 4000;
beken_semaphore_t wpas_sema = NULL;
struct wpa_ssid_value *wpas_connect_ssid = 0;
struct wpa_interface *wpas_ifaces = 0;
uint32_t supplicant_exit_flag = 0;

extern beken_queue_t wpah_queue;

extern void wpas_thread_start(void);
extern void wpas_thread_stop(void);
extern void wpa_handler_signal(void *arg, u8 vif_idx);

int wpa_get_psk(char *psk)
{
    struct wpa_config *conf = NULL;

    if(!psk)
    {
        return -1;
    }
    memset(psk, 0, 32);
    conf = wpa_global_ptr->ifaces->conf;
    memcpy(psk, conf->ssid->psk, 32);

    return 0;
}

static unsigned char gmd5_passphrase[16];
int wpa_get_passphrase_md5(unsigned char *md5_passphrase, int len)
{
    int min = len;

    if (md5_passphrase == 0 || len <= 0) return -1;

    min = min < sizeof(gmd5_passphrase) ? min : sizeof(gmd5_passphrase);
    memcpy(md5_passphrase, gmd5_passphrase, min);

    //hex_dump("wpa_get_passphrase_md5", md5_passphrase, min);

    return 0;
}

int wpa_set_passphrase_md5(char *passphrase)
{
    unsigned char md5_tmp[16];
    MD5_CTX ctx;
    int min = sizeof(gmd5_passphrase);

    if (passphrase == 0) return -1;

    memset(md5_tmp,  0,  sizeof(md5_tmp));
    MD5Init(&ctx);
    MD5Update(&ctx, passphrase, strlen(passphrase));
    MD5Final(md5_tmp, &ctx);

    //printf("==wpa_set_passphrase_md5: %s==\r\n", passphrase);

    min = min < sizeof(md5_tmp) ? min : sizeof(md5_tmp);
    memcpy(gmd5_passphrase, md5_tmp, min);

    //hex_dump("wpa_set_passphrase_md5", gmd5_passphrase, min);
    return 0;
}


int supplicant_exit_done(void)
{
	supplicant_exit_flag = 0;
	os_printf("supplicant_exit_done\r\n");
	
	return 0;
}

int supplicant_is_exiting(void)
{
	return supplicant_exit_flag;
}

int supplicant_main_exit(void)
{
	supplicant_exit_flag = 1;

	wpa_hostapd_queue_poll(0xff);
	while(supplicant_exit_flag)
	{
		os_printf("supplicant_main_exiting\r\n");
		rtos_delay_milliseconds(10);
	}

	return 0;
}

int supplicant_exit_handler(void)
{
	if (wpa_global_ptr == NULL)
		return 0;
	
    if(wpa_global_ptr)
    {
        wpa_supplicant_deinit(wpa_global_ptr);
        wpa_global_ptr = NULL;
    }

    if(wpas_ifaces)
    {
        os_free(wpas_ifaces);
        wpas_ifaces = 0;
    }

    if(wpas_connect_ssid)
    {
        os_free(wpas_connect_ssid);
        wpas_connect_ssid = 0;
    }

    return 0;
}

u8 supplicant_main_is_exit(void)
{
    return (wpa_global_ptr==NULL)?1:0;
}

int supplicant_main_entry(char *oob_ssid)
{
    int i;
    int iface_count, exitcode = -1;
    struct wpa_params params;
    struct wpa_supplicant *wpa_s;
    struct wpa_interface *iface;

    os_memset(&params, 0, sizeof(params));
    params.wpa_debug_level = MSG_INFO;

    if(0 == wpas_ifaces)
    {
        wpas_ifaces = os_zalloc(sizeof(struct wpa_interface));
        if (wpas_ifaces == NULL)
            return -1;
    }

    iface = wpas_ifaces;
    iface_count = 1;
    iface->ifname = bss_iface;
    exitcode = 0;

    wpa_global_ptr = wpa_supplicant_init(&params);
    if (wpa_global_ptr == NULL)
    {
        wpa_printf(MSG_ERROR, "Failed to initialize wpa_supplicant");
        exitcode = -1;
        goto out;
    }
    else
    {
        wpa_printf(MSG_INFO, "Successfully initialized wpa_supplicant");
    }

    for (i = 0; exitcode == 0 && i < iface_count; i++)
    {
        if (wpas_ifaces[i].ctrl_interface == NULL &&
                wpas_ifaces[i].ifname == NULL)
        {
            if (iface_count == 1
                && (params.ctrl_interface
                    || params.dbus_ctrl_interface))
            {
                break;
            }

            exitcode = -1;
            break;
        }

        wpa_s = wpa_supplicant_add_iface(wpa_global_ptr, &wpas_ifaces[i], NULL);
        if (wpa_s == NULL)
        {
            exitcode = -1;
            break;
        }


#if CFG_SUPPOET_BSSID_CONNECT
        if ((NULL == oob_ssid || 0 == os_strlen(oob_ssid))
        && ((g_sta_param_ptr->fast_connect.bssid[0] != 0xFF)
         || (g_sta_param_ptr->fast_connect.bssid[1] != 0xFF)
         || (g_sta_param_ptr->fast_connect.bssid[2] != 0xFF)
         || (g_sta_param_ptr->fast_connect.bssid[3] != 0xFF)
         || (g_sta_param_ptr->fast_connect.bssid[4] != 0xFF)
         || (g_sta_param_ptr->fast_connect.bssid[5] != 0xFF))
       && ((g_sta_param_ptr->fast_connect.bssid[0] != 0x0)
         || (g_sta_param_ptr->fast_connect.bssid[1] != 0x0)
         || (g_sta_param_ptr->fast_connect.bssid[2] != 0x0)
         || (g_sta_param_ptr->fast_connect.bssid[3] != 0x0)
         || (g_sta_param_ptr->fast_connect.bssid[4] != 0x0)
         || (g_sta_param_ptr->fast_connect.bssid[5] != 0x0)))
        {
            ASSERT(0 == wpa_s->ssids_from_scan_req);

            if(0 == wpas_connect_ssid)
            {
                wpas_connect_ssid = (struct wpa_ssid_value *)os_malloc(sizeof(struct wpa_ssid_value));
                ASSERT(wpas_connect_ssid);
            }

            os_memset(wpas_connect_ssid, 0x00, sizeof(*wpas_connect_ssid));
            os_memcpy(wpas_connect_ssid->bssid, g_sta_param_ptr->fast_connect.bssid, sizeof(wpas_connect_ssid->bssid));

            wpa_s->num_ssids_from_scan_req = 1;
            wpa_s->ssids_from_scan_req = wpas_connect_ssid;
            wpa_s->scan_req = MANUAL_SCAN_REQ;
            os_printf("MANUAL_SCAN_REQ with %02x-%02x-%02x-%02x-%02x-%02x\n",
                      wpas_connect_ssid->bssid[0],
                      wpas_connect_ssid->bssid[1],
                      wpas_connect_ssid->bssid[2],
                      wpas_connect_ssid->bssid[3],
                      wpas_connect_ssid->bssid[4],
                      wpas_connect_ssid->bssid[5]);
        }
        else
#endif
        if(oob_ssid)
        {
            int len;
            int oob_ssid_len;

            ASSERT(0 == wpa_s->ssids_from_scan_req);
            oob_ssid_len = os_strlen(oob_ssid);

            if(0 == wpas_connect_ssid)
            {
                wpas_connect_ssid = (struct wpa_ssid_value *)os_malloc(sizeof(struct wpa_ssid_value));
                ASSERT(wpas_connect_ssid);
            }

            len = MIN(SSID_MAX_LEN, oob_ssid_len);

            wpas_connect_ssid->ssid_len = len;
            os_memcpy(wpas_connect_ssid->ssid, oob_ssid, len);

            wpa_s->num_ssids_from_scan_req = 1;
            wpa_s->ssids_from_scan_req = wpas_connect_ssid;
            wpa_s->scan_req = MANUAL_SCAN_REQ;
            os_printf("MANUAL_SCAN_REQ\r\n");
        }
    }

    if (exitcode)
    {
        wpa_supplicant_deinit(wpa_global_ptr);
    }
    else
    {
        wpa_supplicant_run(wpa_global_ptr);

        return 0;
    }

out:
    os_free(wpas_ifaces);
    wpas_ifaces = 0;

    os_free(params.pid_file);
    params.pid_file = 0;

    return exitcode;
}

static void wpas_thread_main( void *arg )
{
//	wpa_psk_init();
	
    eloop_init();

    eloop_run();

    wpas_thread_handle = NULL;

    rtos_deinit_queue(&wpah_queue);
    wpah_queue = NULL;

    rtos_delete_thread(NULL);
}

void wpas_thread_start(void)
{
    OSStatus ret;

    if(wpah_queue == NULL) {
        ret = rtos_init_queue(&wpah_queue,
                                "wpah_queue",
                                sizeof(WPAH_MSG_ST),
                                64);
        ASSERT(kNoErr == ret);
    }

    if(NULL == wpas_thread_handle)
    {
        ret = rtos_create_thread(&wpas_thread_handle,
                                 THD_WPAS_PRIORITY,
                                 "wpas_thread",
                                 (beken_thread_function_t)wpas_thread_main,
                                 (unsigned short)wpas_stack_size,
                                 (beken_thread_arg_t)NULLPTR);
        ASSERT(kNoErr == ret);
    }
}

void wpas_thread_stop(void)
{
    wpa_handler_signal((void*)SIGTERM, 0xff);

    while(wpas_thread_handle != NULL) {
        bk_rtos_delay_milliseconds(10);
    }
}

void wpa_supplicant_poll(void *param)
{
    OSStatus ret;

    if(wpas_sema)
    {
        ret = bk_rtos_set_semaphore(&wpas_sema);
    }

    (void)ret;
}

int wpa_sem_wait(uint32_t ms)
{
    if(NULL == wpas_sema)
    {
        return kTimeoutErr;
    }

    return bk_rtos_get_semaphore(&wpas_sema, ms);
}

u8* wpas_get_sta_psk(void)
{
    return wpa_global_ptr->ifaces->conf->ssid->psk;
}
// eof


