/*
 * Copyright (c) 2015 National Instruments
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <serial.h>


#ifndef CONFIG_DM_SERIAL

static int nulldev_init(void)
{
	/* nulldev is empty! */
	return 0;
}

static void nulldev_setbrg(void)
{
	/* nulldev is empty! */
}

static void nulldev_putc(const char c)
{
	/* nulldev is empty! */
}

static void nulldev_puts(const char *s)
{
	/* nulldev is empty! */
}

static int nulldev_input(void)
{
	/* nulldev is empty! */
	return 0;
}

struct serial_device nulldev_serial_device = {
       "nulldev",
       nulldev_init,
       NULL,
       nulldev_setbrg,
       nulldev_input,
       nulldev_input,
       nulldev_putc,
       nulldev_puts,
};

void nulldev_serial_initialize(void)
{
        serial_register(&nulldev_serial_device);
}

__weak struct serial_device *default_serial_console(void)
{
        return &nulldev_serial_device;
}

#else /* CONFIG_DM_SERIAL */

static int nulldev_serial_setbrg(struct udevice *dev, int baudrate)
{
	return 0;
}

static int nulldev_serial_getc(struct udevice *dev)
{
	return -EAGAIN;
}

static int nulldev_serial_input(struct udevice *dev)
{
	return 0;
}

static int nulldev_serial_putc(struct udevice *dev, const char ch)
{
	return 0;
}

static const struct udevice_id nulldev_serial_ids[] = {
	{ .compatible = "nulldev-serial" },
	{ }
};


const struct dm_serial_ops nulldev_serial_ops = {
        .putc = nulldev_serial_putc,
        .getc = nulldev_serial_getc,
        .setbrg = nulldev_serial_setbrg,
};

U_BOOT_DRIVER(serial_nulldev) = {
	.name	= "serial_nulldev",
	.id	= UCLASS_SERIAL,
	.of_match = nulldev_serial_ids,
	.ops	= &nulldev_serial_ops,
};

#endif /* CONFIG_DM_SERIAL */
