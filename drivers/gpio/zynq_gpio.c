/*
 * Driver for the Zynq-7000 PSS GPIO controller
 *
 * Author: Jeff Westfahl <jeff.westfahl@ni.com>
 * Copyright (c) 2013 Jeff Westfahl.
 *
 * This driver is based on the Linux Zynq GPIO driver and on the
 * U-Boot OMAP GPIO driver.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <common.h>
#include <asm/gpio.h>

/* Register offsets for the GPIO device */
#define XGPIOPS_DATA_LSW_OFFSET(BANK)	(0x000 + (0x08 * (BANK)))
#define XGPIOPS_DATA_MSW_OFFSET(BANK)	(0x004 + (0x08 * (BANK)))
#define XGPIOPS_DATA_OFFSET(BANK)	(0x040 + (0x04 * (BANK)))
#define XGPIOPS_DIRM_OFFSET(BANK)	(0x204 + (0x40 * (BANK)))
#define XGPIOPS_OUTEN_OFFSET(BANK)	(0x208 + (0x40 * (BANK)))

#define XGPIOPS_BANK_COUNT	4

/* Data Memory Barrier */
#define dmb() __asm__ __volatile__ ("dmb" : : : "memory")
#define SYNCHRONIZE_IO dmb()

/* Read/Write access to the GPIO PS registers */
static inline void Xout_le32(u32 addr, u32 value)
{
	*(volatile u32 *)((u32 *)addr) = value;
	SYNCHRONIZE_IO;
}

static inline u32 Xin_le32(u32 addr)
{
	volatile u32 temp = *(volatile u32 *)((u32 *)addr);
	SYNCHRONIZE_IO;
	return temp;
}

static unsigned int xgpiops_pin_table[] = {
	31, /* 0 - 31 */
	53, /* 32 - 53 */
	85, /* 54 - 85 */
	117 /* 86 - 117 */
};

static inline int gpio_valid(int gpio)
{
	if (gpio < 0)
		return -1;
	if (gpio <= xgpiops_pin_table[XGPIOPS_BANK_COUNT - 1])
		return 0;
	return -1;
}

static int check_gpio(int gpio)
{
	if (gpio_valid(gpio) < 0) {
		printf("ERROR : check_gpio: invalid GPIO %d\n", gpio);
		return -1;
	}
	return 0;
}

/**
 * xgpiops_get_bank_pin - Get the bank number and pin number within that bank
 * for a given pin in the GPIO device
 * @pin_num:	gpio pin number within the device
 * @bank_num:	an output parameter used to return the bank number of the gpio
 *		pin
 * @bank_pin_num: an output parameter used to return pin number within a bank
 *		  for the given gpio pin
 *
 * Returns the bank number.
 */
static inline void xgpiops_get_bank_pin(unsigned int pin_num,
		unsigned int *bank_num,
		unsigned int *bank_pin_num)
{
	for (*bank_num = 0; *bank_num < XGPIOPS_BANK_COUNT; (*bank_num)++)
		if (pin_num <= xgpiops_pin_table[*bank_num])
			break;

	if (*bank_num == 0)
		*bank_pin_num = pin_num;
	else
		*bank_pin_num = pin_num %
			(xgpiops_pin_table[*bank_num - 1] + 1);
}

/**
 * Get value of the specified gpio
 */
int gpio_get_value(unsigned gpio)
{
	unsigned int reg, bank_num, bank_pin_num;

	if (check_gpio(gpio) < 0)
		return -1;

	xgpiops_get_bank_pin(gpio, &bank_num, &bank_pin_num);

	reg = Xin_le32(XPSS_GPIO_BASEADDR | XGPIOPS_DATA_OFFSET(bank_num));

	return (reg >> bank_pin_num) & 1;
}

/**
 * Set value of the specified gpio
 */
int gpio_set_value(unsigned gpio, int value)
{
	unsigned int reg_offset;
	unsigned int bank_num, bank_pin_num;

	if (check_gpio(gpio) < 0)
		return -1;

	xgpiops_get_bank_pin(gpio, &bank_num, &bank_pin_num);

	if (bank_pin_num >= 16) {
		bank_pin_num -= 16; /* only 16 data bits in bit maskable reg */
		reg_offset = XGPIOPS_DATA_MSW_OFFSET(bank_num);
	} else {
		reg_offset = XGPIOPS_DATA_LSW_OFFSET(bank_num);
	}

	/*
	 * get the 32 bit value to be written to the mask/data register where
	 * the upper 16 bits is the mask and lower 16 bits is the data
	 */
	if (value)
		value = 1;
	value = ~(1 << (bank_pin_num + 16)) & ((value << bank_pin_num) |
					       0xFFFF0000);

	Xout_le32(XPSS_GPIO_BASEADDR | reg_offset, value);

	return 0;
}

/**
 * Set gpio direction as input
 */
int gpio_direction_input(unsigned gpio)
{
	unsigned int reg, bank_num, bank_pin_num;

	if (check_gpio(gpio) < 0)
		return -1;

	xgpiops_get_bank_pin(gpio, &bank_num, &bank_pin_num);

	/* configure the output enable reg for the pin */
	reg = Xin_le32(XPSS_GPIO_BASEADDR | XGPIOPS_OUTEN_OFFSET(bank_num));
	reg &= ~(1 << bank_pin_num);
	Xout_le32(XPSS_GPIO_BASEADDR | XGPIOPS_OUTEN_OFFSET(bank_num), reg);

	/* clear the bit in direction mode reg to set the pin as input */
	reg = Xin_le32(XPSS_GPIO_BASEADDR | XGPIOPS_DIRM_OFFSET(bank_num));
	reg &= ~(1 << bank_pin_num);
	Xout_le32(XPSS_GPIO_BASEADDR | XGPIOPS_DIRM_OFFSET(bank_num), reg);

	return 0;
}

/**
 * Set gpio direction as output
 */
int gpio_direction_output(unsigned gpio, int value)
{
	unsigned int reg, bank_num, bank_pin_num;

	if (check_gpio(gpio) < 0)
		return -1;

	/* set the state of the pin */
	gpio_set_value(gpio, value);

	xgpiops_get_bank_pin(gpio, &bank_num, &bank_pin_num);

	/* set the GPIO pin as output */
	reg = Xin_le32(XPSS_GPIO_BASEADDR | XGPIOPS_DIRM_OFFSET(bank_num));
	reg |= 1 << bank_pin_num;
	Xout_le32(XPSS_GPIO_BASEADDR | XGPIOPS_DIRM_OFFSET(bank_num), reg);

	/* configure the output enable reg for the pin */
	reg = Xin_le32(XPSS_GPIO_BASEADDR | XGPIOPS_OUTEN_OFFSET(bank_num));
	reg |= 1 << bank_pin_num;
	Xout_le32(XPSS_GPIO_BASEADDR | XGPIOPS_OUTEN_OFFSET(bank_num), reg);

	return 0;
}

/**
 * Request a gpio before using it.
 *
 * NOTE: Argument 'label' is unused.
 */
int gpio_request(unsigned gpio, const char *label)
{
	if (check_gpio(gpio) < 0)
		return -1;

	return 0;
}

/**
 * Reset and free the gpio after using it.
 */
int gpio_free(unsigned gpio)
{
	return 0;
}
