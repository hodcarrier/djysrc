#ifndef _FAKE_CLOCK_PUB_H_
#define _FAKE_CLOCK_PUB_H_

#include "include.h"

#define FCLK_PWM_ID           PWM0
#if (CFG_SOC_NAME != SOC_BK7231)
#define FCLK_TIMER_ID           BKTIMER3
#define CAL_TIMER_ID           BKTIMER2
#endif
#if (CFG_SUPPORT_RTT)
#define FCLK_DURATION_MS      (1000 / RT_TICK_PER_SECOND)
#define FCLK_SECOND           (RT_TICK_PER_SECOND)
#endif
#if (CFG_SUPPORT_DJYOS)
#include "project_config.h"
#define FCLK_DURATION_MS      (1000 / CN_CFG_TICK_HZ)
#define FCLK_SECOND           (CN_CFG_TICK_HZ)
#else
#define FCLK_DURATION_MS      2
#define FCLK_SECOND           (1000/FCLK_DURATION_MS)
#endif
#define TICK_PER_SECOND       FCLK_SECOND

#define BK_MS_TO_TICKS(x)     ((x) / (FCLK_DURATION_MS))
#define BK_TICKS_TO_MS(x)     ((x) * (FCLK_DURATION_MS))

extern UINT64 fclk_get_tick(void);
extern UINT32 fclk_get_second(void);
extern void fclk_reset_count(void);
extern void fclk_init(void);
extern UINT32 fclk_from_sec_to_tick(UINT32 sec);
extern UINT32 fclk_cal_endvalue(UINT32 mode);

#endif // _FAKE_CLOCK_PUB_H_
// eof

