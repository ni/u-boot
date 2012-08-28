/*
 * Xilinx Zynq Serial PS driver
 * Based on existing U-boot serial drivers.
 */

#include <common.h>

#include "serial_zynq.h"

DECLARE_GLOBAL_DATA_PTR;

/* Set up the baud rate in gd struct */
static void _zynq_serial_setbrg(int base)
{
	/*              master clock
	 * Baud rate = ---------------
	 *              bgen*(bdiv+1)
	 */
	long baud = gd->baudrate;

	/* Variables to vary. */
	unsigned int bdiv, bgen;
	unsigned int master_clock;

	/* Calculation results. */
	long calc_baud = 0;
	unsigned int calc_bauderror;

	if (base == ZYNQUART0_BASE)
		master_clock = ZYNQUART0_MASTER;
	else if (base == ZYNQUART1_BASE)
		master_clock = ZYNQUART1_MASTER;
	else
		return;

	/* Find acceptable values for baud generation. */
	for (bdiv = 4; bdiv < 255; bdiv++) {

		bgen = master_clock / (baud * (bdiv + 1));
		if (bgen < 2 || bgen > 65535)
			continue;

		calc_baud = master_clock / (bgen * (bdiv + 1));

		/* Use first calculated baudrate with an acceptable
		 * (<3%) error.
		 */
		if (baud > calc_baud)
			calc_bauderror = baud - calc_baud;
		else
			calc_bauderror = calc_baud - baud;
		if (((calc_bauderror * 100) / baud) < 3)
			break;

	}

	zynq_uart_writel(base, BAUDDIV, bdiv);
	zynq_uart_writel(base, BAUDGEN, bgen);
}

/* Initialize the UART, with...some settings. */
static int _zynq_serial_init(int base)
{
	zynq_uart_writel(base, CR, 0x17); /* RX/TX enabled & reset */
	zynq_uart_writel(base, MR, 0x20); /* 8 bit, no parity */
	_zynq_serial_setbrg(base);
	return 0;
}

/* Write a char to the Tx buffer */
static void _zynq_serial_putc(int base, char c)
{
	while ((zynq_uart_readl(base, SR) & ZYNQUART_SR_TXFULL) != 0)
		;
	if (c == '\n') {
		zynq_uart_writel(base, FIFO,'\r');
		while ((zynq_uart_readl(base, SR) & ZYNQUART_SR_TXFULL) != 0)
			;
	}
	zynq_uart_writel(base, FIFO, c);
}

/* Write a null-terminated string to the UART */
static void _zynq_serial_puts(int base, const char *s)
{
	while (*s)
		_zynq_serial_putc(base, *s++);
}

/* Test character presence in Rx buffer */
static int _zynq_serial_tstc(int base)
{
	return (zynq_uart_readl(base, SR) & ZYNQUART_SR_RXEMPTY) == 0;
}

/* Get a char from Rx buffer */
static int _zynq_serial_getc(int base)
{
	while (!_zynq_serial_tstc(base));
	return zynq_uart_readl(base, FIFO);
}

void serial_setbrg(void)
{
	_zynq_serial_setbrg(ZYNQUART_BASE);
}
int serial_init(void)
{
	return _zynq_serial_init(ZYNQUART_BASE);
}
void serial_putc(char c)
{
	_zynq_serial_putc(ZYNQUART_BASE, c);
}
void serial_puts(const char *s)
{
	_zynq_serial_puts(ZYNQUART_BASE, s);
}
int serial_getc(void)
{
	return _zynq_serial_getc(ZYNQUART_BASE);
}
int serial_tstc(void)
{
	return _zynq_serial_tstc(ZYNQUART_BASE);
}
