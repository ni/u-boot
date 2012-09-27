/*
 * USB host controller support for the Zynq-7000 PSS EHCI controller
 *
 * Author: Jeff Westfahl <jeff.westfahl@ni.com>
 * Copyright (c) 2012 National Instruments Corporation.
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
#include <usb/ulpi.h>

#include "../drivers/usb/host/ehci.h"
#include "../drivers/usb/host/ehci-core.h"

/*
 * Create the appropriate control structures to manage
 * a new EHCI host controller.
 */
int ehci_hcd_init(void)
{
	struct ulpi_viewport usb_phy;

	hccr = (struct ehci_hccr *)(CONFIG_USB_BASE_ADDR + XPSS_USB_CAPLENGTH);
	hcor = (struct ehci_hcor *)((uint32_t) hccr +
			HC_LENGTH(ehci_readl(&hccr->cr_capbase)));

	/* ULPI_VIEWPORT register */
	usb_phy.viewport_addr = CONFIG_USB_BASE_ADDR + XPSS_USB_ULPI_VIEWPORT;
	usb_phy.port_num = 0;

	/* Turn on USB power. */
	ulpi_set_vbus(&usb_phy, 1, 0, 0);

	/* Wait for USB power to ramp up, and give devices time to see it. Some
	   devices aren't detected without a delay here. */
	mdelay(500);

	return 0;
}

/*
 * Destroy the appropriate control structures corresponding
 * the the EHCI host controller.
 */
int ehci_hcd_stop(void)
{
	struct ulpi_viewport usb_phy;

	/* ULPI_VIEWPORT register */
	usb_phy.viewport_addr = CONFIG_USB_BASE_ADDR + XPSS_USB_ULPI_VIEWPORT;
	usb_phy.port_num = 0;

	/* Turn off USB power. */
	ulpi_set_vbus(&usb_phy, 0, 0, 0);

	/* Wait for USB power to fall off. If USB power is turned on again too
	   soon, devices may not recognize that power was turned off. */
	mdelay(500);

	return 0;
}

void ehci_powerup_fixup(uint32_t *status_reg, uint32_t *reg)
{
	/* The Zynq EHCI controller behaves slightly differently than other
	   embedded high speed host controllers. It automatically clears
	   EHCI_PS_PR after the reset delay time has elapsed, and sets
	   EHCI_PS_PE at that time. The EHCI host controller driver writes
	   the port status register with EHCI_PS_PR clear to end the reset,
	   but it also has EHCI_PS_PE clear. On Zynq, this disables the port.
	   The EHCI host controller driver thinks the reset failed, and gives
	   up after a few retries. We can easily work around this by updating
	   'reg' to the current value of the port status register before
	   returning. The MX51 Efika board requires something similar, see
	   board/efikamx/efikamx-usb.c. */

	/* Root port reset typically takes about 50ms. */
	#define ZYNQ_EHCI_RESET_WAIT_MSEC 100
	#define ZYNQ_EHCI_RESET_MSEC_PER 10

	int msec = ZYNQ_EHCI_RESET_WAIT_MSEC;

	do {
		mdelay(ZYNQ_EHCI_RESET_MSEC_PER);
		msec -= ZYNQ_EHCI_RESET_MSEC_PER;

		*reg = ehci_readl(status_reg);

		if (!(EHCI_PS_PR & *reg)) {
			debug("EHCI root port reset complete after ~%dms\n",
			      ZYNQ_EHCI_RESET_WAIT_MSEC - msec);
			break;
		}
	} while(0 < msec);

	if (EHCI_PS_PR & *reg)
		printf("EHCI root port reset didn't complete within %dms\n",
		       ZYNQ_EHCI_RESET_WAIT_MSEC);
}
