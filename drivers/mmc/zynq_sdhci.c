/*
 * Copyright 2012 Joe Hershberger <joe.hershberger@ni.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/arch/sdhci.h>
#include <malloc.h>

static char *NAME = "SDHCI";
int zynq_sdhci_init(u32 regbase, u32 max_clk, u32 min_clk, u32 quirks)
{
	struct sdhci_host *host = NULL;
	host = (struct sdhci_host *)malloc(sizeof(struct sdhci_host));
	if (!host) {
		printf("sdh_host malloc fail!\n");
		return 1;
	}

	host->name = NAME;
	host->ioaddr = (void *)regbase;
	host->quirks = quirks;
	host->version = sdhci_readw(host, SDHCI_HOST_VERSION);
	add_sdhci(host, max_clk, min_clk);
	return 0;
}

