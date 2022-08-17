/*
 * silan_gpio.c
 *
 * Modify Date: 
*/
#define MODULE_NAME          "SILAN GPIO"

#include "ap1508.h"
#include "silan_gpio.h"
#include "silan_printf.h"
#include "silan_irq.h"
#include "silan_syscfg.h"
#include "silan_syscfg_regs.h"
#include "silan_iomux.h"

#define GPIO_CONFIG_OFFSET(x)    (x / (GPIO1_MAX_NUM+1))
#define GPIO_CONFIG_PIN(x)       (x % (GPIO1_MAX_NUM+1))
#define IO_TO_GPIO(x)            ((x>=8) ? (x-8) : (34+x))

void silan_gpio_input(u32 addr, u32 pin)
{
	__sREG32(addr, GPIO_DIR) &= ~pin;
}
void silan_gpio_output(u32 addr, u32 pin)
{
	__sREG32(addr, GPIO_DIR) |= pin;
}
void silan_gpio_set_high(u32 addr, u32 pin)
{
	__sREG32(addr, GPIO_DATA) |= pin;
}
void silan_gpio_set_low(u32 addr, u32 pin)
{
	__sREG32(addr, GPIO_DATA) &= ~pin;
}
int silan_gpio_get(u32 addr, u32 pin)
{
	if (__sREG32(addr, GPIO_DATA) & pin)
	{
        return 1;
    }
    return 0;
}

void silan_gpio_irq_enable(u32 addr, u32 pin)
{
	__sREG32(addr, GPIO_IE) |= pin;
}

void silan_gpio_irq_clear(u32 addr, u32 pin)
{
	__sREG32(addr, GPIO_IC) |= pin;
}

void silan_gpio_irq_mode(u32 addr, u32 pin, int mode)
{
    u32 base = addr;

    if ((mode == GPIO_High_Level) || (mode == GPIO_Low_Level))
    {
        __sREG32(base,GPIO_IS) |= pin;
    }
    else 
    {
        __sREG32(base,GPIO_IS) &= ~pin;
        if (mode == GPIO_Edge)
        {
            __sREG32(base,GPIO_IBE) |= pin;
        }
    }
    if ((mode == GPIO_High_Level) || (mode == GPIO_High_Edge))
    {
        __sREG32(base,GPIO_IEV) |= pin;
    }
    else 
    {
        __sREG32(base,GPIO_IEV) &= ~pin;
    }
}

static void silan_gpio_filsel_open(u32 addr,u32 pin)
{
	__sREG32(addr, GPIO_FILSEL) |= pin;		
}

static void silan_gpio_filsel_close(u32 addr,u32 pin)
{
	__sREG32(addr, GPIO_FILSEL) &= ~pin;		
}

static void silan_gpio_filsel_div(u32 addr,int div)
{
	__sREG32(addr, GPIO_FILDIV) |= ((div&0xf)<<1)|0x1;		
}

void silan_gpio_filsel_sel(u32 addr,u32 pin,int if_open)
{
	if(if_open){
		silan_gpio_filsel_open(addr,pin);
		silan_gpio_filsel_div(addr,0x3);
	}else{
		silan_gpio_filsel_close(addr,pin);
	}
}

void silan_io_input(int io)
{
	int offset, pin, gpio;

	gpio = IO_TO_GPIO(io);
	offset = GPIO_CONFIG_OFFSET(gpio);
	pin = GPIO_CONFIG_PIN(gpio);
	silan_gpio_input(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
}

void silan_io_output(int io)
{
	int offset, pin, gpio;

	gpio = IO_TO_GPIO(io);
	offset = GPIO_CONFIG_OFFSET(gpio);
	pin = GPIO_CONFIG_PIN(gpio);
	silan_gpio_output(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
}

void silan_io_direction(int io, int dir)
{
	int offset, pin, gpio;

	gpio = IO_TO_GPIO(io);
	offset = GPIO_CONFIG_OFFSET(gpio);
	pin = GPIO_CONFIG_PIN(gpio);
	if (dir)
		silan_gpio_output(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
	else
		silan_gpio_input(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
}

void silan_io_set_value(int io, int val)
{
	int offset, pin, gpio;

	gpio = IO_TO_GPIO(io);
	offset = GPIO_CONFIG_OFFSET(gpio);
	pin = GPIO_CONFIG_PIN(gpio);
	if(val)
		silan_gpio_set_high(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
	else
		silan_gpio_set_low(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
}

void silan_io_set_high(int io)
{
	int offset, pin, gpio;

	gpio = IO_TO_GPIO(io);
	offset = GPIO_CONFIG_OFFSET(gpio);
	pin = GPIO_CONFIG_PIN(gpio);
    silan_gpio_set_high(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
}

void silan_io_set_low(int io)
{
	int offset, pin, gpio;

	gpio = IO_TO_GPIO(io);
	offset = GPIO_CONFIG_OFFSET(gpio);
	pin = GPIO_CONFIG_PIN(gpio);
    silan_gpio_set_low(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
}

int silan_io_get(int io)
{
    int offset, pin, gpio;

	gpio = IO_TO_GPIO(io);
	offset = GPIO_CONFIG_OFFSET(gpio);
	pin = GPIO_CONFIG_PIN(gpio);

	return(silan_gpio_get(SILAN_GPIO_BASE(offset), GPIO_PIN(pin)));
}

void silan_io_toggle(int io)
{
	int offset, pin, gpio;

	gpio = IO_TO_GPIO(io);
	offset = GPIO_CONFIG_OFFSET(gpio);
	pin = GPIO_CONFIG_PIN(gpio);
    if (silan_gpio_get(SILAN_GPIO_BASE(offset), GPIO_PIN(pin)))
		silan_gpio_set_low(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
	else
		silan_gpio_set_high(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
}


void silan_io_irq_enable(int io)
{
    int offset, pin;

	offset = GPIO_CONFIG_OFFSET(io);
	pin = GPIO_CONFIG_PIN(io);

	silan_gpio_irq_enable(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
}

void silan_io_irq_clear(int io)
{
	int offset, pin;

	offset = GPIO_CONFIG_OFFSET(io);
	pin = GPIO_CONFIG_PIN(io);

	silan_gpio_irq_clear(SILAN_GPIO_BASE(offset), GPIO_PIN(pin));
}

void silan_io_irq_mode(int io, int mode)
{
	int offset, pin;

	offset = GPIO_CONFIG_OFFSET(io);
	pin = GPIO_CONFIG_PIN(io);

	silan_gpio_irq_mode(SILAN_GPIO_BASE(offset), GPIO_PIN(pin), mode);
}

void silan_io_filter_config(int gpio, int div)
{
	silan_gpio_filsel_div(SILAN_GPIO_BASE(gpio), div);
}

void silan_io_filter_open(int io)
{
	int offset, pin;

	offset = GPIO_CONFIG_OFFSET(io);
	pin = GPIO_CONFIG_PIN(io);

	silan_gpio_filsel_open(SILAN_GPIO_BASE(pin), offset);
}

void silan_io_filter_close(int io)
{
	int offset, pin;

	offset = GPIO_CONFIG_OFFSET(io);
	pin = GPIO_CONFIG_PIN(io);

	silan_gpio_filsel_open(SILAN_GPIO_BASE(pin), offset);
}

